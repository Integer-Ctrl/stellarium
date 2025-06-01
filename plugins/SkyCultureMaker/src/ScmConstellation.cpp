#include "ScmConstellation.hpp"

scm::ScmConstellation::ScmConstellation(std::vector<scm::CoordinateLine> coordinates, std::vector<scm::StarLine> stars)
	: constellationCoordinates(coordinates)
	, constellationStars(stars)
{
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

void scm::ScmConstellation::setConstellation(std::vector<CoordinateLine> coordinates, std::vector<StarLine> stars)
{
	constellationCoordinates = coordinates;
	constellationStars = stars;
}

void scm::ScmConstellation::drawConstellation(StelCore *core) const
{
	StelPainter painter(core->getProjection(drawFrame));
	painter.setBlending(true);
	painter.setLineSmooth(true);
	Vec3f color = {0.3f, 1.f, 0.f};
	bool alpha = 1.0f;
	painter.setColor(color, alpha);

	for (CoordinateLine p : constellationCoordinates)
	{
		painter.drawGreatCircleArc(p.start, p.end);
	}
}