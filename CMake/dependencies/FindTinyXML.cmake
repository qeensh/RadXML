#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find TINYXML
# Once done, this will define
#
#  TINYXML_FOUND - system has TINYXML
#  TINYXML_INCLUDE_DIRS - the TINYXML include directories 
#  TINYXML_LIBRARIES - link these to use TINYXML
#  TINYXML_BINARY_REL / TINYXML_BINARY_DBG - DLL names (windows only)

include(FindPkgMacros)
findpkg_begin(TINYXML)

set (TINYXML_SOURCE $ENV{TINYXML_SOURCE} CACHE PATH "TINYXML path")
set (TINYXML_LIBRARY_PATH $ENV{TINYXML_LIBRARY_PATH} CACHE PATH "TINYXML library path")

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(TINYXML_SOURCE)
getenv_path(RADXML_DEPENDENCIES_DIR)

# construct search paths
set(TINYXML_PREFIX_PATH ${TINYXML_SOURCE} ${ENV_TINYXML_SOURCE} 
	${RADXML_DEPENDENCIES_DIR} ${ENV_RADXML_DEPENDENCIES_DIR})
create_search_paths(TINYXML)
# redo search if prefix path changed
clear_if_changed(TINYXML_PREFIX_PATH
  TINYXML_LIBRARY_FWK
  TINYXML_LIBRARY_REL
  TINYXML_LIBRARY_DBG
  TINYXML_INCLUDE_DIR
)

set(TINYXML_LIBRARY_NAMES tinyxml)
get_debug_names(TINYXML_LIBRARY_NAMES)

use_pkgconfig(TINYXML_PKGC TINYXML)

# For TINYXML, prefer static library over framework (important when referencing TINYXML source build)
set(CMAKE_FIND_FRAMEWORK "LAST")

findpkg_framework(TINYXML)

set (TINYXML_INC_SEARCH_PATH ${TINYXML_INC_SEARCH_PATH} ${TINYXML_SOURCE})
set (TINYXML_LIB_SEARCH_PATH ${TINYXML_LIB_SEARCH_PATH} ${TINYXML_LIBRARY_PATH})

if (APPLE AND TINYXML_SOURCE)
  # TINYXML source build on Mac stores libs in a different location
  # Also this is for static build
  set(TINYXML_LIB_SEARCH_PATH ${TINYXML_LIB_SEARCH_PATH} ${TINYXML_SOURCE}/Mac/XCode-2.2/build)
endif()

find_path(TINYXML_INCLUDE_DIR NAMES tinyxml.h HINTS ${TINYXML_INC_SEARCH_PATH} ${TINYXML_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "")

if (SYMBIAN) 
	set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
	set(CMAKE_PREFIX_PATH ${CMAKE_SYSYEM_OUT_DIR})
	message(STATUS "Lib will be searched in Symbian out dir: ${CMAKE_SYSYEM_OUT_DIR}")
endif (SYMBIAN)

find_library(TINYXML_LIBRARY_REL NAMES ${TINYXML_LIBRARY_NAMES} HINTS ${TINYXML_LIB_SEARCH_PATH} ${TINYXML_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(TINYXML_LIBRARY_DBG NAMES ${TINYXML_LIBRARY_NAMES_DBG} HINTS ${TINYXML_LIB_SEARCH_PATH} ${TINYXML_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

if (SYMBIAN) 
	set(TINYXML_LIBRARY_REL "${FreeImage_LIBRARY_REL} TINYXMLBase.lib touchfeedback.lib")
	set(CMAKE_PREFIX_PATH ${ORIGINAL_CMAKE_PREFIX_PATH})
endif (SYMBIAN)

make_library_set(TINYXML_LIBRARY)

mark_as_advanced(TINYXML_BINARY_REL TINYXML_BINARY_DBG)

findpkg_finish(TINYXML)

# add parent of TINYXML folder to support TINYXML/TINYXML.h
add_parent_dir(TINYXML_INCLUDE_DIRS TINYXML_INCLUDE_DIR)

# Reset framework finding
set(CMAKE_FIND_FRAMEWORK "FIRST")
