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
	ui->saveBtn->setEnabled(false);

	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	connect(ui->titleBar, &TitleBar::closeClicked, this, &StelDialogSeparate::close);

	connect(ui->penBtn, &QPushButton::toggled, this, &ScmConstellationDialog::togglePen);
	connect(ui->eraserBtn, &QPushButton::toggled, this, &ScmConstellationDialog::toggleEraser);
	connect(ui->undoBtn, &QPushButton::clicked, this, &ScmConstellationDialog::triggerUndo);

	// LABELS TAB
	connect(ui->enNameTE, &QTextEdit::textChanged, this, [this]() 
	{
		constellationEnglishName = ui->enNameTE->toPlainText();
		updateCanBeSavedState();
	});
	connect(ui->natNameTE, &QTextEdit::textChanged, this, [this]() 
	{
		constellationNativeName = ui->natNameTE->toPlainText();
		if (constellationNativeName->isEmpty())
		{
			constellationNativeName = std::nullopt;
		}
	});
	connect(ui->pronounceTE, &QTextEdit::textChanged, this, [this]() 
	{
		constellationPronounce = ui->pronounceTE->toPlainText();
		if (constellationPronounce->isEmpty())
		{
			constellationPronounce = std::nullopt;
		}
	});
	connect(ui->ipaTE, &QTextEdit::textChanged, this, [this]() 
	{
		constellationIPA = ui->ipaTE->toPlainText();
		if (constellationIPA->isEmpty())
		{
			constellationIPA = std::nullopt;
		}
	});
}

void ScmConstellationDialog::togglePen(bool checked)
{
	if (checked)
	{
		ui->eraserBtn->setChecked(false);
		activeTool = Tools::Pen;
	}
	else
	{
		activeTool = Tools::None;
	}
	updateCanBeSavedState();
}

void ScmConstellationDialog::toggleEraser(bool checked)
{
	if (checked)
	{
		ui->penBtn->setChecked(false);
		activeTool = Tools::Eraser;
	}
	else
	{
		activeTool = Tools::None;
	}
	updateCanBeSavedState();
}

void ScmConstellationDialog::triggerUndo()
{
	maker->triggerDrawUndo();
}

void ScmConstellationDialog::updateCanBeSavedState()
{
	bool canBeSaved = true;

	////////////////////////////////////////////////////
	// check criteria here which if not met, will set
	// canBeSaved to false
	////////////////////////////////////////////////////

	// shouldnt happen
	if(nullptr == maker->getCurrentSkyCulture())
	{
		canBeSaved = false;
	}
	if (constellationEnglishName.isEmpty())
	{
		canBeSaved = false;
	}
	// Check if drawnStars is empty
	auto drawnStars = maker->getScmDraw()->getDrawnStars();
	if(std::visit([](const auto& vec) { return vec.empty(); }, drawnStars))
	{
		canBeSaved = false;
	}

	// Update UI here
	ui->saveBtn->setEnabled(canBeSaved);

	// Set final state
	ScmConstellationDialog::canBeSaved = canBeSaved;
}

void ScmConstellationDialog::saveConstellation()
{
	QString id = constellationEnglishName.toLower().replace(" ", "_");
	scm::ListCoordinateStar constellation = maker->getScmDraw()->getDrawnStars();
	maker->getCurrentSkyCulture()->addConstellation(id, constellation);
	scm::ScmConstellation *constellationObj = maker->getCurrentSkyCulture()->getConstellation(id);

	constellationObj->setEnglishName(constellationEnglishName);
	constellationObj->setNativeName(constellationNativeName);
	constellationObj->setPronounce(constellationPronounce);
	constellationObj->setIPA(constellationIPA);
}

void ScmConstellationDialog::resetDialog()
{
	// TODO
	// reset ScmDraw as well?
	// reset labels, artwork etc
}