#include "ScmConstellation.hpp"

scm::ScmConstellation::ScmConstellation(std::vector<scm::ConstellationLine> constellationLines)
	: constellationLines(constellationLines)
{
	QSettings* conf = StelApp::getInstance().getSettings();
	constellationLabelFont.setPixelSize(conf->value("viewing/constellation_font_size", 15).toInt());
 
	QString defaultColor = conf->value("color/default_color", "0.5,0.5,0.7").toString();
	colorDrawDefault = Vec3f(conf->value("color/const_lines_color", defaultColor).toString());
	colorLabelDefault = Vec3f(conf->value("color/const_names_color", defaultColor).toString());
}

void scm::ScmConstellation::setId(QString id)
{
	ScmConstellation::id = id;
}

QString scm::ScmConstellation::getId() const
{
	return id;
}

void scm::ScmConstellation::setEnglishName(QString name)
{
	englishName = name;
}

QString scm::ScmConstellation::getEnglishName() const
{
	return englishName;
}

void scm::ScmConstellation::setNativeName(std::optional<QString> name)
{
	nativeName = name;
}

void scm::ScmConstellation::setPronounce(std::optional<QString> pronounce)
{
	ScmConstellation::pronounce = pronounce;
}

void scm::ScmConstellation::setIPA(std::optional<QString> ipa)
{
	ScmConstellation::ipa = ipa;
}

void scm::ScmConstellation::setConstellation(std::vector<ConstellationLine> constellationLines)
{
	ScmConstellation::constellationLines = constellationLines;
}

void scm::ScmConstellation::drawConstellation(StelCore *core, Vec3f color)
{
	StelPainter painter(core->getProjection(drawFrame));
	painter.setBlending(true);
	painter.setLineSmooth(true);
	painter.setFont(constellationLabelFont);
	
	bool alpha = 1.0f;
	painter.setColor(color, alpha);

	XYZname.set(0.,0.,0.);
	for (ConstellationLine p : constellationLines)
	{
		painter.drawGreatCircleArc(p.startCoordinate, p.endCoordinate);
		XYZname += p.endCoordinate;
		XYZname += p.startCoordinate;
	}

	XYZname.normalize();

	drawNames(core, painter, colorLabelDefault);
}

void scm::ScmConstellation::drawConstellation(StelCore *core)
{
	drawConstellation(core, colorDrawDefault);
}

void scm::ScmConstellation::drawNames(StelCore *core, StelPainter sPainter, Vec3f labelColor) 
{
	sPainter.setBlending(true);

	Vec3d velocityObserver(0.);
	if (core->getUseAberration())
	{
		velocityObserver = core->getAberrationVec(core->getJDE());
	}

	XYZname+=velocityObserver;
	XYZname.normalize();

	if(!sPainter.getProjector()->projectCheck(XYZname, this->XYname))
	{
		return;
	}

	sPainter.getProjector()->project(XYZname, XYname);
	sPainter.setColor(labelColor, 1.0f);
	sPainter.drawText(static_cast<float>(XYname[0]), static_cast<float>(XYname[1]), englishName, 0., -sPainter.getFontMetrics().boundingRect(englishName).width()/2, 0, false);
}

void scm::ScmConstellation::drawNames(StelCore *core, StelPainter sPainter)
{
	drawNames(core, sPainter, colorLabelDefault);
}

QJsonObject scm::ScmConstellation::toJson(QString &skyCultureName) const
{
	QJsonObject json;

	// Assemble lines object
	QJsonArray linesArray;
	// A constellation can not have both names and coordinates as lines
	bool containsName = false;
	bool containsCoordinates = false;
	for (const auto& line : constellationLines)
	{
		QJsonArray lineJson = line.toJson();
		if (!lineJson.isEmpty())
		{
			linesArray.append(lineJson);
			// check whether the line contains coordinates or a name
			if(lineJson[0].isArray() || lineJson[1].isArray())
			{
				containsCoordinates = true;
			}
			else
			{
				containsName = true;
			}
		}
		else
		{
			// TODO: invalid line
			qWarning() << "Invalid constellation line found";
		}
	}

	if(containsName && containsCoordinates)
	{
		qWarning() << "Invalid constellation: A constellation can not have both names and coordinates as lines.";
		return QJsonObject(); // Invalid constellation, return empty JSON object
	}

	json["id"] = "CON " + skyCultureName + " " + id;
	json["lines"] = linesArray;

	// Assemble common name object
	QJsonObject commonNameObj;
	commonNameObj["english"] = englishName;
	if (nativeName.has_value())
	{
		commonNameObj["native"] = nativeName.value();
	}
	if (pronounce.has_value())
	{
		commonNameObj["pronounce"] = pronounce.value();
	}
	if (ipa.has_value())
	{
		commonNameObj["ipa"] = ipa.value();
	}
	if (references.has_value() && !references->isEmpty())
	{
		QJsonArray refsArray;
		for (const auto& ref : references.value())
		{
			refsArray.append(ref);
		}
		commonNameObj["references"] = refsArray;
	}
	json["common_name"] = commonNameObj;

	return json;
}