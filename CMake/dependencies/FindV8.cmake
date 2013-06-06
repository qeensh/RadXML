#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find V8
# Once done, this will define
#
#  V8_FOUND - system has V8
#  V8_INCLUDE_DIRS - the V8 include directories 
#  V8_LIBRARIES - link these to use V8
#  V8_BINARY_REL / V8_BINARY_DBG - DLL names (windows only)

include(FindPkgMacros)
findpkg_begin(V8)

set (V8_SOURCE $ENV{V8_SOURCE} CACHE PATH "V8 path")

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(V8_SOURCE)
getenv_path(RADXML_DEPENDENCIES_DIR)

# construct search paths
set(V8_PREFIX_PATH ${V8_SOURCE} ${ENV_V8_SOURCE} 
	${RADXML_DEPENDENCIES_DIR} ${ENV_RADXML_DEPENDENCIES_DIR})
create_search_paths(V8)
# redo search if prefix path changed
clear_if_changed(V8_PREFIX_PATH
  V8_LIBRARY_FWK
  V8_LIBRARY_REL
  V8_LIBRARY_DBG
  V8_INCLUDE_DIR
)

set(V8_LIBRARY_NAMES v8)
get_debug_names(V8_LIBRARY_NAMES)

use_pkgconfig(V8_PKGC V8)

# For V8, prefer static library over framework (important when referencing V8 source build)
set(CMAKE_FIND_FRAMEWORK "LAST")

findpkg_framework(V8)

if (APPLE AND V8_SOURCE)
  # V8 source build on Mac stores libs in a different location
  # Also this is for static build
  set(V8_LIB_SEARCH_PATH ${V8_LIB_SEARCH_PATH} ${V8_SOURCE}/Mac/XCode-2.2/build)
endif()

find_path(V8_INCLUDE_DIR NAMES V8.h HINTS ${V8_INC_SEARCH_PATH} ${V8_PKGC_INCLUDE_DIRS} PATH_SUFFIXES V8)

if (SYMBIAN) 
	set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
	set(CMAKE_PREFIX_PATH ${CMAKE_SYSYEM_OUT_DIR})
	message(STATUS "Lib will be searched in Symbian out dir: ${CMAKE_SYSYEM_OUT_DIR}")
endif (SYMBIAN)

find_library(V8_LIBRARY_REL NAMES ${V8_LIBRARY_NAMES} HINTS ${V8_LIB_SEARCH_PATH} ${V8_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(V8_LIBRARY_DBG NAMES ${V8_LIBRARY_NAMES_DBG} HINTS ${V8_LIB_SEARCH_PATH} ${V8_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

if (SYMBIAN) 
	set(V8_LIBRARY_REL "${FreeImage_LIBRARY_REL} V8Base.lib touchfeedback.lib")
	set(CMAKE_PREFIX_PATH ${ORIGINAL_CMAKE_PREFIX_PATH})
endif (SYMBIAN)

make_library_set(V8_LIBRARY)

mark_as_advanced(V8_BINARY_REL V8_BINARY_DBG)

findpkg_finish(V8)

# add parent of V8 folder to support V8/V8.h
add_parent_dir(V8_INCLUDE_DIRS V8_INCLUDE_DIR)

# Reset framework finding
set(CMAKE_FIND_FRAMEWORK "FIRST")
