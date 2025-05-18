#ifndef SCM_EDITOR_DIALOG_HPP
#define SCM_EDITOR_DIALOG_HPP

#include <QObject>
#include <QString>
#include <optional>
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

private slots:
	void saveLabels();

private:
	Ui_scmEditorDialog *ui;

	QString constellationEnglishName;
	std::optional<QString> constellationNativeName;
	std::optional<QString> constellationPronounce;
	std::optional<QString> constellationIpa;

	void updateLabelsSavedLabel(bool saved);
};

#endif	// SCM_EDITOR_DIALOG_HPP
