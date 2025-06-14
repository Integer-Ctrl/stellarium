/**
 * @file ScmSkyCulture.hpp
 * @author fhofer, lgrumbach
 * @brief Object holding all data of a sky culture during the creation process with the Sky Culture Maker.
 * @version 0.1
 * @date 2025-05-09
 *
 * The minimun information that the object should store can be found in any index.json file of an existing sky culture.
 * For example: /{userpath}/stellarium/skycultures/maya/index.json
 */
#ifndef SCM_SKYCULTURE_HPP
#define SCM_SKYCULTURE_HPP

#include <QString>
// #include <vector>
#include <optional>

#include "StelSkyCultureMgr.hpp"
#include "ScmConstellation.hpp"
#include "ScmAsterism.hpp"
#include "types/CoordinateLine.hpp"
#include "types/StarLine.hpp"
#include "types/License.hpp"
#include "types/Classification.hpp"
#include "ScmCommonName.hpp"
#include "StelCore.hpp"

namespace scm
{

class ScmSkyCulture
{
public:
	/// Sets the id of the sky culture
	void setId(QString id);

	/// Sets the region of the sky culture
	void setRegion(QString region);

	/// Sets the classification of the sky culture
	void setClassificationType(ClassificationType classificationType);

	/// Sets whether to show common names in addition to the culture-specific ones
	void setFallbackToInternationalNames(bool fallback);

	/// Adds an asterism to the sky culture
	void addAsterism(ScmAsterism asterism);

	/// Removes an asterism from the sky culture by its ID
	void removeAsterism(QString id);

	/// Adds a constellation to the sky culture
	void addConstellation(QString id, std::vector<CoordinateLine> coordinates, std::vector<StarLine> stars);

	/// Removes a constellation from the sky culture by its ID
	void removeConstellation(QString id);

	/// Gets a constellation from the sky culture by its ID
	scm::ScmConstellation *getConstellation(QString id);

	/// Adds a common name to the sky culture
	void addCommonName(ScmCommonName commonName);

	/// Removes a common name from the sky culture by its ID
	void removeCommonName(QString id);

	/// Returns the asterisms of the sky culture
	// TODO: QVector<ScmAsterism> getAsterisms() const;

	/// Returns a pointer to the constellations of the sky culture
	std::vector<ScmConstellation> *getConstellations();

	/// Returns the common names of the stars, planets and nonstellar objects
	std::vector<ScmCommonName> getCommonNames() const;

	/// Sets the license of the sky culture
	void setLicense(scm::LicenseType license);

	/// Returns the license of the sky culture
	scm::LicenseType getLicense() const;

	/// Sets the authors of the sky culture
	void setAuthors(const QString authors);

	/// Returns the authors of the sky culture
	QString getAuthors() const;

	/**
	* @brief Returns the sky culture as a JSON object
	*/
	QJsonObject toJson() const;

	/**
	* @brief Draws the sky culture.
	*/
	void draw(StelCore *core);

private:
	/// Sky culture identifier
	QString id;

	/*! The name of region following the United Nations geoscheme UN~M49
	 *   https://unstats.un.org/unsd/methodology/m49/ For skycultures of worldwide applicability (mostly those
	 *   adhering to IAU constellation borders), use "World".
	 */
	QString region;

	/// Classification of the sky culture
	ClassificationType classificationType = ClassificationType::NONE;

	/// Whether to show common names in addition to the culture-specific ones
	bool fallbackToInternationalNames = false;

	/// The asterisms of the sky culture
	std::vector<ScmAsterism> asterisms;

	/// The constellations of the sky culture
	std::vector<ScmConstellation> constellations;

	/// The common names of the stars, planets and nonstellar objects
	std::vector<ScmCommonName> commonNames;

	/// The license of the sky culture
	scm::LicenseType license = scm::LicenseType::NONE;

	/// The authors of the sky culture
	QString authors;

	// TODO: edges:
	/// Type of the edges. Can be one of "none", "iau" or "own". TODO: enum?
	// std::optional<QString> edgeType;

	/// Source of the edges.
	// std::optional<QString> edgeSource;

	/*! Describes the coordinate epoch. Allowed values:
	 *   "J2000" (default)
	 *   "B1875" used for the edge list defining the IAU borders.
	 *   "Byyyy.y" (a number with B prepended) Arbitrary epoch as Besselian year.
	 *   "Jyyyy.y" (a number with J prepended) Arbitrary epoch as Julian year.
	 *   "JDddddddd.ddd" (a number with JD prepended) Arbitrary epoch as Julian Day number.
	 *   "ddddddd.ddd"
	 */
	// std::optional<QString> edgeEpoch;

	/// Describes the edges of the constellations.
	// std::optional<QVector<QString>> edges;
};

}  // namespace scm

#endif	// SCM_SKYCULTURE_HPP