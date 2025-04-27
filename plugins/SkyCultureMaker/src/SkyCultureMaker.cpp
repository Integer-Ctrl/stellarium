#include "StelProjector.hpp"
#include "StelPainter.hpp"
#include "StelApp.hpp"
#include "StelCore.hpp"
#include "StelLocaleMgr.hpp"
#include "StelModuleMgr.hpp"
#include "SkyCultureMaker.hpp"

#include <QDebug>

/*************************************************************************
 This method is the one called automatically by the StelModuleMgr just 
 after loading the dynamic library
*************************************************************************/
StelModule* SkyCultureMakerStelPluginInterface::getStelModule() const
{
	return new SkyCultureMaker();
}

StelPluginInfo SkyCultureMakerStelPluginInterface::getPluginInfo() const
{
	StelPluginInfo info;
	info.id = "SkyCultureMaker";
	info.displayedName = "Sky Culture Maker";
	info.authors = "info.authors";
	info.contact = "info.contact";
	info.description = "info.description";
	info.version = SKYCULTUREMAKER_PLUGIN_VERSION;
	info.license = SKYCULTUREMAKER_PLUGIN_LICENSE;
	return info;
}

/*************************************************************************
 Constructor
*************************************************************************/
SkyCultureMaker::SkyCultureMaker()
{
	setObjectName("SkyCultureMaker");
	font.setPixelSize(25);
}

/*************************************************************************
 Destructor
*************************************************************************/
SkyCultureMaker::~SkyCultureMaker()
{
}

/*************************************************************************
 Reimplementation of the getCallOrder method
*************************************************************************/
double SkyCultureMaker::getCallOrder(StelModuleActionName actionName) const
{
	if (actionName==StelModule::ActionDraw)
		return StelApp::getInstance().getModuleMgr().getModule("NebulaMgr")->getCallOrder(actionName)+10.;
	return 0;
}


/*************************************************************************
 Init our module
*************************************************************************/
void SkyCultureMaker::init()
{
	qDebug() << "init called for SkyCultureMaker";
}

/*************************************************************************
 Draw our module. This should print "Hello world!" in the main window
*************************************************************************/
void SkyCultureMaker::draw(StelCore* core)
{
	StelPainter painter(core->getProjection2d());
	painter.setColor(1,1,1,1);
	painter.setFont(font);
	painter.drawText(300, 300, "Hello World!");
}

