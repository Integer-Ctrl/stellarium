#include "ScmConstellation.hpp"

scm::ScmConstellation::ScmConstellation(scm::ListCoordinateStar constellation)
: constellation(constellation)
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

void scm::ScmConstellation::setConstellation(scm::ListCoordinateStar constellation)
{
    ScmConstellation::constellation = constellation;
}
