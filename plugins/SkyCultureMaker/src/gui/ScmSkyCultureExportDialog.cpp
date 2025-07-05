/*
 * Sky Culture Maker plug-in for Stellarium
 *
 * Copyright (C) 2025 Vincent Gerlach
 * Copyright (C) 2025 Luca-Philipp Grumbach
 * Copyright (C) 2025 Fabian Hofer
 * Copyright (C) 2025 Mher Mnatsakanyan
 * Copyright (C) 2025 Richard Hofmann
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScmSkyCultureExportDialog.hpp"
#include "QDir"
#include "ScmSkyCulture.hpp"
#include "ui_scmSkyCultureExportDialog.h"
#include <QJsonDocument>
#include <QJsonObject>

ScmSkyCultureExportDialog::ScmSkyCultureExportDialog(SkyCultureMaker* maker)
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
	connect(ui->saveBtn, &QPushButton::clicked, this, &ScmSkyCultureExportDialog::saveSkyCulture);
	connect(ui->saveAndExitBtn, &QPushButton::clicked, this, &ScmSkyCultureExportDialog::saveAndExitSkyCulture);
	connect(ui->cancelBtn, &QPushButton::clicked, this, &ScmSkyCultureExportDialog::close);
}

void ScmSkyCultureExportDialog::saveSkyCulture()
{
	if (maker == nullptr)
	{
		qWarning() << "SkyCultureMaker: maker is nullptr. Cannot export sky culture.";
		ScmSkyCultureExportDialog::close();
		return;
	}

	scm::ScmSkyCulture* currentSkyCulture = maker->getCurrentSkyCulture();
	if (currentSkyCulture == nullptr)
	{
		qWarning() << "SkyCultureMaker: current sky culture is nullptr. Cannot export.";
		maker->setSkyCultureDialogInfoLabel("ERROR: No sky culture is set.");
		ScmSkyCultureExportDialog::close();
		return;
	}

	QString export_directory = currentSkyCulture->getId();

	// save illustrations before json, because the relative illustrations path is required for the json export
	bool savedIllustrationsSuccessfully = currentSkyCulture->saveIllustrations(export_directory +
	                                                                           QDir::separator() + "illustrations");
	if (!savedIllustrationsSuccessfully)
	{
		maker->setSkyCultureDialogInfoLabel("WARNING: Failed to save the illustrations.");
		qWarning() << "SkyCultureMaker: Failed to export sky culture illustrations.";
		return;
	}

	// TODO: Export sky culture as json file (#88)
	qDebug() << "Exporting sky culture...";
	QJsonObject scJsonObject = currentSkyCulture->toJson();
	QJsonDocument scJsonDoc(scJsonObject);
	qDebug().noquote() << scJsonDoc.toJson(QJsonDocument::Compact);
	// TODO: the error handling here should be improved once we also have to
	// check whether the json file was successfully saved (#88)

	bool savedDescriptionSuccessfully = maker->saveSkyCultureDescription();

	if (!savedDescriptionSuccessfully)
	{
		maker->setSkyCultureDialogInfoLabel("WARNING: Failed to export sky culture description.");
		qWarning() << "SkyCultureMaker: Failed to export sky culture description.";
	}

	maker->setSkyCultureDialogInfoLabel("Sky culture exported successfully!");

	ScmSkyCultureExportDialog::close();
}

void ScmSkyCultureExportDialog::saveAndExitSkyCulture()
{
	saveSkyCulture();
	maker->resetScmDialogs();
	maker->hideAllDialogs();
	maker->setIsScmEnabled(false);
}
