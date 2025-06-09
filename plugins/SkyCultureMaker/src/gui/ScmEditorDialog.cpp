#include "ScmEditorDialog.hpp"
#include "ui_scmEditorDialog.h"
#include "SkyCultureConverter.hpp"
#include "unarr.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>
#include <QFile>
#include <QFileInfo>
#include <QDir>

// Code snippert from https://github.com/selmf/unarr/blob/master/test/main.c
ar_archive *ar_open_any_archive(ar_stream *stream, const char *fileext)
{
	ar_archive *ar = ar_open_rar_archive(stream);
	if (!ar)
		ar = ar_open_zip_archive(stream,
					 fileext && (strcmp(fileext, ".xps") == 0 || strcmp(fileext, ".epub") == 0));
	if (!ar)
		ar = ar_open_7z_archive(stream);
	if (!ar)
		ar = ar_open_tar_archive(stream);
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
		QString name = QString::fromUtf8(ar_entry_get_name(archive));
		QString outPath = destinationPath + "/" + name;
		QDir().mkpath(QFileInfo(outPath).path());
		QFile outFile(outPath);
		if (outFile.open(QIODevice::WriteOnly))
		{
			qint64 remaining = ar_entry_get_size(archive);
			const qint64 bufSize = 8192;
			while (remaining > 0)
			{
				qint64 chunk = qMin<qint64>(remaining, bufSize);
				QByteArray buffer(chunk, 0);
				if (!ar_entry_uncompress(
					archive, reinterpret_cast<unsigned char *>(buffer.data()), chunk))
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

ScmEditorDialog::ScmEditorDialog()
	: StelDialog("ScmEditorDialog")
{
	ui = new Ui_scmEditorDialog;
}

ScmEditorDialog::~ScmEditorDialog()
{
	delete ui;
}

void ScmEditorDialog::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void ScmEditorDialog::createDialogContent()
{
	ui->setupUi(dialog);

	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	connect(ui->titleBar, &TitleBar::closeClicked, this, &StelDialog::close);

	// LABELS TAB
	connect(ui->enNameTE,
		&QTextEdit::textChanged,
		this,
		[this]()
		{
			constellationEnglishName = ui->enNameTE->toPlainText();
			if (constellationEnglishName.isEmpty())
			{
				ui->saveLabelsBtn->setEnabled(false);
			}
			else
			{
				ui->saveLabelsBtn->setEnabled(true);
			}
			updateLabelsSavedLabel(false);
		});
	connect(ui->natNameTE,
		&QTextEdit::textChanged,
		this,
		[this]()
		{
			constellationNativeName = ui->natNameTE->toPlainText();
			if (constellationNativeName->isEmpty())
			{
				constellationNativeName = std::nullopt;
			}
			updateLabelsSavedLabel(false);
		});
	connect(ui->pronounceTE,
		&QTextEdit::textChanged,
		this,
		[this]()
		{
			constellationPronounce = ui->pronounceTE->toPlainText();
			if (constellationPronounce->isEmpty())
			{
				constellationPronounce = std::nullopt;
			}
			updateLabelsSavedLabel(false);
		});
	connect(ui->ipaTE,
		&QTextEdit::textChanged,
		this,
		[this]()
		{
			constellationIpa = ui->ipaTE->toPlainText();
			if (constellationIpa->isEmpty())
			{
				constellationIpa = std::nullopt;
			}
			updateLabelsSavedLabel(false);
		});
	ui->saveLabelsBtn->setEnabled(false);
	connect(ui->saveLabelsBtn, &QPushButton::clicked, this, &ScmEditorDialog::saveLabels);
	updateLabelsSavedLabel(false);

	/* ============================================= SCM importer/converter */
	auto fsModel = new QFileSystemModel(this);
	fsModel->setRootPath(QDir::homePath());

	connect(ui->browseButton,
		&QPushButton::clicked,
		this,
		[this]()
		{
			const QString file = QFileDialog::getOpenFileName(nullptr,
									  tr("Select an archive"),
									  QDir::homePath(),
									  tr("Archives (*.zip *.rar *.7z *.tar)"));
			if (!file.isEmpty())
			{
				ui->filePathLineEdit->setText(file);
			}
		});

	connect(
	    ui->importButton,
	    &QPushButton::clicked,
	    this,
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
		    QString baseName = QFileInfo(path).fileName();  // e.g. "foo.zip"
		    int dotPos = baseName.indexOf('.');
		    QString stem =
			(dotPos == -1) ? baseName : baseName.left(dotPos);  // Extract the part before the first dot

		    const QString tempDir = QDir::tempPath() + "/skycultures/" + stem;
		    QDir().mkpath(tempDir);
		    QDir tempFolder(tempDir);

		    // Destination is where the converted files will be saved temporarily
		    const QString tempDestDir = QDir::tempPath() + "/skycultures/results/" + stem;
		    QDir tempDestFolder(tempDestDir);

		    ui->importButton->setEnabled(false);

		    // Run conversion in a background thread
		    QFuture<QString> future = QtConcurrent::run(
			[path, tempDir, tempFolder, tempDestDir, tempDestFolder, stem]() mutable -> QString
			{
				// Check if the file is a valid archive
				QMimeDatabase db;
				QMimeType mime = db.mimeTypeForFile(path, QMimeDatabase::MatchContent);

				static const QStringList archiveTypes = {QStringLiteral("application/zip"),
									 QStringLiteral("application/x-tar"),
									 QStringLiteral("application/x-7z-compressed"),
									 QStringLiteral("application/gzip"),
									 QStringLiteral("application/x-rar-compressed"),
									 QStringLiteral("application/vnd.rar")};

				if (!archiveTypes.contains(mime.name()))
				{
					return QStringLiteral(
					    "Please select a valid archive file (zip, tar, rar or 7z)");
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

				QStringList extracted_files =
				    tempFolder.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

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
					return "Invalid archive structure. Expected 'info.ini' or a single subfolder.";

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
					default:
						msg = "Unknown error.";
						break;
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
					qDebug() << "Target folder" << targetFolderPath << "already exists. No move operation performed.";
				}
				else
				{
					// Target folder does not exist, attempt to move/rename tempDestFolder.
					// tempDestFolder.path() gives the full path to the source directory.
					// QDir().rename is a common way to move/rename a directory.
					if (QDir().rename(tempDestFolder.path(), targetFolderPath))
					{
						moveOperationCompleted = true;
						qDebug() << "Successfully moved contents of" << tempDestFolder.path() << "to" << targetFolderPath;
					}
					else
					{
						moveOperationCompleted = false;
						qWarning() << "Failed to move" << tempDestFolder.path() << "to" << targetFolderPath;
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
		    connect(watcher,
			    &QFutureWatcher<QString>::finished,
			    this,
			    [this, watcher, tempFolder, tempDestFolder, moveOperationCompleted]() mutable
			    {
				    QString resultText = watcher->future().result();
					if (!moveOperationCompleted) {
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
	connect(ui->tabs,
		&QTabWidget::currentChanged,
		this,
		[this, importPage](int idx)
		{
			if (idx != importPage)
			{
				ui->filePathLineEdit->clear();
			}
		});
	/* ==================================================================== */
}

void ScmEditorDialog::saveLabels()
{
	qDebug() << "ScmEditorDialog: Saving labels:";
	qDebug() << "	English Name:" << constellationEnglishName;
	if (constellationNativeName)
		qDebug() << "	Native Name:" << constellationNativeName.value_or("N/A");
	if (constellationPronounce)
		qDebug() << "	Pronounce:" << constellationPronounce.value_or("N/A");
	if (constellationIpa)
		qDebug() << "	IPA:" << constellationIpa.value_or("N/A");

	updateLabelsSavedLabel(true);
}

void ScmEditorDialog::updateLabelsSavedLabel(bool saved)
{
	if (saved)
	{
		ui->labelsSavedLbl->setText("Saved labels.");
	}
	else
	{
		ui->labelsSavedLbl->setText("");
	}
}
