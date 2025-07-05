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

#ifndef SCMCONVERTDIALOG_HPP
#define SCMCONVERTDIALOG_HPP

#include "StelDialog.hpp"

#ifdef SCM_CONVERTER_ENABLED_CPP

# include "StelFileMgr.hpp"
# include "ui_scmConvertDialog.h"
# include "unarr.h"
# include <QDebug>
# include <QDir>
# include <QFileDialog>
# include <QFileInfo>
# include <QFutureWatcher>
# include <QHBoxLayout>
# include <QLabel>
# include <QLineEdit>
# include <QMimeDatabase>
# include <QPushButton>
# include <QVBoxLayout>
# include <QtConcurrent/QtConcurrent>
# include <atomic>

class Ui_scmConvertDialog;

class ScmConvertDialog : public StelDialog
{
	Q_OBJECT

public:
	explicit ScmConvertDialog();
	~ScmConvertDialog() override;
	void retranslate() override;

protected:
	void createDialogContent() override;

private slots:
	void browseFile();
	void convert();
	void onConversionFinished();
	void closeDialog();

private:
	Ui_scmConvertDialog *ui;
	QFutureWatcher<QString> *watcher;
	QString tempDirPath;
	QString tempDestDirPath;
	std::atomic<bool> conversionCancelled;
};

#endif // SCM_CONVERTER_ENABLED_CPP
#endif // SCMCONVERTDIALOG_HPP
