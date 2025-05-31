#include "ScmConstellation.hpp"

scm::ScmConstellation::ScmConstellation(ScmDraw::ListCoordinateStar constellation)
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
