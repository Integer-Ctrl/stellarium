#include "ScmStartDialog.hpp"
#include "ui_scmStartDialog.h"

ScmStartDialog::ScmStartDialog(SkyCultureMaker* maker)
	: StelDialog("ScmStartDialog"), maker(maker)
{
	ui = new Ui_scmStartDialog;
}

ScmStartDialog::~ScmStartDialog()
{
	if (ui != nullptr)
	{
		delete ui;
	}
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
	connect(ui->titleBar, &TitleBar::closeClicked, this, &ScmStartDialog::closeDialog);
	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));

	// Buttons
	connect(ui->scmStartCancelpushButton, &QPushButton::clicked, this, &ScmStartDialog::closeDialog); // Cancel
	connect(ui->scmStartCreatepushButton, &QPushButton::clicked, this, &ScmStartDialog::startScmCreationProcess); // Create
	connect(ui->scmStartEditpushButton, &QPushButton::clicked, this, &ScmStartDialog::closeDialog); // Edit - TODO: add logic (currently closing the window)
}

void ScmStartDialog::startScmCreationProcess()
{
	dialog->setVisible(false); // Close the dialog before starting the editor
	maker->setSkyCultureDialogVisibility(true); // Start the editor dialog for creating a new Sky Culture

	SkyCultureMaker::pressKey(Qt::Key_F5); // Simulate pressing F5 to trigger Datetime window
	SkyCultureMaker::pressKey(Qt::Key_F6); // Simulate pressing F6 to trigger Location window
}

void ScmStartDialog::closeDialog()
{
	StelDialog::close();
	maker->setIsScmEnabled(false); // Disable the Sky Culture Maker 
}