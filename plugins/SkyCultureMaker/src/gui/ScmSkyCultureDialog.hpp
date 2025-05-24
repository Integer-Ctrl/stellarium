#ifndef SCM_SKY_CULTURE_DIALOG_HPP
#define SCM_SKY_CULTURE_DIALOG_HPP

#include <QObject>
#include <QString>
#include <optional>
#include "StelDialogSeparate.hpp"
#include "../SkyCultureMaker.hpp"

class Ui_scmSkyCultureDialog;

class ScmSkyCultureDialog : public StelDialogSeparate
{

protected:
	void createDialogContent() override;

public:
	ScmSkyCultureDialog(SkyCultureMaker* maker);
	~ScmSkyCultureDialog() override;

public slots:
	void retranslate() override;

private slots:
	void saveSkyCulture();
	void constellationDialog();
	void removeConstellation();

private:
	Ui_scmSkyCultureDialog *ui;
	SkyCultureMaker* maker;

	QString constellationEnglishName;
	std::vector<QString> constellationList;
	std::optional<QString> constellationNativeName;
	std::optional<QString> constellationPronounce;
	std::optional<QString> constellationIpa;

	void updateSkyCultureSave(bool saved);
};

#endif	// SCM_SKY_CULTURE_DIALOG_HPP
