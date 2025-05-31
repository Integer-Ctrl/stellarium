#include "ScmCommonName.hpp"

scm::ScmCommonName::ScmCommonName(QString id)
{
    ScmCommonName::id = id;
}

void scm::ScmCommonName::setEnglishName(QString name)
{
    englishName = name;
}

void scm::ScmCommonName::setNativeName(QString name)
{
    nativeName = name;
}

void scm::ScmCommonName::setPronounce(QString name)
{
    pronounce = name;
}

void scm::ScmCommonName::setIpa(QString name)
{
    ipa = name;
}

void scm::ScmCommonName::setReferences(std::vector<int> refs)
{
    references = refs;
}

QString scm::ScmCommonName::getEnglishName() const
{
    return englishName;
}

std::optional<QString> scm::ScmCommonName::getIpa() const
{
	return ipa;
}

std::optional<std::vector<int>> scm::ScmCommonName::getReferences() const
{
	return references;
}

QJsonObject scm::ScmCommonName::toJson() const
{
    // TODO implement
}
