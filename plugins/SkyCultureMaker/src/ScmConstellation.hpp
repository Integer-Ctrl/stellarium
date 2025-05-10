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

#include "ScmCommonName.hpp"

namespace scm
{
class ScmConstellation;
}

class scm::ScmConstellation
{
public:

    //! Sets the common name of the constellation
    void setCommonName(ScmCommonName name);

    //! Returns the common name of the constellation
    ScmCommonName getCommonName() const;

private:
    //! Common name of the constellation
    ScmCommonName commonName;
};

#endif	// SCM_CONSTELLATION_HPP
