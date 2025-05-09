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
#include <optional>

#include "StelSkyCultureMgr.hpp"
#include "Constellation.hpp"
#include "Asterism.hpp"

#include "ScmCommonName.hpp"

namespace scm
{
class ScmSkyCulture;
}

class scm::ScmSkyCulture
{
public:
	ScmSkyCulture(QString id, QString region, StelSkyCulture::CLASSIFICATION classification,
        bool fallbackToInternationalNames, QVector<Asterism> asterisms,
        QVector<Constellation> constellations, QVector<ScmCommonName> commonNames);
	~ScmSkyCulture();

    //! Returns the identifier of the sky culture
    QString getId() const;

    //! Returns the region of the sky culture
    QString getRegion() const;

    //! Returns the classification of the sky culture
    StelSkyCulture::CLASSIFICATION getClassification() const;

    //! Returns whether to show common names in addition to the culture-specific ones
    bool getFallbackToInternationalNames() const;

    //! Returns the asterisms of the sky culture
    QVector<Asterism> getAsterisms() const;

    //! Returns the constellations of the sky culture
    QVector<Constellation> getConstellations() const;

    //! Returns the common names of the stars, planets and nonstellar objects
    QVector<ScmCommonName> getCommonNames() const;

    //! Returns the sky culture as a JSON object
    QJsonObject toJson() const;

private:
	//! Sky culture identifier
	QString id;

	/*! The name of region following the United Nations geoscheme UN~M49
	 https://unstats.un.org/unsd/methodology/m49/ For skycultures of worldwide applicability (mostly those
	 adhering to IAU constellation borders), use "World".
    */
	QString region;

	//! Classification of sky culture (enum)
	StelSkyCulture::CLASSIFICATION classification;

	//! Whether to show common names in addition to the culture-specific ones
	bool fallbackToInternationalNames = false;

	// TODO: can we use "Asterism" or create own class?
	//! The asterisms of the sky culture
	QVector<Asterism> asterisms;

	// TODO: can we use "Constellation" or create own class?
	//! The constellations of the sky culture
	QVector<Constellation> constellations;

	//! The common names of the stars, planets and nonstellar objects
	QVector<ScmCommonName> commonNames;

	// TODO: edges:
	//! Type of the edges. Can be one of "none", "iau" or "own". TODO: enum?
	// std::optional<QString> edgeType;

	//! Source of the edges.
	// std::optional<QString> edgeSource;

	/*! Describes the coordinate epoch. Allowed values:
    "J2000" (default)
    "B1875" used for the edge list defining the IAU borders.
    "Byyyy.y" (a number with B prepended) Arbitrary epoch as Besselian year.
    "Jyyyy.y" (a number with J prepended) Arbitrary epoch as Julian year.
    "JDddddddd.ddd" (a number with JD prepended) Arbitrary epoch as Julian Day number.
    "ddddddd.ddd"
    */
	// std::optional<QString> edgeEpoch;

	//! Describes the edges of the constellations.
	// std::optional<QVector<QString>> edges;
};

#endif	// SCM_SKYCULTURE_HPP