#ifndef SCM_CONSTELLATION_DIALOG_HPP
#define SCM_CONSTELLATION_DIALOG_HPP

#include <QObject>
#include <QString>
#include <optional>
#include "StelDialogSeparate.hpp"
#include "../SkyCultureMaker.hpp"

class Ui_scmConstellationDialog;

class ScmConstellationDialog : public StelDialogSeparate
{
public:
	enum class Tools
	{
		None,
		Pen,
		Eraser,
	};

protected:
	void createDialogContent() override;

public:
	ScmConstellationDialog(SkyCultureMaker *maker);
	~ScmConstellationDialog() override;

public slots:
	void retranslate() override;

private slots:
	void togglePen(bool checked);
	void toggleEraser(bool checked);
	void triggerUndo();

private:
	/// Specifies whether the current data can be saved as a constellation or not.
	bool canBeSaved = false;

	Ui_scmConstellationDialog *ui;
	SkyCultureMaker *maker;
	Tools activeTool = Tools::None;

	QString constellationEnglishName;
	std::optional<QString> constellationNativeName;
	std::optional<QString> constellationPronounce;
	std::optional<QString> constellationIPA;

	/**
	 * @brief Updates the canBeSaved state based on different criteria.
	 */
	void updateCanBeSavedState();

	void saveConstellation();

	void resetDialog();
};

#endif	// SCM_CONSTELLATION_DIALOG_HPP
