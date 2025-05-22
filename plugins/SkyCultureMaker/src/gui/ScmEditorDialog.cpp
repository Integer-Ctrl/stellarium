#include "ScmEditorDialog.hpp"
#include "ui_scmEditorDialog.h"
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMessageBox>

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

	/* ============================================= SCM importer/converter ============================================= */
	auto fsModel = new QFileSystemModel(this);
	fsModel->setRootPath(QDir::homePath());
	ui->fileSystem->setModel(fsModel);
	ui->fileSystem->setRootIndex(fsModel->index(QDir::homePath()));

	connect(ui->fileSystem,
		&QTreeView::doubleClicked,
		this,
		[this, fsModel](const QModelIndex &idx)
		{
			const QString path = fsModel->filePath(idx);
			ui->filePathLineEdit->setText(path);
		});
	connect(ui->pushButton, &QPushButton::clicked, this, [this]()
		{
			const QString path = ui->filePathLineEdit->text();
			if (path.isEmpty())
			{
				QMessageBox::warning(dialog, tr("No file selected"), tr("Please pick a file first."));
				return;
			}
			qDebug() << "Importing file:" << path;
		});
	/* ================================================================================================================== */
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
