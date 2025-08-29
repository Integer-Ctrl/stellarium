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

#ifndef SCMSTARTDIALOG_HPP
#define SCMSTARTDIALOG_HPP

#include "SkyCultureMaker.hpp"
#include "StelDialog.hpp"
#include <QObject>

class Ui_scmStartDialog;

class ScmStartDialog : public StelDialog
{
protected:
	void createDialogContent() override;

public:
	ScmStartDialog(SkyCultureMaker *maker);
	~ScmStartDialog() override;

public slots:
	void retranslate() override;

private slots:
	void startScmCreationProcess();
	void closeDialog();

private:
	Ui_scmStartDialog *ui  = nullptr;
	SkyCultureMaker *maker = nullptr;
};

#endif // SCMSTARTDIALOG_HPP
