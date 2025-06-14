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
#include <QString>
#include <QJsonArray>

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

    /**
     * @brief Extracts the HIP number from a string.
     * 
     * @param hipString The string containing the HIP number.
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
     * @brief Converts the ConstellationLine to a JSON array.
     * 
     * @return QJsonArray The JSON representation of the ConstellationLine.
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
				json.append(hipId);
			}
			else if(startNameStr.contains("Gaia DR3", Qt::CaseInsensitive))
			{
				json.append(startName.value());
			}
			else
			{
				return QJsonArray();
			}
		}
		else if (startRAJ2000 != 0.0f && startDecJ2000 != 0.0f)
		{
			QJsonArray coordinateArray;
			coordinateArray.append(startRAJ2000);
			coordinateArray.append(startDecJ2000);
			json.append(coordinateArray);
		}
		else
		{
			return QJsonArray();
		}

		// END POINT
		if (endName.has_value())
		{
			QString endNameStr = endName.value();
			if (int hipId = extractHipNumber(endNameStr); hipId != -1)
			{
				json.append(hipId);
			}
			else if(endNameStr.contains("Gaia DR3", Qt::CaseInsensitive))
			{
				json.append(endNameStr);
			}
			else
			{
				return QJsonArray();
			}
		}
		else if (endRAJ2000 != 0.0f && endDecJ2000 != 0.0f)
		{
			QJsonArray coordinateArray;
			coordinateArray.append(endRAJ2000);
			coordinateArray.append(endDecJ2000);
			json.append(coordinateArray);
		}
		else
		{
			return QJsonArray();
		}
		return json;
	}
};

} // namespace scm

#endif
