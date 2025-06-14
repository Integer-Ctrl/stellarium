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
#include <QJsonArray>

namespace scm
{
//! The pair of start and end coordinate
struct CoordinateLine
{
	//! The start coordinate of the line.
	Vec3d start;

	//! Optional name of the start coordinate, e.g. a star name.
	std::optional<QString> startName;

	//! Right ascension angle (J2000 frame) in decimal degrees for the start coordinate.
	float startRAJ2000 = 0.0f;

	//! Declination angle (J2000 frame) in decimal degrees for the start coordinate.
	float startDecJ2000 = 0.0f;

	//! The end coordinate of the line.
	Vec3d end;

	//! Optional name of the end coordinate, e.g. a star name.
	std::optional<QString> endName;

	//! Right ascension angle (J2000 frame) in decimal degrees for the end coordinate.
	float endRAJ2000 = 0.0f;

	//! Declination angle (J2000 frame) in decimal degrees for the end coordinate.
	float endDecJ2000 = 0.0f;

	/**
	* @brief Extracts the HIP number from a star name such as "HIP 32349 A" or "HIP 31223".
	* @param hipString The HIP code string.
	* @return The HIP number as an integer, or -1 if the pattern doesn't match.
	*/
	inline int extractHipNumber(const QString& hipString) const
	{
		QRegularExpression re(R"(HIP\s+(\d+))");
		QRegularExpressionMatch match = re.match(hipString);
		if (match.hasMatch())
		{
			return match.captured(1).toInt();
		}
		return -1;
	}

	/**
 	* @brief Converts a CoordinateLine to a QJsonArray.
 	* @param line The CoordinateLine to convert.
 	* @return A QJsonArray representing the line, containing either star names or coordinates.
 	*/
	QJsonArray toJson() const
	{
		QJsonArray json;
		// START POINT
		if (startName.has_value())
		{
			QString startNameStr = startName.value();
			if (int hipId = extractHipNumber(startNameStr); hipId != -1)
			{
				// append HIP Id is available
				json.append(hipId);
			}
			// add Gaia DR3 star names as string
			else if(startNameStr.contains("Gaia DR3", Qt::CaseInsensitive))
			{
				json.append(startName.value());
			}
			else
			{
				return QJsonArray(); // Return an empty array if no valid data is available
			}
		}
		else if (startRAJ2000 != 0.0f && startDecJ2000 != 0.0f)
		{
			// If no name is available, use the right ascension and declination
			QJsonArray coordinateArray;
			coordinateArray.append(startRAJ2000);
			coordinateArray.append(startDecJ2000);
			json.append(coordinateArray);
		}
		else
		{
			return QJsonArray(); // Return an empty array if no valid data is available
		}

		// END POINT
		if (endName.has_value())
		{
			QString endNameStr = endName.value();
			if (int hipId = extractHipNumber(endNameStr); hipId != -1)
			{
				// append HIP Id is available
				json.append(hipId);
			}
			// add Gaia DR3 star names as string
			else if(endNameStr.contains("Gaia DR3", Qt::CaseInsensitive))
			{
				json.append(endNameStr);
			}
			else
			{
				return QJsonArray(); // Return an empty array if no valid data is available
			}
		}
		else if (endRAJ2000 != 0.0f && endDecJ2000 != 0.0f)
		{
			// If no name is available, use the right ascension and declination
			QJsonArray coordinateArray;
			coordinateArray.append(endRAJ2000);
			coordinateArray.append(endDecJ2000);
			json.append(coordinateArray);
		}
		else
		{
			return QJsonArray(); // Return an empty array if no valid data is available
		}
		return json;
	}
};

} // namespace scm

#endif