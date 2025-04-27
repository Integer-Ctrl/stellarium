#ifndef SKYCULTUREMAKER_HPP
#define SKYCULTUREMAKER_HPP

#include "StelModule.hpp"
#include <QFont>

//! This is an example of a plug-in which can be dynamically loaded into stellarium
class SkyCultureMaker : public StelModule
{
public:
	SkyCultureMaker();
	~SkyCultureMaker() override;

	///////////////////////////////////////////////////////////////////////////
	// Methods defined in the StelModule class
	void init() override;
	// Activate only if update() does something.
	//void update(double deltaTime) override {}
	void draw(StelCore* core) override;
	double getCallOrder(StelModuleActionName actionName) const override;
private:
	// Font used for displaying our text
	QFont font;
};


#include <QObject>
#include "StelPluginInterface.hpp"

//! This class is used by Qt to manage a plug-in interface
class SkyCultureMakerStelPluginInterface : public QObject, public StelPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID StelPluginInterface_iid)
	Q_INTERFACES(StelPluginInterface)
public:
	StelModule* getStelModule() const override;
	StelPluginInfo getPluginInfo() const override;
	QObjectList getExtensionList() const override { return QObjectList(); }
};

#endif /* SKYCULTUREMAKER_HPP */
