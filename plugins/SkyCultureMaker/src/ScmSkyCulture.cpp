#include "ScmSkyCulture.hpp"

void scm::ScmSkyCulture::setId(QString id)
{
	ScmSkyCulture::id = id;
}

void scm::ScmSkyCulture::setRegion(QString region)
{
	ScmSkyCulture::region = region;
}

void scm::ScmSkyCulture::setClassification(StelSkyCulture::CLASSIFICATION classification)
{
	ScmSkyCulture::classification = classification;
}

void scm::ScmSkyCulture::setFallbackToInternationalNames(bool fallback)
{
	ScmSkyCulture::fallbackToInternationalNames = fallback;
}

void scm::ScmSkyCulture::addAsterism(scm::ScmAsterism asterism)
{
	asterisms.push_back(asterism);
}

void scm::ScmSkyCulture::removeAsterism(QString id)
{
	asterisms.erase(
	    remove_if(begin(asterisms), end(asterisms), [id](scm::ScmAsterism const &a) { return a.getId() == id; }),
	    end(asterisms));
}

void scm::ScmSkyCulture::addConstellation(scm::ScmConstellation constellation)
{
	constellations.push_back(constellation);
}

void scm::ScmSkyCulture::removeConstellation(QString id)
{
	constellations.erase(remove_if(begin(constellations),
				       end(constellations),
				       [id](ScmConstellation const &c) { return c.getId() == id; }),
			     end(constellations));
}