#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find WX
# Once done, this will define
#
#  WX_FOUND - system has WX
#  WX_INCLUDE_DIRS - the WX include directories 
#  WX_LIBRARIES - link these to use WX
#  WX_BINARY_REL / WX_BINARY_DBG - DLL names (windows only)

include(FindPkgMacros)
findpkg_begin(WX)

set (WX_SOURCE $ENV{WX_SOURCE} CACHE PATH "wxWidgets path")
set (WX_LIBRARY_PATH "" CACHE PATH "wxWidgets Library Path")

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(WX_SOURCE)
getenv_path(RADXML_DEPENDENCIES_DIR)

# construct search paths
set(WX_PREFIX_PATH ${WX_SOURCE} ${ENV_WX_SOURCE} 
	${RADXML_DEPENDENCIES_DIR} ${ENV_RADXML_DEPENDENCIES_DIR})
create_search_paths(WX)

# redo search if prefix path changed
clear_if_changed(WX_PREFIX_PATH
  WX_LIBRARY_FWK
  WX_LIBRARY_REL
  WX_LIBRARY_DBG
  WX_INCLUDE_DIR
)

set(WX_LIBRARY_NAMES wxbase28)
get_debug_names(WX_LIBRARY_NAMES)

use_pkgconfig(WX_PKGC WX)

# For WX, prefer static library over framework (important when referencing WX source build)
set(CMAKE_FIND_FRAMEWORK "LAST")

findpkg_framework(WX)

set(WX_LIB_SEARCH_PATH ${WX_LIB_SEARCH_PATH} ${WX_LIBRARY_PATH})

if (APPLE AND WX_SOURCE)
  # WX source build on Mac stores libs in a different location
  # Also this is for static build
  set(WX_LIB_SEARCH_PATH ${WX_LIB_SEARCH_PATH} ${WX_SOURCE}/Mac/XCode-2.2/build)
endif()

find_path(WX_INCLUDE_DIR NAMES wx.h HINTS ${WX_INC_SEARCH_PATH} ${WX_PKGC_INCLUDE_DIRS} PATH_SUFFIXES wx)

if (SYMBIAN) 
	set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
	set(CMAKE_PREFIX_PATH ${CMAKE_SYSYEM_OUT_DIR})
	message(STATUS "Lib will be searched in Symbian out dir: ${CMAKE_SYSYEM_OUT_DIR}")
endif (SYMBIAN)

find_library(WX_LIBRARY_REL NAMES ${WX_LIBRARY_NAMES} HINTS ${WX_LIB_SEARCH_PATH} ${WX_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(WX_LIBRARY_DBG NAMES ${WX_LIBRARY_NAMES_DBG} HINTS ${WX_LIB_SEARCH_PATH} ${WX_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

if (SYMBIAN) 
	set(WX_LIBRARY_REL "${FreeImage_LIBRARY_REL} wxbase28.lib touchfeedback.lib")
	set(CMAKE_PREFIX_PATH ${ORIGINAL_CMAKE_PREFIX_PATH})
endif (SYMBIAN)

make_library_set(WX_LIBRARY)

if (WIN32)
	set(WX_LIB_SEARCH_PATH ${WX_SOURCE}/lib ${ENV_WX_SOURCE}/lib
		${RADXML_DEPENDENCIES_DIR}/bin ${ENV_RADXML_DEPENDENCIES_DIR}/bin ${WX_LIB_SEARCH_PATH})
	set(WX_BIN_SEARCH_PATH ${WX_SOURCE}/bin ${ENV_WX_SOURCE}/bin
		${RADXML_DEPENDENCIES_DIR}/bin ${ENV_RADXML_DEPENDENCIES_DIR}/bin ${WX_BIN_SEARCH_PATH})
	find_file(WX_BINARY_REL NAMES "wxbase28.dll" HINTS ${WX_BIN_SEARCH_PATH}
	  PATH_SUFFIXES "" release relwithdebinfo minsizerel)
	find_file(WX_BINARY_DBG NAMES "wxbase28d.dll" HINTS ${WX_BIN_SEARCH_PATH}
	  PATH_SUFFIXES "" debug )
endif()

if (WIN32)
	set(WX_INCLUDE_DIR ${WX_SOURCE}/include/msvc ${WX_INCLUDE_DIR})
endif ()

string (REPLACE "/include/wx" "/include" WX_INCLUDE_DIR "${WX_INCLUDE_DIR}")

mark_as_advanced(WX_BINARY_REL WX_BINARY_DBG)

findpkg_finish(WX)

# add parent of WX folder to support WX/WX.h
add_parent_dir(WX_INCLUDE_DIRS WX_INCLUDE_DIR)

# Reset framework finding
set(CMAKE_FIND_FRAMEWORK "FIRST")
