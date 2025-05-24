#include "ScmConstellationDialog.hpp"
#include "ui_scmConstellationDialog.h"
#include "StelGui.hpp"

ScmConstellationDialog::ScmConstellationDialog(SkyCultureMaker *maker)
	: StelDialogSeparate("ScmConstellationDialog")
	, maker(maker)
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

	connect(ui->penBtn, &QPushButton::toggled, this, &ScmConstellationDialog::togglePen);
	connect(ui->eraserBtn, &QPushButton::toggled, this, &ScmConstellationDialog::toggleEraser);
	connect(ui->undoBtn, &QPushButton::clicked, this, &ScmConstellationDialog::triggerUndo);
}

void ScmConstellationDialog::togglePen(bool checked)
{
	if (checked)
	{
		ui->eraserBtn->setChecked(false);
		activeTool = Tools::Pen;
		maker->setIsLineDrawEnabled(true);
	}
	else
	{
		activeTool = Tools::None;
		maker->setIsLineDrawEnabled(false);
	}
}

void ScmConstellationDialog::toggleEraser(bool checked)
{
	if (checked)
	{
		ui->penBtn->setChecked(false);
		activeTool = Tools::Eraser;
		maker->setIsLineDrawEnabled(true);
	}
	else
	{
		activeTool = Tools::None;
		maker->setIsLineDrawEnabled(false);
	}
}

void ScmConstellationDialog::triggerUndo()
{
	maker->triggerDrawUndo();
}