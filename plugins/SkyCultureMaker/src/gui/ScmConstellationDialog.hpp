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
	void close() override;

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

	/// Identifier of the constellation
	QString constellationId;
	/// Placeholder identifier of the constellation
	QString constellationPlaceholderId;
	/// English name of the constellation
	QString constellationEnglishName;
	/// Native name of the constellation
	std::optional<QString> constellationNativeName;
	/// Pronounciation of the constellation
	std::optional<QString> constellationPronounce;
	/// IPA representation of the constellation
	std::optional<QString> constellationIPA;

	/**
	 * @brief Updates the canBeSaved state based on different criteria.
	 */
	void updateCanBeSavedState();

	/**
	 * @brief Saves the constellation data as an object in the current sky culture.
	 */
	void saveConstellation();

	/**
	 * @brief Resets the constellation dialog data.
	 */
	void resetDialog();
};

#endif	// SCM_CONSTELLATION_DIALOG_HPP
