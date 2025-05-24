#ifndef SCMSTARTDIALOG_HPP
#define SCMSTARTDIALOG_HPP

#include <QObject>
#include "StelDialog.hpp"

class Ui_scmStartDialog;

class ScmStartDialog : public StelDialog
{

protected:
	void createDialogContent() override;

public:
	ScmStartDialog();
	~ScmStartDialog() override;

public slots:
	void retranslate() override;

private:
	Ui_scmStartDialog* ui;
};

#endif // SCMSTARTDIALOG_HPP
