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
