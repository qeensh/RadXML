#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find OGG
# Once done, this will define
#
#  OGG_FOUND - system has OGG
#  OGG_INCLUDE_DIRS - the OGG include directories 
#  OGG_LIBRARIES - link these to use OGG
#  OGG_BINARY_REL / OGG_BINARY_DBG - DLL names (windows only)

include(FindPkgMacros)
findpkg_begin(OGG)

set (OGG_SOURCE $ENV{OGG_SOURCE} CACHE PATH "Ogg path")
set (OGG_LIBRARY_PATH $ENV{OGG_LIBRARY_PATH} CACHE PATH "Ogg library path")

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(OGG_SOURCE)
getenv_path(RADXML_DEPENDENCIES_DIR)

# construct search paths
set(OGG_PREFIX_PATH ${OGG_SOURCE} ${ENV_OGG_SOURCE} 
	${RADXML_DEPENDENCIES_DIR} ${ENV_RADXML_DEPENDENCIES_DIR})
create_search_paths(OGG)
# redo search if prefix path changed
clear_if_changed(OGG_PREFIX_PATH
  OGG_LIBRARY_FWK
  OGG_LIBRARY_REL
  OGG_LIBRARY_DBG
  OGG_INCLUDE_DIR
)

set(OGG_LIBRARY_NAMES libogg)
get_debug_names(OGG_LIBRARY_NAMES)

use_pkgconfig(OGG_PKGC OGG)

# For OGG, prefer static library over framework (important when referencing OGG source build)
set(CMAKE_FIND_FRAMEWORK "LAST")

findpkg_framework(OGG)

set (OGG_LIB_SEARCH_PATH ${OGG_LIB_SEARCH_PATH} ${OGG_LIBRARY_PATH})

if (APPLE AND OGG_SOURCE)
  # OGG source build on Mac stores libs in a different location
  # Also this is for static build
  set(OGG_LIB_SEARCH_PATH ${OGG_LIB_SEARCH_PATH} ${OGG_SOURCE}/Mac/XCode-2.2/build)
endif()

find_path(OGG_INCLUDE_DIR NAMES ogg.h HINTS ${OGG_INC_SEARCH_PATH} ${OGG_PKGC_INCLUDE_DIRS} PATH_SUFFIXES ogg)

if (SYMBIAN) 
	set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
	set(CMAKE_PREFIX_PATH ${CMAKE_SYSYEM_OUT_DIR})
	message(STATUS "Lib will be searched in Symbian out dir: ${CMAKE_SYSYEM_OUT_DIR}")
endif (SYMBIAN)

find_library(OGG_LIBRARY_REL NAMES ${OGG_LIBRARY_NAMES} HINTS ${OGG_LIB_SEARCH_PATH} ${OGG_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(OGG_LIBRARY_DBG NAMES ${OGG_LIBRARY_NAMES_DBG} HINTS ${OGG_LIB_SEARCH_PATH} ${OGG_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

if (SYMBIAN) 
	set(OGG_LIBRARY_REL "${FreeImage_LIBRARY_REL} OGGBase.lib touchfeedback.lib")
	set(CMAKE_PREFIX_PATH ${ORIGINAL_CMAKE_PREFIX_PATH})
endif (SYMBIAN)

make_library_set(OGG_LIBRARY)

mark_as_advanced(OGG_BINARY_REL OGG_BINARY_DBG)

findpkg_finish(OGG)

# add parent of OGG folder to support OGG/OGG.h
add_parent_dir(OGG_INCLUDE_DIRS OGG_INCLUDE_DIR)

# Reset framework finding
set(CMAKE_FIND_FRAMEWORK "FIRST")
