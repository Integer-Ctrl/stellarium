#include "StartDialog.hpp"
#include "ui_startDialog.h"

StartDialog::StartDialog()
	: StelDialog("StartDialog")
{
	ui = new Ui_startDialog;
}

StartDialog::~StartDialog()
{
	delete ui;
}

void StartDialog::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void StartDialog::createDialogContent()
{
	ui->setupUi(dialog);

	connect(&StelApp::getInstance(), SIGNAL(languageChanged()),this, SLOT(retranslate()));

	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	connect(ui->titleBar, &TitleBar::closeClicked, this, &StelDialog::close);
}
