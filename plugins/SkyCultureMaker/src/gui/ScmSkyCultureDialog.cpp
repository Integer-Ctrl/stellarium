#include "ScmSkyCultureDialog.hpp"
#include "ui_scmSkyCultureDialog.h"

#ifdef SCM_IMPORTER_ENABLED_CPP
# include <QFileDialog>
# include <QFileSystemModel>
# include <QFutureWatcher>
# include <QMimeDatabase>
# include <QtConcurrent/QtConcurrent>
#else // SCM_IMPORTER_ENABLED_CPP not defined
# include <QLabel>
# include <QVBoxLayout>
# include <QtGlobal> // For QT_VERSION_STR
#endif               // SCM_IMPORTER_ENABLED_CPP

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#ifdef SCM_IMPORTER_ENABLED_CPP
// Code snippert from https://github.com/selmf/unarr/blob/master/test/main.c
ar_archive *ar_open_any_archive(ar_stream *stream, const char *fileext)
{
	ar_archive *ar = ar_open_rar_archive(stream);
	if (!ar)
		ar = ar_open_zip_archive(stream,
		                         fileext && (strcmp(fileext, ".xps") == 0 || strcmp(fileext, ".epub") == 0));
	if (!ar) ar = ar_open_7z_archive(stream);
	if (!ar) ar = ar_open_tar_archive(stream);
	return ar;
}

QString extractArchive(const QString &archivePath, const QString &destinationPath)
{
	ar_stream *stream = ar_open_file(archivePath.toUtf8().constData());
	if (!stream)
	{
		return QString("Failed to open archive: %1").arg(archivePath);
	}
	ar_archive *archive = ar_open_any_archive(stream, QFileInfo(archivePath).suffix().toUtf8().constData());
	if (!archive)
	{
		ar_close(stream);
		return QString("Failed to open archive: %1").arg(archivePath);
	}

	// iterate entries and decompress each
	while (ar_parse_entry(archive))
	{
		QString name    = QString::fromUtf8(ar_entry_get_name(archive));
		QString outPath = destinationPath + "/" + name;
		QDir().mkpath(QFileInfo(outPath).path());
		QFile outFile(outPath);
		if (outFile.open(QIODevice::WriteOnly))
		{
			qint64 remaining     = ar_entry_get_size(archive);
			const qint64 bufSize = 8192;
			while (remaining > 0)
			{
				qint64 chunk = qMin<qint64>(remaining, bufSize);
				QByteArray buffer(chunk, 0);
				if (!ar_entry_uncompress(archive, reinterpret_cast<unsigned char *>(buffer.data()),
				                         chunk))
					break;
				outFile.write(buffer);
				remaining -= chunk;
			}
			outFile.close();
		}
	}
	ar_close_archive(archive);
	ar_close(stream);

	return QString();
}
#endif // SCM_IMPORTER_ENABLED_CPP

ScmSkyCultureDialog::ScmSkyCultureDialog(SkyCultureMaker *maker)
	: StelDialogSeparate("ScmSkyCultureDialog")
	, maker(maker)
{
	ui = new Ui_scmSkyCultureDialog;
}

ScmSkyCultureDialog::~ScmSkyCultureDialog()
{
	delete ui;
}

void ScmSkyCultureDialog::setConstellations(std::vector<scm::ScmConstellation> *constellations)
{
	ScmSkyCultureDialog::constellations = constellations;
	if (ui && dialog && constellations != nullptr)
	{
		ui->constellationsList->clear();
		for (const auto &constellation : *constellations)
		{
			// Add the constellation to the list widget
			ui->constellationsList->addItem(getDisplayNameFromConstellation(constellation));
		}
	}
}

void ScmSkyCultureDialog::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void ScmSkyCultureDialog::close()
{
	maker->setSkyCultureDialogVisibility(false);
}

