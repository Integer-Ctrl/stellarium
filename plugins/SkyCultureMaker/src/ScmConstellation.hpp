/**
 * @file ScmConstellation.hpp
 * @author lgrumbach
 * @brief Represents a constellation in a sky culture.
 * @version 0.1
 * @date 2025-05-09
 *
 */

#ifndef SCM_CONSTELLATION_HPP
#define SCM_CONSTELLATION_HPP

#include <QString>
#include <QVector>
#include <optional>
#include <variant>
#include "ScmTypes.hpp"

namespace scm
{

class ScmConstellation
{
public:
	ScmConstellation(scm::ListCoordinateStar constellation);

	void setId(QString id);
    QString getId() const;

	void setEnglishName(QString name);
	void setNativeName(std::optional<QString> name);
	void setPronounce(std::optional<QString> pronounce);
	void setIPA(std::optional<QString> ipa);
	void setConstellation(scm::ListCoordinateStar constellation);
private:
    //! Identifier of the constellation
    QString id;

    //! The english name
	QString englishName;

	//! The native name
	std::optional<QString> nativeName;

	//! Native name in European glyphs, if needed. For Chinese, expect Pinyin here.
	std::optional<QString> pronounce;

	//! The native name in IPA (International Phonetic Alphabet)
	std::optional<QString> ipa;

	//! References to the sources of the name spellings
	std::optional<QVector<int>> references;

	//! List of stars forming the segments
	scm::ListCoordinateStar constellation;
};

}  // namespace scm


#endif	// SCM_CONSTELLATION_HPP
