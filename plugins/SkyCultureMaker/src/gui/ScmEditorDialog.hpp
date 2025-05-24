#ifndef SCM_EDITOR_DIALOG_HPP
#define SCM_EDITOR_DIALOG_HPP

#include <QObject>
#include <QString>
#include <optional>
#include "StelDialogSeparate.hpp"
#include "../SkyCultureMaker.hpp"

class Ui_scmEditorDialog;

class ScmEditorDialog : public StelDialogSeparate
{

protected:
	void createDialogContent() override;

public:
	ScmEditorDialog(SkyCultureMaker* maker);
	~ScmEditorDialog() override;

public slots:
	void retranslate() override;

private slots:
	void saveLabels();

private:
	Ui_scmEditorDialog *ui;
	SkyCultureMaker* maker;

	QString constellationEnglishName;
	std::optional<QString> constellationNativeName;
	std::optional<QString> constellationPronounce;
	std::optional<QString> constellationIpa;

	void updateSkyCultureSave(bool saved);
};

#endif	// SCM_EDITOR_DIALOG_HPP