void ScmSkyCultureDialog::createDialogContent()
{
	ui->setupUi(dialog);

	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	connect(ui->titleBar, &TitleBar::closeClicked, this, &ScmSkyCultureDialog::close);

	// Overview Tab
	connect(ui->skyCultureNameTE, &QTextEdit::textChanged, this,
	        [this]()
	        {
			name = ui->skyCultureNameTE->toPlainText();
			if (name.isEmpty())
			{
				ui->SaveSkyCultureBtn->setEnabled(false);
			}
			else
			{
				ui->SaveSkyCultureBtn->setEnabled(true);
			}
			setIdFromName(name);
		});

	ui->SaveSkyCultureBtn->setEnabled(false);
	ui->RemoveConstellationBtn->setEnabled(false);
	connect(ui->SaveSkyCultureBtn, &QPushButton::clicked, this, &ScmSkyCultureDialog::saveSkyCulture);
	connect(ui->AddConstellationBtn, &QPushButton::clicked, this, &ScmSkyCultureDialog::constellationDialog);
	connect(ui->RemoveConstellationBtn, &QPushButton::clicked, this,
	        &ScmSkyCultureDialog::removeSelectedConstellation);
	connect(ui->constellationsList, &QListWidget::itemSelectionChanged, this,
	        &ScmSkyCultureDialog::updateRemoveConstellationButton);

	/* ============================================= SCM importer/converter */
	const int importTabIndex = ui->tabs->indexOf(ui->Import);
#ifdef SCM_IMPORTER_ENABLED_CPP
	qDebug() << "SCM Importer is ENABLED - setting up importer UI";
	if (importTabIndex != -1)
	{
		connect(ui->browseButton, &QPushButton::clicked, this,
		        [this]()
		        {
				const QString file =
					QFileDialog::getOpenFileName(nullptr, tr("Select an archive"), QDir::homePath(),
			                                             tr("Archives (*.zip *.rar *.7z *.tar)"));
				if (!file.isEmpty())
				{
					ui->filePathLineEdit->setText(file);
				}
			});

		connect(ui->importButton, &QPushButton::clicked, this,
		        [this]()
		        {
				const QString path = ui->filePathLineEdit->text();
				if (path.isEmpty())
				{
					ui->filePathLineEdit->setText("Please select a file.");
					return;
				}

				qDebug() << "Selected file:" << path;

				// Create a temporary directory for extraction
				QString baseName = QFileInfo(path).fileName(); // e.g. "foo.zip"
				int dotPos       = baseName.indexOf('.');
				QString stem     = (dotPos == -1)
			                                   ? baseName
			                                   : baseName.left(dotPos); // Extract the part before the first dot

				const QString tempDir = QDir::tempPath() + "/skycultures/" + stem;
				QDir().mkpath(tempDir);
				QDir tempFolder(tempDir);

				// Destination is where the converted files will be saved temporarily
				const QString tempDestDir = QDir::tempPath() + "/skycultures/results/" + stem;
				QDir tempDestFolder(tempDestDir);

				ui->importButton->setEnabled(false);

				// Run conversion in a background thread
				QFuture<QString> future = QtConcurrent::run(
					[path, tempDir, tempFolder, tempDestDir, tempDestFolder,
			                 stem]() mutable -> QString
					{
						// Check if the file is a valid archive
						QMimeDatabase db;
						QMimeType mime = db.mimeTypeForFile(path, QMimeDatabase::MatchContent);

						static const QStringList archiveTypes =
							{QStringLiteral("application/zip"),
				                         QStringLiteral("application/x-tar"),
				                         QStringLiteral("application/x-7z-compressed"),
				                         QStringLiteral("application/gzip"),
				                         QStringLiteral("application/x-rar-compressed"),
				                         QStringLiteral("application/vnd.rar")};

						if (!archiveTypes.contains(mime.name()))
						{
							return QStringLiteral("Please select a valid archive file "
					                                      "(zip, tar, rar or 7z)");
						}

						try
						{
							// Extract the archive to the temporary directory
							qDebug() << "Extracting archive:" << path << "to" << tempDir;

							QString error = extractArchive(path, tempDir);
							if (!error.isEmpty())
							{
								return error;
							}

							qDebug() << "Archive extracted to:" << tempDir;
						}
						catch (const std::exception &e)
						{
							return QString("Error extracting archive: %1").arg(e.what());
						}

						QStringList extracted_files = tempFolder.entryList(
							QDir::AllEntries | QDir::NoDotAndDotDot);

						qDebug() << "Extracted files:" << extracted_files.length();

						if (extracted_files.isEmpty())
						{
							return "No files found in the archive.";
						}

						// set source as the folder that gets converted
						// Archive can have a single folder with the skyculture files or
						// an the skyculture files directly in the root
						QString source;
						if (extracted_files.contains("info.ini"))
							source = tempDir;
						else if (extracted_files.length() == 1)
							source = tempDir + "/" + extracted_files.first();
						else
							return "Invalid archive structure. Expected 'info.ini' or a "
							       "single "
							       "subfolder.";

						qDebug() << "Source for conversion:" << source;
						qDebug() << "Destination for conversion:" << tempDestDir;

						SkyCultureConverter::ReturnValue result;

						try
						{
							result = SkyCultureConverter::convert(source, tempDestDir);
						}
						catch (const std::exception &e)
						{
							return QString("Error during conversion: %1").arg(e.what());
						}

						QString msg;
						switch (result)
						{
						case SkyCultureConverter::ReturnValue::CONVERT_SUCCESS:
							msg = "Conversion completed successfully.";
							break;
						case SkyCultureConverter::ReturnValue::ERR_OUTPUT_DIR_EXISTS:
							msg = "Output directory already exists.";
							break;
						case SkyCultureConverter::ReturnValue::ERR_INFO_INI_NOT_FOUND:
							msg = "info.ini not found in the archive.";
							break;
						case SkyCultureConverter::ReturnValue::ERR_OUTPUT_DIR_CREATION_FAILED:
							msg = "Failed to create output directory.";
							break;
						case SkyCultureConverter::ReturnValue::ERR_OUTPUT_FILE_WRITE_FAILED:
							msg = "Failed to write output file.";
							break;
						default: msg = "Unknown error."; break;
						}

						qDebug() << "Conversion result:" << msg;
						return msg;
					});

				// Prompt for explorer view for the destination folder
				const QString destDir = QFileDialog::getExistingDirectory(
					nullptr, tr("Select a directory to save the converted files"), QDir::homePath());

				bool moveOperationCompleted = false; // This boolean tracks the outcome

				if (!destDir.isEmpty())
				{
					// Construct the target path for the folder using 'stem'
					QString targetFolderPath = QDir(destDir).filePath(stem);
					QDir targetDir(targetFolderPath); // QDir object for checking existence

					if (targetDir.exists())
					{
						// Target folder already exists. Do not copy/move.
						moveOperationCompleted = false;
						qDebug() << "Target folder" << targetFolderPath
							 << "already exists. No move operation performed.";
					}
					else
					{
						// Target folder does not exist, attempt to move/rename tempDestFolder.
						// tempDestFolder.path() gives the full path to the source directory.
						// QDir().rename is a common way to move/rename a directory.
						if (QDir().rename(tempDestFolder.path(), targetFolderPath))
						{
							moveOperationCompleted = true;
							qDebug() << "Successfully moved contents of"
								 << tempDestFolder.path() << "to" << targetFolderPath;
						}
						else
						{
							moveOperationCompleted = false;
							qWarning() << "Failed to move" << tempDestFolder.path() << "to"
								   << targetFolderPath;
							// The original code had a comment about attempting to copy if rename fails.
							// This fallback is not included here as per the new requirements.
						}
					}
				}
				else
				{
					// User cancelled the dialog or selected nothing
					qDebug() << "Conversion aborted. No destination directory selected.";
				}

				// At this point, 'moveOperationCompleted' holds the status of the operation.
				// The original 'else' for if (!destDir.isEmpty()) was:
				// }
				// else
				// {
				// User cancelled the dialog or selected nothing
				// qDebug() << "Conversion aborted. No destination directory selected.";
				// }
				// This is covered by the logic above. The qDebug message for cancellation is present.
				// User cancelled the dialog or selected nothing

				// Watcher to re-enable the button & report result on UI thread
				auto *watcher = new QFutureWatcher<QString>(this);
				connect(watcher, &QFutureWatcher<QString>::finished, this,
			                [this, watcher, tempFolder, tempDestFolder, moveOperationCompleted]() mutable
			                {
						QString resultText = watcher->future().result();
						if (!moveOperationCompleted)
						{
							resultText = "Output directory already exists.";
						}
						ui->filePathLineEdit->setText(resultText);
						tempFolder.removeRecursively();
						tempDestFolder.removeRecursively();
						ui->importButton->setEnabled(true);
						watcher->deleteLater();
					});
				watcher->setFuture(future);

				qDebug() << "Conversion ended.";
			});
		// Reset the dialog when switching tabs
		const int importPage = ui->tabs->indexOf(ui->Import);
		connect(ui->tabs, &QTabWidget::currentChanged, this,
		        [this, importPage](int idx)
		        {
				if (idx != importPage)
				{
					ui->filePathLineEdit->clear();
				}
			});
	}
#else   // SCM_IMPORTER_ENABLED_CPP is not defined
	// Importer is disabled
	qDebug() << "SCM Importer is DISABLED - replacing import tab with incompatibility message";
	if (importTabIndex != -1)
	{
		QWidget *importPageWidget = ui->tabs->widget(importTabIndex);
		if (importPageWidget)
		{
			ui->tabs->setTabText(importTabIndex, tr("Import (Disabled)"));

			// Hide all child widgets first
			QList<QWidget*> allWidgets = importPageWidget->findChildren<QWidget*>();
			for (QWidget* widget : allWidgets)
			{
				widget->hide();
				widget->deleteLater();
			}

			// Remove and delete the current layout
			QLayout *currentLayout = importPageWidget->layout();
			if (currentLayout)
			{
				QLayoutItem *item;
				while ((item = currentLayout->takeAt(0)) != nullptr)
				{
					if (item->widget())
					{
						item->widget()->deleteLater();
					}
					delete item;
				}
				delete currentLayout;
			}

			// Create new layout with incompatibility message
			QVBoxLayout *newLayout = new QVBoxLayout();
			QLabel *incompatibilityLabel = new QLabel(
				tr("The Sky Culture Importer feature is not available.\nIt requires Qt6 or later, but "
			           "Stellarium is currently using Qt%1.")
					.arg(QT_VERSION_STR));
			incompatibilityLabel->setWordWrap(true);
			incompatibilityLabel->setAlignment(Qt::AlignCenter);
			incompatibilityLabel->setStyleSheet("QLabel { margin: 10px; }");

			newLayout->addStretch();
			newLayout->addWidget(incompatibilityLabel);
			newLayout->addStretch();
			importPageWidget->setLayout(newLayout);
		}
	}
#endif  // SCM_IMPORTER_ENABLED_CPP
	/* ==================================================================== */
}

