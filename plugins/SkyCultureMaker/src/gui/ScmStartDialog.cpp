#include "ScmStartDialog.hpp"
#include "ui_scmStartDialog.h"

ScmStartDialog::ScmStartDialog()
	: StelDialog("ScmStartDialog")
{
	ui = new Ui_scmStartDialog;
}

ScmStartDialog::~ScmStartDialog()
{
	delete ui;
}

void ScmStartDialog::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void ScmStartDialog::createDialogContent()
{
	ui->setupUi(dialog);

	// connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
	// connect(ui->titleBar, &TitleBar::closeClicked, this, &StelDialog::close);
	// connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
}
