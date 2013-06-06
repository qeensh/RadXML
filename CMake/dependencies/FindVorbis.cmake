#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find VORBIS
# Once done, this will define
#
#  VORBIS_FOUND - system has VORBIS
#  VORBIS_INCLUDE_DIRS - the VORBIS include directories 
#  VORBIS_LIBRARIES - link these to use VORBIS
#  VORBIS_BINARY_REL / VORBIS_BINARY_DBG - DLL names (windows only)

include(FindPkgMacros)
findpkg_begin(VORBIS)

set (VORBIS_SOURCE $ENV{VORBIS_SOURCE} CACHE PATH "Vorbis path")
set (VORBIS_LIBRARY_PATH $ENV{VORBIS_LIBRARY_PATH} CACHE PATH "Vorbis library path")

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(VORBIS_SOURCE)
getenv_path(RADXML_DEPENDENCIES_DIR)

# construct search paths
set(VORBIS_PREFIX_PATH ${VORBIS_SOURCE} ${ENV_VORBIS_SOURCE} 
	${RADXML_DEPENDENCIES_DIR} ${ENV_RADXML_DEPENDENCIES_DIR})
create_search_paths(VORBIS)
# redo search if prefix path changed
clear_if_changed(VORBIS_PREFIX_PATH
  VORBIS_LIBRARY_FWK
  VORBIS_LIBRARY_REL
  VORBIS_LIBRARY_DBG
  VORBIS_INCLUDE_DIR
)

set(VORBIS_LIBRARY_NAMES libVORBIS)
get_debug_names(VORBIS_LIBRARY_NAMES)

use_pkgconfig(VORBIS_PKGC VORBIS)

# For VORBIS, prefer static library over framework (important when referencing VORBIS source build)
set(CMAKE_FIND_FRAMEWORK "LAST")

findpkg_framework(VORBIS)

set (VORBIS_LIB_SEARCH_PATH ${VORBIS_LIB_SEARCH_PATH} ${VORBIS_LIBRARY_PATH})

if (APPLE AND VORBIS_SOURCE)
  # VORBIS source build on Mac stores libs in a different location
  # Also this is for static build
  set(VORBIS_LIB_SEARCH_PATH ${VORBIS_LIB_SEARCH_PATH} ${VORBIS_SOURCE}/Mac/XCode-2.2/build)
endif()

find_path(VORBIS_INCLUDE_DIR NAMES vorbisfile.h HINTS ${VORBIS_INC_SEARCH_PATH} ${VORBIS_PKGC_INCLUDE_DIRS} PATH_SUFFIXES vorbis)

if (SYMBIAN) 
	set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
	set(CMAKE_PREFIX_PATH ${CMAKE_SYSYEM_OUT_DIR})
	message(STATUS "Lib will be searched in Symbian out dir: ${CMAKE_SYSYEM_OUT_DIR}")
endif (SYMBIAN)

find_library(VORBIS_LIBRARY_REL NAMES ${VORBIS_LIBRARY_NAMES} HINTS ${VORBIS_LIB_SEARCH_PATH} ${VORBIS_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(VORBIS_LIBRARY_DBG NAMES ${VORBIS_LIBRARY_NAMES_DBG} HINTS ${VORBIS_LIB_SEARCH_PATH} ${VORBIS_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

if (SYMBIAN) 
	set(VORBIS_LIBRARY_REL "${FreeImage_LIBRARY_REL} VORBISBase.lib touchfeedback.lib")
	set(CMAKE_PREFIX_PATH ${ORIGINAL_CMAKE_PREFIX_PATH})
endif (SYMBIAN)

make_library_set(VORBIS_LIBRARY)

mark_as_advanced(VORBIS_BINARY_REL VORBIS_BINARY_DBG)

findpkg_finish(VORBIS)

# add parent of VORBIS folder to support VORBIS/VORBIS.h
add_parent_dir(VORBIS_INCLUDE_DIRS VORBIS_INCLUDE_DIR)

# Reset framework finding
set(CMAKE_FIND_FRAMEWORK "FIRST")
