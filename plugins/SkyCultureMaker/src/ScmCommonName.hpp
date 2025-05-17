/**
 * @file ScmCommonName.hpp
 * @author lgrumbach
 * @brief Represents a common name of a star, planet or nonstellar object. Do not use for constellations.
 * @version 0.1
 * @date 2025-05-09
 *
 */

#ifndef SCM_COMMONNAME_HPP
#define SCM_COMMONNAME_HPP

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <optional>

namespace scm
{
class ScmCommonName;
}

class scm::ScmCommonName
{
public:
	ScmCommonName(QString id, QString englishName);
	~ScmCommonName();

	//! Sets the native name
	void setNativeName(QString name);

	//! Sets the native name in European glyphs or Pinyin for Chinese.
	void setPronounce(QString name);

	//! Sets the native name in IPA (International Phonetic Alphabet)
	void setIpa(QString name);

	//! Sets the references to the sources of the name spellings
	void setReferences(QVector<int> refs);

	//! Returns the identifier of the common name
	QString getId() const;

	//! Returns the english name
	QString getEnglishName() const;

	//! Returns the native name
	std::optional<QString> getNativeName() const;

	//! Returns the native name in European glyphs or Pinyin for Chinese.
	std::optional<QString> getPronounce() const;

	//! Returns the native name in IPA (International Phonetic Alphabet)
	std::optional<QString> getIpa() const;

	//! Returns the references to the sources of the name spellings
	std::optional<QVector<int>> getReferences() const;

	//! Returns the common name as a JSON object
	QJsonObject toJson() const;

private:
	/*! Identifier of the common name
	 * 	Example:
	 *    Star  : "HIP  <code>"
	 *    Planet: "NAME <name>"
	 */
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

#endif	// SCM_COMMONNAME_HPP
