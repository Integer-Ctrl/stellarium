#include "ScmEditorDialog.hpp"
#include "ui_scmEditorDialog.h"

ScmEditorDialog::ScmEditorDialog()
	: StelDialogSeparate("ScmEditorDialog")
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
	connect(ui->enNameTE, &QTextEdit::textChanged, this, [this]() 
	{
		constellationEnglishName = ui->enNameTE->toPlainText();
		if(constellationEnglishName.isEmpty())
		{
			ui->saveLabelsBtn->setEnabled(false);
		}
		else
		{
			ui->saveLabelsBtn->setEnabled(true);
		}
		updateLabelsSavedLabel(false);
	});
	connect(ui->natNameTE, &QTextEdit::textChanged, this, [this]() 
	{
		constellationNativeName = ui->natNameTE->toPlainText();
		if (constellationNativeName->isEmpty())
		{
			constellationNativeName = std::nullopt;
		}
		updateLabelsSavedLabel(false);
	});
	connect(ui->pronounceTE, &QTextEdit::textChanged, this, [this]() 
	{
		constellationPronounce = ui->pronounceTE->toPlainText();
		if (constellationPronounce->isEmpty())
		{
			constellationPronounce = std::nullopt;
		}
		updateLabelsSavedLabel(false);
	});
	connect(ui->ipaTE, &QTextEdit::textChanged, this, [this]() 
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
}

void ScmEditorDialog::saveLabels()
{
	qDebug() << "ScmEditorDialog: Saving labels:";
	qDebug() << "	English Name:" << constellationEnglishName;
	if(constellationNativeName) qDebug() << "	Native Name:" << constellationNativeName.value_or("N/A");
	if(constellationPronounce) qDebug() << "	Pronounce:" << constellationPronounce.value_or("N/A");
	if(constellationIpa) qDebug() << "	IPA:" << constellationIpa.value_or("N/A");

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
