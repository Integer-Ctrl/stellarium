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
#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace scm
{
struct ConstellationLine
{
	//! Default value for right ascension and declination angles.
	static constexpr double DEFAULT_RA_DEC = 361.0;

	//! The coordinate of the start point of the line.
	Vec3d startCoordinate = Vec3d(0.0, 0.0, 0.0);

	//! Optional ID of the start point, e.g. a star name.
	QString startId;

	//! Right ascension angle (J2000 frame) in decimal degrees for the start point.
	double startRAJ2000 = DEFAULT_RA_DEC;

	//! Declination angle (J2000 frame) in decimal degrees for the start point.
	double startDecJ2000 = DEFAULT_RA_DEC;

	//! Star ID number for the start point.
	QString startIdNumber = "-1";

	//! The coordinate of the end point of the line.
	Vec3d endCoordinate = Vec3d(0.0, 0.0, 0.0);

	//! Optional ID of the end point, e.g. a star name.
	QString endId;

	//! Right ascension angle (J2000 frame) in decimal degrees for the end point.
	double endRAJ2000 = DEFAULT_RA_DEC;

	//! Declination angle (J2000 frame) in decimal degrees for the end point.
	double endDecJ2000 = DEFAULT_RA_DEC;

	//! Star ID number for the end point.
	QString endIdNumber = "-1";

	/**
	 * @brief Gets the star ID from the name.
	 * 
	 * @param starId The ID of the star, which may contain identifiers like "HIP" or "Gaia DR3".
	 */
	static QString getStarIdNumber(QString starId)
	{
		QRegularExpression hipExpression(R"(HIP\s+(\d+))");
		QRegularExpression gaiaExpression(R"(Gaia DR3\s+(\d+))");

		QRegularExpressionMatch hipMatch = hipExpression.match(starId);
		if (hipMatch.hasMatch())
		{
			return hipMatch.captured(1);
		}
		QRegularExpressionMatch gaiaMatch = gaiaExpression.match(starId);
		if (gaiaMatch.hasMatch())
		{
			return gaiaMatch.captured(1);
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

        // only save names if both are not empty
        if (startIdNumber != "-1" && endIdNumber != "-1")
		{
			// START POINT
			if (startId.contains("HIP"))
			{
                // save HIP as int
				json.append(startIdNumber.toInt());
			}
			else
			{
				// save other numbers as string (Gaia DR3)
				json.append(startIdNumber);
			}
            // END POINT
			if (endId.contains("HIP"))
			{
                // save HIP as int
				json.append(endIdNumber.toInt());
			}
			else
			{
				// save other numbers as string (Gaia DR3)
				json.append(endIdNumber);
			}
		}
		else if(startRAJ2000 != DEFAULT_RA_DEC && startDecJ2000 != DEFAULT_RA_DEC &&
				endRAJ2000 != DEFAULT_RA_DEC && endDecJ2000 != DEFAULT_RA_DEC)
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
		else
		{
			// If no names or coordinates are available, return empty array
			return QJsonArray();
		}
		return json;
	}
};

} // namespace scm

#endif
