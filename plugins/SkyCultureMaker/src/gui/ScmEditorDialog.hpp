#ifndef SCM_EDITOR_DIALOG_HPP
#define SCM_EDITOR_DIALOG_HPP

#include <QObject>
#include "StelDialog.hpp"

class Ui_scmEditorDialog;

class ScmEditorDialog : public StelDialog
{

protected:
	void createDialogContent() override;

public:
	ScmEditorDialog();
	~ScmEditorDialog() override;

public slots:
	void retranslate() override;

private:
	Ui_scmEditorDialog *ui;
};

#endif	// SCM_EDITOR_DIALOG_HPP
