# - Try to find the OggVorbis libraries
# Once done this will define
#
#  OGGVORBIS_FOUND - system has OggVorbis
#  OGGVORBIS_VERSION - set either to 1 or 2
#  OGGVORBIS_INCLUDE_DIR - the OggVorbis include directory
#  OGGVORBIS_LIBRARIES - The libraries needed to use OggVorbis
#  OGG_LIBRARY         - The Ogg library
#  VORBIS_LIBRARY      - The Vorbis library
#  VORBISFILE_LIBRARY  - The VorbisFile library
#  VORBISENC_LIBRARY   - The VorbisEnc library

# Copyright (c) 2006, Richard Laerkaeng, <richard@goteborg.utfors.se>
# Modifications for Sigma/Windows 2013, Meisaka Yukara
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


include (CheckLibraryExists)

find_path(VORBIS_INCLUDE_DIR vorbis/codec.h)
find_path(VORBISFILE_INCLUDE_DIR vorbis/vorbisfile.h)
find_path(OGG_INCLUDE_DIR ogg/ogg.h)

if(WIN32)
	find_library(OGG_LIBRARY NAMES libogg)
	find_library(OGG_SLIBRARY NAMES libogg_static)
	find_library(VORBIS_LIBRARY NAMES libvorbis)
	find_library(VORBIS_SLIBRARY NAMES libvorbis_static)
	find_library(VORBISFILE_LIBRARY NAMES libvorbisfile)
	find_library(VORBISENC_LIBRARY NAMES libvorbisenc)

	set(USE_STATIC_OGGVORBIS FALSE CACHE BOOL "Use static versions of ogg and vorbis libraries")

	if (VORBIS_INCLUDE_DIR AND NOT VORBIS_LIBRARY AND VORBIS_SLIBRARY)
	   set(USE_STATIC_OGGVORBIS TRUE)
	   set(VORBIS_LIBRARY VORBIS_SLIBRARY)
	   message(STATUS "Found static Vorbis library")
	endif (VORBIS_INCLUDE_DIR AND NOT VORBIS_LIBRARY AND VORBIS_SLIBRARY)

	if (OGG_INCLUDE_DIR AND NOT OGG_LIBRARY AND OGG_SLIBRARY)
	   set(USE_STATIC_OGGVORBIS TRUE)
	   set(VORBIS_LIBRARY VORBIS_SLIBRARY)
	   message(STATUS "Found static Ogg library")
	endif (OGG_INCLUDE_DIR AND NOT OGG_LIBRARY AND OGG_SLIBRARY)

	if (USE_STATIC_OGGVORBIS)
		set(OGG_LIBRARY ${OGG_SLIBRARY})
		set(VORBIS_LIBRARY ${VORBIS_SLIBRARY})
	endif (USE_STATIC_OGGVORBIS)

	unset(OGG_SLIBRARY)
	unset(VORBIS_SLIBRARY)
else(WIN32)
	find_library(OGG_LIBRARY NAMES ogg)
	find_library(VORBIS_LIBRARY NAMES vorbis)
	find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
	find_library(VORBISENC_LIBRARY NAMES vorbisenc)
endif(WIN32)

mark_as_advanced(VORBIS_INCLUDE_DIR OGG_INCLUDE_DIR
                 OGG_LIBRARY VORBIS_LIBRARY VORBISFILE_LIBRARY VORBISENC_LIBRARY)

if (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)
# AND VORBISFILE_LIBRARY AND VORBISENC_LIBRARY
   set(OGGVORBIS_FOUND TRUE)

   set(OGGVORBIS_LIBRARIES ${OGG_LIBRARY} ${VORBIS_LIBRARY})
   # ${VORBISFILE_LIBRARY} ${VORBISENC_LIBRARY}
   set(_CMAKE_REQUIRED_LIBRARIES_TMP ${CMAKE_REQUIRED_LIBRARIES})
   set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${OGGVORBIS_LIBRARIES})
   check_library_exists(vorbis vorbis_bitrate_addblock "" HAVE_LIBVORBISENC2)
   set(CMAKE_REQUIRED_LIBRARIES ${_CMAKE_REQUIRED_LIBRARIES_TMP})

   if (HAVE_LIBVORBISENC2)
      set (OGGVORBIS_VERSION 2)
   else (HAVE_LIBVORBISENC2)
      set (OGGVORBIS_VERSION 1)
   endif (HAVE_LIBVORBISENC2)

else (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)
   set (OGGVORBIS_VERSION)
   set(OGGVORBIS_FOUND FALSE)
endif (VORBIS_INCLUDE_DIR AND VORBIS_LIBRARY)


if (OGGVORBIS_FOUND)
   if (NOT OggVorbis_FIND_QUIETLY)
      message(STATUS "Found OggVorbis: ${OGGVORBIS_LIBRARIES}")
   endif (NOT OggVorbis_FIND_QUIETLY)
else (OGGVORBIS_FOUND)
   if (OggVorbis_FIND_REQUIRED)
      if (NOT OGG_LIBRARY)
         message(FATAL_ERROR "Could NOT find Ogg library")
	  endif (NOT OGG_LIBRARY)
	  if (NOT OGG_INCLUDE_DIR)
         message(FATAL_ERROR "Could NOT find Ogg header")
	  endif (NOT OGG_INCLUDE_DIR)
	  if (NOT VORBIS_INCLUDE_DIR)
         message(FATAL_ERROR "Could NOT find Vorbis headers")
	  endif (NOT VORBIS_INCLUDE_DIR)
	  if (NOT VORBIS_LIBRARY)
         message(FATAL_ERROR "Could NOT find Vorbis libraries")
	  endif (NOT VORBIS_LIBRARY)
	  message(FATAL_ERROR "Could NOT find all OggVorbis libraries")
   endif (OggVorbis_FIND_REQUIRED)
   if (NOT OggVorbis_FIND_QUITELY)
      message(STATUS "Could NOT find OggVorbis libraries")
   endif (NOT OggVorbis_FIND_QUITELY)
endif (OGGVORBIS_FOUND)
