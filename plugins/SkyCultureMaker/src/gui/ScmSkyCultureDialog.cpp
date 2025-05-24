#include "ScmSkyCultureDialog.hpp"
#include "ui_scmSkyCultureDialog.h"

ScmSkyCultureDialog::ScmSkyCultureDialog(SkyCultureMaker* maker)
	: StelDialogSeparate("ScmSkyCultureDialog"), maker(maker)
{
	ui = new Ui_scmSkyCultureDialog;
}

ScmSkyCultureDialog::~ScmSkyCultureDialog()
{
	delete ui;
}

void ScmSkyCultureDialog::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void ScmSkyCultureDialog::createDialogContent()
{
	ui->setupUi(dialog);

	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	connect(ui->titleBar, &TitleBar::closeClicked, this, &StelDialogSeparate::close);

	// Overview Tab
	connect(ui->constellationNameTE, &QTextEdit::textChanged, this, [this]()
	{
		constellationEnglishName = ui->constellationNameTE->toPlainText();
		if (constellationEnglishName.isEmpty())
		{
			ui->SaveSkyCultureBtn->setEnabled(false);
		}
		else
		{
			ui->SaveSkyCultureBtn->setEnabled(true);
		}
		updateSkyCultureSave(false);
	});

	ui->SaveSkyCultureBtn->setEnabled(false);
	connect(ui->SaveSkyCultureBtn, &QPushButton::clicked, this, &ScmSkyCultureDialog::saveSkyCulture);
	connect(ui->AddConstellationBtn, &QPushButton::clicked, this, &ScmSkyCultureDialog::constellationDialog);
	connect(ui->RemoveConstellationBtn, &QPushButton::clicked, this, &ScmSkyCultureDialog::removeConstellation);
	updateSkyCultureSave(false);
}

void ScmSkyCultureDialog::updateSkyCultureSave(bool saved)
{
}

void ScmSkyCultureDialog::saveSkyCulture()
{
	qDebug() << "ScmSkyCulture Dialog: Saving";
	qDebug() << "	English Name:" << constellationEnglishName;
	if(constellationNativeName) qDebug() << "	Native Name:" << constellationNativeName.value_or("N/A");
	if(constellationPronounce) qDebug() << "	Pronounce:" << constellationPronounce.value_or("N/A");
	if(constellationIpa) qDebug() << "	IPA:" << constellationIpa.value_or("N/A");

	updateSkyCultureSave(true);
}

void ScmSkyCultureDialog::removeConstellation()
{
	// TODO: Implement logic to remove the selected constellation
	qDebug() << "Removed a constellation";
}

void ScmSkyCultureDialog::constellationDialog()
{
	maker->setConstellationDialogVisibility(true); // Disable the Sky Culture Maker 
}