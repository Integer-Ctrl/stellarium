#include "ScmEditorDialog.hpp"
#include "ui_scmEditorDialog.h"

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
}
