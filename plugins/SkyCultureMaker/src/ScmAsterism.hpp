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
 
namespace scm
{
   class ScmAsterism;
}
 
class scm::ScmAsterism
{
public:
 
    //! Sets the common name of the asterism
    void setCommonName(ScmCommonName name);

    //! Returns the common name of the asterism
    ScmCommonName getCommonName() const;
 
private:
    //! Common name of the constellation
    ScmCommonName commonName;
};
 
#endif	// SCM_ASTERISM_HPP
 