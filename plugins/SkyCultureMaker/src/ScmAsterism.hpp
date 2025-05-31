/**
 * @file ScmAsterism.hpp
 * @author lgrumbach
 * @brief Represents an asterism in a sky culture.
 * @version 0.1
 * @date 2025-05-09
 *
 */

#ifndef SCM_ASTERISM_HPP
#define SCM_ASTERISM_HPP
 
#include "ScmCommonName.hpp"
#include "QString"
 
namespace scm
{
 
class ScmAsterism
{
public:
    void setId(QString id);
    QString getId() const;

    //! Sets the common name of the asterism
    void setCommonName(ScmCommonName name);

    //! Returns the common name of the asterism
    ScmCommonName getCommonName() const;
 
private:
    //! Id of the Asterism
    QString id;

    //! Common name of the constellation
    ScmCommonName commonName;
};

}  // namespace scm

 
#endif	// SCM_ASTERISM_HPP
 