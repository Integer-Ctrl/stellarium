#ifndef SCM_TYPES_HPP
#define SCM_TYPES_HPP

#include <variant>
#include <vector>
#include <QString>
#include "StelCore.hpp"

namespace scm
{
//! @brief The pair of start and end coordinate
struct CoordinateLine
{
	//! @brief The start coordinate of the line.
	Vec3d start;

	//! @brief The end coordinate of the line.
	Vec3d end;
};

//! @brief The pair of optional start and end stars
struct StarLine
{
	//! @brief The start star of the line.
	std::optional<QString> start;

	//! @brief The end star of the line.
	std::optional<QString> end;
};

//! @brief The lines of the current drawn constellation
struct Lines
{
	//! @brief The coordinate pairs of a line.
	std::vector<CoordinateLine> coordinates;

	//! @brief The optional available stars too the coordinates.
	std::vector<StarLine> stars;
};

//! @brief The point of a single star with coordinates.
struct StarPoint
{
	//! @brief The coordinate of a single point.
	Vec3d coordinate;

	//! @brief The optional star at that coordinate.
	std::optional<QString> star;
};

//! @brief The possibles states during the drawing.
enum class Drawing
{
	//! @brief No line is available.
	None = 1,
	//! @brief The line as a starting point.
	hasStart = 2,
	//! @brief The line has a not placed end that is attached to the cursor.
	hasFloatingEnd = 4,
	//! @brief The line is complete i.e. has start and end point.
	hasEnd = 8,
	//! @brief The end is an already existing point.
	hasEndExistingPoint = 16,
};
}  // namespace scm

#endif