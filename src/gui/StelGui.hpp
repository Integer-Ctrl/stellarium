/*
 * Stellarium
 * Copyright (C) 2008 Fabien Chereau
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#ifndef STELGUI_HPP
#define STELGUI_HPP

#ifndef NO_GUI

#include "StelModule.hpp"
#include "StelObject.hpp"
#include "StelGuiBase.hpp"
#include "StelStyle.hpp"

#include <QGraphicsTextItem>

class QGraphicsSceneMouseEvent;
class QTimeLine;
class StelButton;
class BottomStelBar;
class InfoPanel;
class ConfigurationDialog;
class DateTimeDialog;
class HelpDialog;
class LocationDialog;
class SearchDialog;
class ViewDialog;
class ShortcutsDialog;
class AstroCalcDialog;
class ObsListDialog;
#ifdef ENABLE_SCRIPT_CONSOLE
class ScriptConsole;
#endif

//! @class StelGui
//! Main class for the GUI based on QGraphicView.
//! It manages the various qt configuration windows, the buttons bars, the list of shortcuts.
class StelGui : public QObject, public StelGuiBase
{
	Q_OBJECT
	Q_PROPERTY(bool visible READ getVisible WRITE setVisible NOTIFY visibleChanged)
	Q_PROPERTY(bool autoHideHorizontalButtonBar READ getAutoHideHorizontalButtonBar WRITE setAutoHideHorizontalButtonBar NOTIFY autoHideHorizontalButtonBarChanged)
	Q_PROPERTY(bool autoHideVerticalButtonBar READ getAutoHideVerticalButtonBar WRITE setAutoHideVerticalButtonBar NOTIFY autoHideVerticalButtonBarChanged)
	Q_PROPERTY(bool flagShowQuitButton READ getFlagShowQuitButton WRITE setFlagShowQuitButton NOTIFY  flagShowQuitButtonChanged)
	Q_PROPERTY(bool flagShowFlipButtons READ getFlagShowFlipButtons WRITE setFlagShowFlipButtons NOTIFY flagShowFlipButtonsChanged)
	Q_PROPERTY(bool flagShowNebulaBackgroundButton READ getFlagShowNebulaBackgroundButton WRITE setFlagShowNebulaBackgroundButton NOTIFY flagShowNebulaBackgroundButtonChanged)
	Q_PROPERTY(bool flagShowDSSButton READ getFlagShowDSSButton WRITE setFlagShowDSSButton NOTIFY  flagShowDSSButtonChanged)
	Q_PROPERTY(bool flagShowHiPSButton READ getFlagShowHiPSButton WRITE setFlagShowHiPSButton NOTIFY  flagShowHiPSButtonChanged)
	Q_PROPERTY(bool flagShowGotoSelectedObjectButton READ getFlagShowGotoSelectedObjectButton WRITE setFlagShowGotoSelectedObjectButton NOTIFY flagShowGotoSelectedObjectButtonChanged)
	Q_PROPERTY(bool flagShowNightmodeButton READ getFlagShowNightmodeButton WRITE setFlagShowNightmodeButton NOTIFY  flagShowNightmodeButtonChanged)
	Q_PROPERTY(bool flagShowFullscreenButton READ getFlagShowFullscreenButton WRITE setFlagShowFullscreenButton NOTIFY  flagShowFullscreenButtonChanged)
	Q_PROPERTY(bool flagShowObsListButton READ getFlagShowObsListButton WRITE setFlagShowObsListButton NOTIFY flagShowObsListButtonChanged)
	Q_PROPERTY(bool flagShowICRSGridButton READ getFlagShowICRSGridButton WRITE setFlagShowICRSGridButton NOTIFY flagShowICRSGridButtonChanged)
	Q_PROPERTY(bool flagShowGalacticGridButton READ getFlagShowGalacticGridButton WRITE setFlagShowGalacticGridButton NOTIFY flagShowGalacticGridButtonChanged )
	Q_PROPERTY(bool flagShowEclipticGridButton READ getFlagShowEclipticGridButton WRITE setFlagShowEclipticGridButton NOTIFY flagShowEclipticGridButtonChanged )
	Q_PROPERTY(bool flagShowConstellationBoundariesButton READ getFlagShowConstellationBoundariesButton WRITE setFlagShowConstellationBoundariesButton NOTIFY flagShowConstellationBoundariesButtonChanged )
	Q_PROPERTY(bool flagShowConstellationArtsButton READ getFlagShowConstellationArtsButton WRITE setFlagShowConstellationArtsButton NOTIFY flagShowConstellationArtsButtonChanged )
	Q_PROPERTY(bool flagShowAsterismLinesButton READ getFlagShowAsterismLinesButton WRITE setFlagShowAsterismLinesButton NOTIFY flagShowAsterismLinesButtonChanged )
	Q_PROPERTY(bool flagShowAsterismLabelsButton READ getFlagShowAsterismLabelsButton WRITE setFlagShowAsterismLabelsButton NOTIFY flagShowAsterismLabelsButtonChanged )
	Q_PROPERTY(bool flagUseButtonsBackground   READ getFlagUseButtonsBackground   WRITE setFlagUseButtonsBackground   NOTIFY flagUseButtonsBackgroundChanged)
	Q_PROPERTY(bool flagUseKineticScrolling READ getFlagUseKineticScrolling WRITE setFlagUseKineticScrolling NOTIFY flagUseKineticScrollingChanged)
	Q_PROPERTY(bool flagEnableFocusOnDaySpinner READ getFlagEnableFocusOnDaySpinner WRITE setFlagEnableFocusOnDaySpinner NOTIFY flagEnableFocusOnDaySpinnerChanged)
	Q_PROPERTY(bool flagShowCardinalButton READ getFlagShowCardinalButton WRITE setFlagShowCardinalButton NOTIFY  flagShowCardinalButtonChanged)
	Q_PROPERTY(bool flagShowCompassButton READ getFlagShowCompassButton WRITE setFlagShowCompassButton NOTIFY  flagShowCompassButtonChanged)

public:
	friend class ViewDialog;
	
	StelGui();
	~StelGui() override;

	///////////////////////////////////////////////////////////////////////////
	// Methods defined in the StelModule class
	//! Initialize the StelGui object.
	void init(QGraphicsWidget* topLevelGraphicsWidget) override;
	void update();

	StelStyle getStelStyle() const {return currentStelStyle;}
	
	///////////////////////////////////////////////////////////////////////////
	// Methods specific to the StelGui class
	// ! Load a Qt style sheet to define the widgets style
	//void loadStyle(const QString& fileName);
	
	//! Get the button bar at the bottom of the screen
	BottomStelBar* getButtonBar() const;
	
	//! Get the button bar of the left of the screen
	class LeftStelBar* getWindowsButtonBar() const;

	//! Get the SkyGui instance (useful for adding other interface elements).
	//! It will return a valid object only if called after init().
	class SkyGui* getSkyGui() const;

	//! returns true if the gui has completed init process.
	bool initComplete(void) const;

#ifdef ENABLE_SCRIPT_CONSOLE
	ScriptConsole* getScriptConsole() const {return scriptConsole;}
#endif

	//! Used to force a refreshing of the GUI elements such as the button bars.
	void forceRefreshGui() override;
	
	void setVisible(bool b) override;

	bool getVisible() const override;

	bool getAstroCalcVisible() const;

	bool isCurrentlyUsed() const override;
	
	void setInfoTextFilters(const StelObject::InfoStringGroup& aflags) override;
	const StelObject::InfoStringGroup& getInfoTextFilters() const override;

public slots:
	//! Define whether the buttons toggling image flip should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowFlipButtons(bool b);

	//! Define whether the button toggling nebulae background should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowNebulaBackgroundButton(bool b);

	//! Define whether the button toggling observing list should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowObsListButton(bool b);

	//! Define whether the button toggling ICRS grid should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowICRSGridButton(bool b);

	//! Define whether the button toggling galactic grid should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowGalacticGridButton(bool b);

	//! Define whether the button toggling ecliptic grid should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowEclipticGridButton(bool b);

	//! Define whether the button toggling constellation boundaries should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowConstellationBoundariesButton(bool b);

	//! Define whether the button toggling constellation arts should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowConstellationArtsButton(bool b);

	//! Define whether the button toggling asterism lines should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowAsterismLinesButton(bool b);

	//! Define whether the button toggling asterism labels should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowAsterismLabelsButton(bool b);

	//! Define whether the button toggling DSS survey (TOAST) should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowDSSButton(bool b);

	//! Define whether the button toggling HiPS surveys should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowHiPSButton(bool b);

	//! Define whether the button for centering on selected object should be visible. If not, user can still press SPACE to center.
	//! This immediately also stores flag to config.ini
	void setFlagShowGotoSelectedObjectButton(bool b);

	//! Define whether the button toggling nightmode should be visible in addition to the Ctrl+N hotkey.
	//! This immediately also stores flag to config.ini
	void setFlagShowNightmodeButton(bool b);

	//! Define whether the button toggling fullscreen view should be visible in addition to F11 hotkey.
	//! This immediately also stores flag to config.ini
	void setFlagShowFullscreenButton(bool b);

	//! Define whether the button for exit should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowQuitButton(bool b);

	//! Define whether the button toggling cardinal should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowCardinalButton(bool b);

	//! Define whether the button toggling compass marks should be visible
	//! This immediately also stores flag to config.ini
	void setFlagShowCompassButton(bool b);

	//! Set the state of the flag of usage background for GUI buttons
	//! This immediately also stores flag to config.ini
	void setFlagUseButtonsBackground(bool b);

	//! Set the state of the flag for kinetic scrolling
	//! This immediately also stores flag to config.ini
	void setFlagUseKineticScrolling(bool b);

	//! Set the state of the flag for enable focus on day spinner in Date and Time dialog
	//! This immediately also stores flag to config.ini
	void setFlagEnableFocusOnDaySpinner(bool b);

	//! Set the auto-hide status of the horizontal toolbar.
	//! When set to true, the horizontal toolbar will auto-hide itself, only
	//! making an appearance when the mouse is nearby.  When false, it will
	//! remain on screen.
	//! @param b to hide or not to hide
	void setAutoHideHorizontalButtonBar(bool b);
	
	//! Set the auto-hide status of the vertical toolbar.
	//! When set to true, the vertical toolbar will auto-hide itself, only
	//! making an appearance when the mouse is nearby.  When false, it will
	//! remain on screen.
	//! @param b to hide or not to hide
	void setAutoHideVerticalButtonBar(bool b);

	//! Hide or show the GUI.  Public so it can be called from scripts.
	void setGuiVisible(bool);

#ifdef ENABLE_SCRIPTING
	//! change keys when a script is running / not running
	void setScriptKeys(bool b);
	void increaseScriptSpeed();
	void decreaseScriptSpeed();
	void setRealScriptSpeed();
	void stopScript();
	#ifndef ENABLE_SCRIPT_QML
	void pauseScript();
	void resumeScript();
	#endif
#endif

	void displayAllInfo();
	void displayDefaultInfo();
	void displayShortInfo();
	void displayNoneInfo();
	void displayCustomInfo();

    public:
	// Getters
	//! Get the state of the flag of usage background for GUI buttons
	bool getFlagUseButtonsBackground() const { return flagUseButtonsBackground; }
	//! Get the state of the flag for kinetic scrolling
	bool getFlagUseKineticScrolling() const { return flagUseKineticScrolling; }
	//! Get the state of the flag for enable focus on day spinner in Date and Time dialog
	bool getFlagEnableFocusOnDaySpinner() const { return flagEnableFocusOnDaySpinner; }
	//! Get whether the exit button is visible
	bool getFlagShowQuitButton() const;
	//! Get whether the buttons toggling image flip are visible
	bool getFlagShowFlipButtons() const;
	//! Get whether the button toggling nebulae background is visible
	bool getFlagShowNebulaBackgroundButton() const;
	//! Get whether the button toggling DSS survey (TOAST) is visible
	bool getFlagShowDSSButton() const;
	//! Get whether the button toggling cardinal is visible
	bool getFlagShowCardinalButton() const;
	//! Get whether the button toggling compass marks is visible
	bool getFlagShowCompassButton() const;
	//! Get whether the button toggling HiPS surveys is visible
	bool getFlagShowHiPSButton() const;
	//! Get whether the button for centering on selected object is visible
	bool getFlagShowGotoSelectedObjectButton() const;
	//! Get whether the button toggling nightmode is visible
	bool getFlagShowNightmodeButton() const;
	//! Get whether the button toggling fullscreen view is visible
	bool getFlagShowFullscreenButton() const;
	//! Get whether the button toggling observing list is visible
	bool getFlagShowObsListButton() const;
	//! Get whether the button toggling ICRS grid is visible
	bool getFlagShowICRSGridButton() const;
	//! Get whether the button toggling galactic grid is visible
	bool getFlagShowGalacticGridButton() const;
	//! Get whether the button toggling ecliptic grid is visible
	bool getFlagShowEclipticGridButton() const;
	//! Get whether the button toggling constellation boundaries is visible
	bool getFlagShowConstellationBoundariesButton() const;
	//! Get whether the button toggling constellation arts is visible
	bool getFlagShowConstellationArtsButton() const;
	//! Get whether the button toggling asterism lines is visible
	bool getFlagShowAsterismLinesButton() const;
	//! Get whether the button toggling asterism labels is visible
	bool getFlagShowAsterismLabelsButton() const;
	//! Get the auto-hide status of the horizontal toolbar.
	bool getAutoHideHorizontalButtonBar() const;
	//! Get the auto-hide status of the vertical toolbar.
	bool getAutoHideVerticalButtonBar() const;

signals:
	void visibleChanged(bool b);
	void autoHideHorizontalButtonBarChanged(bool b);
	void autoHideVerticalButtonBarChanged(bool b);
	void flagUseButtonsBackgroundChanged(bool b);
	void flagUseKineticScrollingChanged(bool b);
	void flagEnableFocusOnDaySpinnerChanged(bool b);
	void flagShowQuitButtonChanged(bool b);
	void flagShowFlipButtonsChanged(bool b);
	void flagShowNebulaBackgroundButtonChanged(bool b);
	void flagShowDSSButtonChanged(bool b);
	void flagShowHiPSButtonChanged(bool b);
	void flagShowGotoSelectedObjectButtonChanged(bool b);
	void flagShowNightmodeButtonChanged(bool b);
	void flagShowFullscreenButtonChanged(bool b);
	void flagShowObsListButtonChanged(bool b);
	void flagShowICRSGridButtonChanged(bool b);
	void flagShowGalacticGridButtonChanged(bool b);
	void flagShowEclipticGridButtonChanged(bool b);
	void flagShowConstellationBoundariesButtonChanged(bool b);
	void flagShowConstellationArtsButtonChanged(bool b);
	void flagShowAsterismLinesButtonChanged(bool b);
	void flagShowAsterismLabelsButtonChanged(bool b);
	void flagShowCardinalButtonChanged(bool b);
	void flagShowCompassButtonChanged(bool b);
	void infoStringChanged();
	void guiStyleChanged(const QString &style);
	void htmlStyleChanged(const QString &style);

private slots:
	void reloadStyle();
#ifdef ENABLE_SCRIPTING
	void scriptStarted();
	void scriptStopped();
#endif
	//! Load color scheme from the given ini file and section name
	void setStelStyle(const QString& style) override final;
	void quit();	
	void updateI18n();
	void copySelectedObjectInfo(void);

private:
	void updateStelStyle();
	//! convenience method to find an action in the StelActionMgr.
	StelAction* getAction(const QString& actionName) const;

	void addButtonOnBottomBar(QString buttonName, QString actionName, QString groupName);
	void addButtonOnLeftBar(QString buttonName, QString actionName);

	QGraphicsWidget* topLevelGraphicsWidget;

	class SkyGui* skyGui;

	bool flagUseButtonsBackground;
	bool flagUseKineticScrolling;
	bool flagEnableFocusOnDaySpinner;

	StelButton* buttonTimeRewind;
	StelButton* buttonTimeRealTimeSpeed;
	StelButton* buttonTimeCurrent;
	StelButton* buttonTimeForward;

	bool flagShowQuitButton;
	StelButton* buttonQuit;

	bool flagShowGotoSelectedObjectButton;
	StelButton* buttonGotoSelectedObject;

	LocationDialog* locationDialog;
	HelpDialog* helpDialog;
	DateTimeDialog* dateTimeDialog;
	SearchDialog* searchDialog;
	ViewDialog* viewDialog;
	ShortcutsDialog* shortcutsDialog;
	ConfigurationDialog* configurationDialog;
#ifdef ENABLE_SCRIPT_CONSOLE
	ScriptConsole* scriptConsole;
#endif
	AstroCalcDialog* astroCalcDialog;
	ObsListDialog* obsListDialog;

	bool flagShowFlipButtons;
	StelButton* flipVert;
	StelButton* flipHoriz;

	bool flagShowNebulaBackgroundButton;
	StelButton* btShowNebulaeBackground;

	bool flagShowDSSButton;
	StelButton* btShowDSS;

	bool flagShowHiPSButton;
	StelButton* btShowHiPS;

	bool flagShowNightmodeButton;
	StelButton* buttonNightmode;

	bool flagShowFullscreenButton;
	StelButton* buttonFullscreen;

	bool flagShowObsListButton;
	StelButton* btShowObsList;

	bool flagShowICRSGridButton;
	StelButton* btShowICRSGrid;

	bool flagShowGalacticGridButton;
	StelButton* btShowGalacticGrid;

	bool flagShowEclipticGridButton;
	StelButton* btShowEclipticGrid;

	bool flagShowConstellationBoundariesButton;
	StelButton* btShowConstellationBoundaries;

	bool flagShowConstellationArtsButton;
	StelButton* btShowConstellationArts;

	bool flagShowAsterismLinesButton;
	StelButton* btShowAsterismLines;

	bool flagShowAsterismLabelsButton;
	StelButton* btShowAsterismLabels;

	bool flagShowCardinalButton;
	StelButton* btShowCardinal;

	bool flagShowCompassButton;
	StelButton* btShowCompass;

	bool initDone;

	QSizeF savedProgressBarSize;

	// Currently used StelStyle
	StelStyle currentStelStyle;

#ifdef ENABLE_SCRIPTING
	// We use a QStringList to save the user-configured buttons while script is running, and restore them later.
	QStringList scriptSaveSpeedbuttons;
#endif
};

#else // NO_GUI

#include "StelGuiBase.hpp"
#include <QProgressBar>

class StelGui : public StelGuiBase
{
public:
	StelGui(): dummyInfoTextFilter(StelObject::InfoStringGroup::None), visible(false) {;}
	~StelGui() {;}
	virtual void init(QGraphicsWidget* topLevelGraphicsWidget, class StelAppGraphicsWidget* stelAppGraphicsWidget) {;}
	virtual void updateI18n() {;}
	virtual void setStelStyle(const QString& section) {;}
	virtual void setInfoTextFilters(const StelObject::InfoStringGroup& aflags) {dummyInfoTextFilter=aflags;}
	virtual const StelObject::InfoStringGroup& getInfoTextFilters() const {return dummyInfoTextFilter;}
	virtual QProgressBar* addProgressBar() {return new QProgressBar;}
	virtual QAction* addGuiActions(const QString& actionName, const QString& text, const QString& shortCut, const QString& helpGroup, bool checkable=true, bool autoRepeat=false) {return nullptr;}
	virtual void forceRefreshGui() {;}
	virtual void setVisible(bool b) {visible=b;}
	virtual bool getVisible() const {return visible;}
	virtual bool isCurrentlyUsed() const {return false;}
private:
	StelObject::InfoStringGroup dummyInfoTextFilter;
	bool visible;
};

#endif

#endif // STELGUI_HPP
