#include "ScmConstellationDialog.hpp"
#include "ui_scmConstellationDialog.h"

ScmConstellationDialog::ScmConstellationDialog(SkyCultureMaker* maker)
	: StelDialogSeparate("ScmConstellationDialog"), maker(maker)
{
	ui = new Ui_scmConstellationDialog;
}

ScmConstellationDialog::~ScmConstellationDialog()
{
	delete ui;
}

void ScmConstellationDialog::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void ScmConstellationDialog::createDialogContent()
{
	ui->setupUi(dialog);

	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	connect(ui->titleBar, &TitleBar::closeClicked, this, &StelDialogSeparate::close);

	connect(ui->penBtn, &QPushButton::clicked, this, &ScmConstellationDialog::togglePen);
}

void ScmConstellationDialog::togglePen()
{
	maker->setConstellationDialogVisibility(true);
}