#ifndef SIGMA_H_INCLUDED
#define SIGMA_H_INCLUDED

#include <string>
#include <stdint.h>

// Put in this header all the things shared by all code
namespace Sigma {
	typedef uint32_t id_t;
	typedef std::string ComponentID;
}

#ifdef libSigma_EXPORTS
// If building as shared library
#if defined(_MSC_VER)
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__clang__)
#define DLL_EXPORT __attribute__((visibility("default")))
#define DLL_IMPORT
#endif
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif
#endif // SIGMA_H_INCLUDED
