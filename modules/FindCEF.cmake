# FindCEF.cmake
#

include(FindPackageHandleStandardArgs)

FIND_LIBRARY(CEF_LIBRARY
	NAMES cef libcef cef.lib libcef.o "Chromium Embedded Framework"
	PATHS ${CMAKE_SOURCE_DIR}/cef ${CMAKE_SOURCE_DIR}/cef/Release
	)
FIND_LIBRARY(CEFWRAPPER_LIBRARY
	NAMES cef_dll_wrapper libcef_dll_wrapper
	PATHS ${CMAKE_SOURCE_DIR}/cef ${CMAKE_SOURCE_DIR}/cef/xcodebuild/Release
	)


if (NOT CEF_LIBRARY)
    message(FATAL_ERROR "Could not find the CEF shared library" )
endif (NOT CEF_LIBRARY)
if (NOT CEFWRAPPER_LIBRARY)
    message(FATAL_ERROR "Could not find the CEF wrapper library" )
endif (NOT CEFWRAPPER_LIBRARY)

set(CEF_LIBRARIES
	${CEF_LIBRARY}
	${CEFWRAPPER_LIBRARY}
	)

find_package_handle_standard_args(CEF DEFAULT_MSG CEF_LIBRARY CEFWRAPPER_LIBRARY)
mark_as_advanced(CEF_LIBRARY CEF_WRAPPER_LIBRARY CEF_LIBRARIES)

