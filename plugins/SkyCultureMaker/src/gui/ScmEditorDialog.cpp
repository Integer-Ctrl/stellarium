#include "ScmEditorDialog.hpp"
#include "ui_scmEditorDialog.h"
#include "SkyCultureConverter.hpp"
#include "qmicroz.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

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
	ui->fileSystem->setModel(fsModel);
	ui->fileSystem->setRootIndex(fsModel->index(QDir::homePath()));

	connect(ui->fileSystem,
		&QTreeView::clicked,
		this,
		[this, fsModel](const QModelIndex &idx)
		{
			const QString path = fsModel->filePath(idx);
			ui->filePathLineEdit->setText(path);
		});

	connect(
	    ui->pushButton,
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

		    ui->pushButton->setEnabled(false);

		    // Run conversion in a background thread
		    QFuture<QString> future = QtConcurrent::run(
			[path, tempDir, tempFolder]() mutable -> QString
			{
				QString baseName = QFileInfo(path).fileName();	// e.g. "foo.zip"
				QString stem = baseName.left(baseName.lastIndexOf('.'));
				const QString tempDir = QDir::tempPath() + "/skycultures/" + stem;
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
					    "Please select a valid archive file (zip, tar, rar, 7z, gzip…)");
				}

				try
				{
					QMicroz::extract(path, tempDir);
					qDebug() << "Archive extracted to:" << tempDir;
				}
				catch (const std::exception &e)
				{
					return QString("Error extracting archive: %1").arg(e.what());
				}

				QStringList extracted_files =
				    tempFolder.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
				qDebug() << "Extracted files:" << extracted_files.length();
				for (const QString &file : extracted_files)
				{
					qDebug() << " - " << file;
				}
				if (extracted_files.isEmpty())
				{
					return "No files found in the archive.";
				}

				// set source as the folder that gets converted
				QString source;
				if (extracted_files.contains("info.ini"))
					source = tempDir;
				else if (extracted_files.length() == 1)
					source = tempDir + "/" + extracted_files.first();
				else
					return "Invalid archive structure. Expected 'info.ini' or a single subfolder.";

				qDebug() << "Source for conversion:" << source;

				// Destination is where the converted files will be saved
				QString destDir =
				    QDir::homePath() + "/Documents/SkyCultureConverter/" +
				    QFileInfo(path).fileName().left(QFileInfo(path).fileName().lastIndexOf('.'));
				qDebug() << "Destination for conversion:" << destDir;

				SkyCultureConverter::ReturnValue result;

				try
				{
					result = SkyCultureConverter::convert(source, destDir);
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

		    // Watcher to re-enable the button & report result on UI thread
		    auto *watcher = new QFutureWatcher<QString>(this);
		    connect(watcher,
			    &QFutureWatcher<QString>::finished,
			    this,
			    [this, watcher, tempFolder]() mutable
			    {
				    QString resultText = watcher->future().result();
				    ui->filePathLineEdit->setText(resultText);
				    tempFolder.removeRecursively();
				    ui->pushButton->setEnabled(true);
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
