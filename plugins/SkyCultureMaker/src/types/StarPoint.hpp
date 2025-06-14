/**
 * @file StarPoint.hpp
 * @author vgerlach, lgrumbach
 * @brief Type describing a single star point.
 * @version 0.1
 * @date 2025-06-02
 */
#ifndef SCM_TYPES_STAR_POINT_HPP
#define SCM_TYPES_STAR_POINT_HPP

#include <optional>
#include <QString>
#include "VecMath.hpp"
#include "ConstellationLine.hpp"

namespace scm
{
//! The point of a single star with coordinates.
struct StarPoint
{
	//! The coordinate of a single point.
	Vec3d coordinate;

	//! The optional star ID at that coordinate.
	QString starId;

	//! Right ascension angle (J2000 frame) in decimal degrees for the star.
	double raJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;

	//! Declination angle (J2000 frame) in decimal degrees for the star.
	double decJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;
};
}  // namespace scm

#endif