void ScmSkyCultureDialog::saveSkyCulture() {}

void ScmSkyCultureDialog::removeSelectedConstellation()
{
	auto selectedItems = ui->constellationsList->selectedItems();
	if (!selectedItems.isEmpty() && constellations != nullptr)
	{
		QListWidgetItem *item     = selectedItems.first();
		QString constellationName = item->text();

		// Get Id by comparing to the display name
		// This will always work, even when the constellation id
		// or name contains special characters
		QString selectedConstellationId = "";
		for (const auto &constellation : *constellations)
		{
			if (constellationName == (getDisplayNameFromConstellation(constellation)))
			{
				selectedConstellationId = constellation.getId();
				break;
			}
		}
		// Remove the constellation from the SC
		maker->getCurrentSkyCulture()->removeConstellation(selectedConstellationId);
		// Disable removal button
		ui->RemoveConstellationBtn->setEnabled(false);
		// The reason for not just removing the constellation in the UI here is that
		// in case the constellation could not be removed from the SC, the UI
		// and the SC would be out of sync
		maker->updateSkyCultureDialog();
	}
}

void ScmSkyCultureDialog::constellationDialog()
{
	maker->setConstellationDialogVisibility(true);
}

void ScmSkyCultureDialog::setIdFromName(QString &name)
{
	QString id = name.toLower().replace(" ", "_");
	maker->getCurrentSkyCulture()->setId(id);
}

void ScmSkyCultureDialog::updateRemoveConstellationButton()
{
	if (!ui->constellationsList->selectedItems().isEmpty())
	{
		ui->RemoveConstellationBtn->setEnabled(true);
	}
	else
	{
		ui->RemoveConstellationBtn->setEnabled(false);
	}
}

QString ScmSkyCultureDialog::getDisplayNameFromConstellation(const scm::ScmConstellation &constellation) const
{
	return constellation.getEnglishName() + " (" + constellation.getId() + ")";
}
