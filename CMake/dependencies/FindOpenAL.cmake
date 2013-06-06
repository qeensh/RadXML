#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find OPENAL
# Once done, this will define
#
#  OPENAL_FOUND - system has OPENAL
#  OPENAL_INCLUDE_DIRS - the OPENAL include directories 
#  OPENAL_LIBRARIES - link these to use OPENAL
#  OPENAL_BINARY_REL / OPENAL_BINARY_DBG - DLL names (windows only)

include(FindPkgMacros)
findpkg_begin(OPENAL)

set (OPENAL_SOURCE $ENV{OPENAL_SOURCE} CACHE PATH "OpenAL path")
set (OPENAL_LIBRARY_PATH $ENV{OPENAL_LIBRARY_PATH} CACHE PATH "OpenAL library path")
set (OPENAL_USE_BITS "32" CACHE STRING "Use 32 or 64 bit?")

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(OPENAL_SOURCE)
getenv_path(RADXML_DEPENDENCIES_DIR)

# construct search paths
set(OPENAL_PREFIX_PATH ${OPENAL_SOURCE} ${ENV_OPENAL_SOURCE} 
	${RADXML_DEPENDENCIES_DIR} ${ENV_RADXML_DEPENDENCIES_DIR})
create_search_paths(OPENAL)
# redo search if prefix path changed
clear_if_changed(OPENAL_PREFIX_PATH
  OPENAL_LIBRARY_FWK
  OPENAL_LIBRARY_REL
  OPENAL_LIBRARY_DBG
  OPENAL_INCLUDE_DIR
)

set(OPENAL_LIBRARY_NAMES OpenAL32)
get_debug_names(OPENAL_LIBRARY_NAMES)

use_pkgconfig(OPENAL_PKGC OPENAL)

# For OPENAL, prefer static library over framework (important when referencing OPENAL source build)
set(CMAKE_FIND_FRAMEWORK "LAST")

findpkg_framework(OPENAL)

#set (OPENAL_INC_SEARCH_PATH ${OPENAL_INC_SEARCH_PATH} ${OPENAL_SOURCE}/include)
set (OPENAL_LIB_SEARCH_PATH ${OPENAL_LIB_SEARCH_PATH} ${OPENAL_LIBRARY_PATH})

if (OPENAL_USE_BITS STREQUAL "32")
	set (OPENAL_LIB_SEARCH_PATH ${OPENAL_LIB_SEARCH_PATH} ${OPENAL_SOURCE}/libs/Win32)
endif ()

if (OPENAL_USE_BITS STREQUAL "64")
	set (OPENAL_LIB_SEARCH_PATH ${OPENAL_LIB_SEARCH_PATH} ${OPENAL_SOURCE}/libs/Win64)
endif ()

if (APPLE AND OPENAL_SOURCE)
  # OPENAL source build on Mac stores libs in a different location
  # Also this is for static build
  set(OPENAL_LIB_SEARCH_PATH ${OPENAL_LIB_SEARCH_PATH} ${OPENAL_SOURCE}/Mac/XCode-2.2/build)
endif()

find_path(OPENAL_INCLUDE_DIR NAMES al.h HINTS ${OPENAL_INC_SEARCH_PATH} ${OPENAL_PKGC_INCLUDE_DIRS} PATH_SUFFIXES AL)

if (SYMBIAN) 
	set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
	set(CMAKE_PREFIX_PATH ${CMAKE_SYSYEM_OUT_DIR})
	message(STATUS "Lib will be searched in Symbian out dir: ${CMAKE_SYSYEM_OUT_DIR}")
endif (SYMBIAN)

find_library(OPENAL_LIBRARY_REL NAMES ${OPENAL_LIBRARY_NAMES} HINTS ${OPENAL_LIB_SEARCH_PATH} ${OPENAL_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(OPENAL_LIBRARY_DBG NAMES ${OPENAL_LIBRARY_NAMES_DBG} HINTS ${OPENAL_LIB_SEARCH_PATH} ${OPENAL_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

if (SYMBIAN) 
	set(OPENAL_LIBRARY_REL "${FreeImage_LIBRARY_REL} OPENALBase.lib touchfeedback.lib")
	set(CMAKE_PREFIX_PATH ${ORIGINAL_CMAKE_PREFIX_PATH})
endif (SYMBIAN)

make_library_set(OPENAL_LIBRARY)

mark_as_advanced(OPENAL_BINARY_REL OPENAL_BINARY_DBG)

findpkg_finish(OPENAL)

# add parent of OPENAL folder to support OPENAL/OPENAL.h
add_parent_dir(OPENAL_INCLUDE_DIRS OPENAL_INCLUDE_DIR)

# Reset framework finding
set(CMAKE_FIND_FRAMEWORK "FIRST")
