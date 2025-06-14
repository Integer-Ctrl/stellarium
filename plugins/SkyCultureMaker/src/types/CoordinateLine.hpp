/**
 * @file CoordinateLine.hpp
 * @author vgerlach, lgrumbach
 * @brief Type describing a line between two coordinates.
 * @version 0.1
 * @date 2025-06-02
 */
#ifndef SCM_TYPES_COORDINATE_LINE_HPP
#define SCM_TYPES_COORDINATE_LINE_HPP

#include "VecMath.hpp"
#include <optional>
#include <QString>

namespace scm
{
//! The pair of start and end coordinate
struct CoordinateLine
{
	//! The start coordinate of the line.
	Vec3d start;

	//! Optional name of the start coordinate, e.g. a star name.
	std::optional<QString> startName;

	//! The end coordinate of the line.
	Vec3d end;

	//! Optional name of the end coordinate, e.g. a star name.
	std::optional<QString> endName;
};

/**
 * @brief Extracts the HIP number from a star name such as "HIP 32349 A" or "HIP 31223".
 * @param hipString The HIP code string.
 * @return The HIP number as an integer, or -1 if the pattern doesn't match.
 */
inline int extractHipNumber(const QString& hipString)
{
	QRegularExpression re(R"(HIP\s+(\d+))");
	QRegularExpressionMatch match = re.match(hipString);
	if (match.hasMatch())
	{
		return match.captured(1).toInt();
	}
	return -1;
}

}  // namespace scm

#endif