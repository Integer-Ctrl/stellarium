/**
 * @file ConstellationLine.hpp
 * @author vgerlach, lgrumbach
 * @brief Type describing a line between two points.
 * @version 0.1
 * @date 2025-06-14
 */
#ifndef SCM_TYPES_CONSTELLATION_LINE_HPP
#define SCM_TYPES_CONSTELLATION_LINE_HPP

#include "VecMath.hpp"
#include <optional>
#include <QJsonArray>
#include <QString>

namespace scm
{
struct ConstellationLine
{
	//! The coordinate of the start point of the line.
	Vec3d startCoordinate;

	//! Optional name of the start point, e.g. a star name.
	std::optional<QString> startName;

	//! Right ascension angle (J2000 frame) in decimal degrees for the start point.
	float startRAJ2000 = 0.0f;

	//! Declination angle (J2000 frame) in decimal degrees for the start point.
	float startDecJ2000 = 0.0f;

	//! The coordinate of the end point of the line.
	Vec3d endCoordinate;

	//! Optional name of the end point, e.g. a star name.
	std::optional<QString> endName;

	//! Right ascension angle (J2000 frame) in decimal degrees for the end point.
	float endRAJ2000 = 0.0f;

	//! Declination angle (J2000 frame) in decimal degrees for the end point.
	float endDecJ2000 = 0.0f;

	inline QString getStarId(const std::optional<QString> &starName) const
	{
		if (starName.has_value())
		{
			QRegularExpression hipExpression(R"(HIP\s+(\d+))");
			QRegularExpression gaiaExpression(R"(Gaia DR3\s+(\d+))");

			QRegularExpressionMatch hipMatch = hipExpression.match(starName.value());
			if (hipMatch.hasMatch())
			{
				return hipMatch.captured(1);
			}
			QRegularExpressionMatch gaiaMatch = gaiaExpression.match(starName.value());
			if (gaiaMatch.hasMatch())
			{
				return gaiaMatch.captured(1);
			}
		}
		return "-1";
	}

	/**
     * @brief Converts the ConstellationLine to a JSON array.
     * 
     * @return QJsonArray The JSON representation of the ConstellationLine.
     */
	QJsonArray toJson() const
	{
		QJsonArray json;

        // get ids
        QString startId = getStarId(startName);
        QString endId = getStarId(endName);

        // only save names if both are not empty
        if (startId != "-1" && endId != "-1")
		{
			// START POINT
			if (startName.value().contains("HIP"))
			{
                // save HIP as int
				json.append(startId.toInt());
			}
			else
			{
				// save other numbers as string (Gaia DR3)
				json.append(startId);
			}
            // END POINT
			if (endName.value().contains("HIP"))
			{
                // save HIP as int
				json.append(endId.toInt());
			}
			else
			{
				// save other numbers as string (Gaia DR3)
				json.append(endId);
			}
		}
		else
		{
            // Only if both start and end points do not have names, we save the coordinates
			QJsonArray startCoordinateArray;
			startCoordinateArray.append(startRAJ2000);
			startCoordinateArray.append(startDecJ2000);
			json.append(startCoordinateArray);

			QJsonArray endCoordinateArray;
			endCoordinateArray.append(endRAJ2000);
			endCoordinateArray.append(endDecJ2000);
			json.append(endCoordinateArray);
		}
		return json;
	}
};

} // namespace scm

#endif
