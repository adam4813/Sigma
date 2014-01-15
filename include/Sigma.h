#ifndef SIGMA_H_INCLUDED
#define SIGMA_H_INCLUDED

#include <string>
#include <stdint.h>

// Put in this header all the things shared by all code
namespace Sigma {
    typedef uint32_t id_t;
    typedef std::string ComponentID;
    // A feature is a set of predefined components that are strongly dependant
    // for example the rotational and directional forces are grouped in ControllableMove
    typedef std::string CompositeID;
}

#endif // SIGMA_H_INCLUDED
