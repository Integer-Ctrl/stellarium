#ifndef STARTDIALOG_HPP
#define STARTDIALOG_HPP

#include <QObject>
#include "StelDialog.hpp"

class Ui_startDialog;

class StartDialog : public StelDialog
{

protected:
	void createDialogContent() override;

public:
	StartDialog();
	~StartDialog() override;

public slots:
	void retranslate() override;

private:
	Ui_startDialog* ui;
};

#endif // STARTDIALOG_HPP
