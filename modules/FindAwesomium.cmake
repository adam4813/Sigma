include (FindPackageHandleStandardArgs)

find_path(AWESOMIUM_INCLUDE_DIR Awesomium/WebCore.h
	PATH_SUFFIXES include
	PATHS
	${AWESOMIUM_ROOT}
	$ENV{AWESOMIUM_ROOT}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/
	/usr/include/
	/usr/
	/sw          # Fink
	/opt/local/  # DarwinPorts
	/opt/csw/    # Blastwave
	/opt/)
	
find_library(AWESOMIUM_LIBRARY 
	NAMES awesomium awesomium-1-7
	PATH_SUFFIXES lib64 lib build/lib 
	PATHS
	${AWESOMIUM_ROOT}
	$ENV{AWESOMIUM_ROOT}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/
	/usr/lib/
	/usr/
	/sw          # Fink
	/opt/local/  # DarwinPorts
	/opt/csw/    # Blastwave
	/opt/)

find_package_handle_standard_args (AWESOMIUM DEFAULT_MSG AWESOMIUM_LIBRARY AWESOMIUM_INCLUDE_DIR)
mark_as_advanced (AWESOMIUM_INCLUDE_DIR AWESOMIUM_LIBRARY)

