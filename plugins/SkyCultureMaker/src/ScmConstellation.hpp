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
#include <QJsonObject>
#include <optional>

namespace scm
{
    class ScmConstellation;
}

class scm::ScmConstellation
{
public:
    //! Returns the constellation as a JSON object
	QJsonObject toJson() const;

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
};

#endif	// SCM_CONSTELLATION_HPP
