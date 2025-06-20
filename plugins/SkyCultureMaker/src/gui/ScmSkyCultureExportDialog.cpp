#include "ScmSkyCultureExportDialog.hpp"
#include "ui_scmSkyCultureExportDialog.h"

ScmSkyCultureExportDialog::ScmSkyCultureExportDialog(SkyCultureMaker *maker)
	: StelDialogSeparate("ScmSkyCultureExportDialog")
	, maker(maker)
{
	assert(maker != nullptr);
	ui = new Ui_scmSkyCultureExportDialog;
}

ScmSkyCultureExportDialog::~ScmSkyCultureExportDialog()
{
	if (ui != nullptr)
	{
		delete ui;
	}
}

void ScmSkyCultureExportDialog::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void ScmSkyCultureExportDialog::createDialogContent()
{
	ui->setupUi(dialog);

	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));
	connect(ui->titleBar, SIGNAL(movedTo(QPoint)), this, SLOT(handleMovedTo(QPoint)));
	connect(ui->titleBar, &TitleBar::closeClicked, this, &ScmSkyCultureExportDialog::close);
	connect(ui->exportBtn, &QPushButton::clicked, this, &ScmSkyCultureExportDialog::exportSkyCulture);
	connect(ui->cancelBtn, &QPushButton::clicked, this, &ScmSkyCultureExportDialog::closeDialog);
}

void ScmSkyCultureExportDialog::closeDialog()
{
	StelDialog::close();
}

void ScmSkyCultureExportDialog::exportSkyCulture()
{
	maker->saveSkyCultureDescription();

	// // only for debugging purposes
	// if (constellations != nullptr)
	// {
	// 	qDebug() << "[Constellations as JSON]:";
	// 	for (const auto &constellation : *constellations)
	// 	{
	// 		QJsonObject obj = constellation.toJson(name);
	// 		QJsonDocument doc(obj);
	// 		qDebug().noquote() << doc.toJson(QJsonDocument::Compact);
	// 	}
	// }
	bool success = maker->saveSkyCultureDescription();
}
