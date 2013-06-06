#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find CEGUI
# Once done, this will define
#
#  CEGUI_FOUND - system has CEGUI
#  CEGUI_INCLUDE_DIRS - the CEGUI include directories 
#  CEGUI_LIBRARIES - link these to use CEGUI
#  CEGUI_BINARY_REL / CEGUI_BINARY_DBG - DLL names (windows only)

include(FindPkgMacros)
findpkg_begin(CEGUI)

set (CEGUI_SOURCE $ENV{CEGUI_SOURCE} CACHE PATH "CEGUI Source path")
set (CEGUI_SDK $ENV{CEGUI_SDK} CACHE PATH "CEGUI SDK path")

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(CEGUI_SOURCE)
getenv_path(CEGUI_SDK)
getenv_path(RADXML_DEPENDENCIES_DIR)

# construct search paths
set(CEGUI_PREFIX_PATH ${CEGUI_SOURCE} ${ENV_CEGUI_SOURCE} 
	${CEGUI_SDK} ${ENV_CEGUI_SDK} 
	${RADXML_DEPENDENCIES_DIR} ${ENV_RADXML_DEPENDENCIES_DIR})
create_search_paths(CEGUI)
# redo search if prefix path changed
clear_if_changed(CEGUI_PREFIX_PATH
  CEGUI_LIBRARY_FWK
  CEGUI_LIBRARY_REL
  CEGUI_LIBRARY_DBG
  CEGUI_INCLUDE_DIR
)

set(CEGUI_LIBRARY_NAMES CEGUIBase)
get_debug_names(CEGUI_LIBRARY_NAMES)

use_pkgconfig(CEGUI_PKGC CEGUI)

# For CEGUI, prefer static library over framework (important when referencing CEGUI source build)
set(CMAKE_FIND_FRAMEWORK "LAST")

findpkg_framework(CEGUI)

if (CEGUI_SOURCE)
	set(CEGUI_INC_SEARCH_PATH ${CEGUI_INC_SEARCH_PATH} ${CEGUI_SOURCE}/cegui/include)
endif ()

if (APPLE AND CEGUI_SOURCE)
  # CEGUI source build on Mac stores libs in a different location
  # Also this is for static build
  set(CEGUI_LIB_SEARCH_PATH ${CEGUI_LIB_SEARCH_PATH} ${CEGUI_SOURCE}/Mac/XCode-2.2/build)
endif()

find_path(CEGUI_INCLUDE_DIR NAMES CEGUI.h HINTS ${CEGUI_INC_SEARCH_PATH} ${CEGUI_PKGC_INCLUDE_DIRS} PATH_SUFFIXES CEGUI)

if (SYMBIAN) 
	set(ORIGINAL_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
	set(CMAKE_PREFIX_PATH ${CMAKE_SYSYEM_OUT_DIR})
	message(STATUS "Lib will be searched in Symbian out dir: ${CMAKE_SYSYEM_OUT_DIR}")
endif (SYMBIAN)

find_library(CEGUI_LIBRARY_REL NAMES ${CEGUI_LIBRARY_NAMES} HINTS ${CEGUI_LIB_SEARCH_PATH} ${CEGUI_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(CEGUI_LIBRARY_DBG NAMES ${CEGUI_LIBRARY_NAMES_DBG} HINTS ${CEGUI_LIB_SEARCH_PATH} ${CEGUI_PKGC_LIBRARY_DIRS} PATH_SUFFIXES "" debug)

if (SYMBIAN) 
	set(CEGUI_LIBRARY_REL "${FreeImage_LIBRARY_REL} CEGUIBase.lib touchfeedback.lib")
	set(CMAKE_PREFIX_PATH ${ORIGINAL_CMAKE_PREFIX_PATH})
endif (SYMBIAN)

make_library_set(CEGUI_LIBRARY)

if (WIN32)
	set(CEGUI_LIB_SEARCH_PATH ${CEGUI_SOURCE}/lib ${ENV_CEGUI_SOURCE}/lib
		${CEGUI_SDK}/lib ${ENV_CEGUI_SDK}/lib
		${RADXML_DEPENDENCIES_DIR}/bin ${ENV_RADXML_DEPENDENCIES_DIR}/bin ${CEGUI_LIB_SEARCH_PATH})
	set(CEGUI_BIN_SEARCH_PATH ${CEGUI_SOURCE}/bin ${ENV_CEGUI_SOURCE}/bin
		${CEGUI_SDK}/bin ${ENV_CEGUI_SDK}/bin
		${RADXML_DEPENDENCIES_DIR}/bin ${ENV_RADXML_DEPENDENCIES_DIR}/bin ${CEGUI_BIN_SEARCH_PATH})
	find_file(CEGUI_BINARY_REL NAMES "CEGUIBase.dll" HINTS ${CEGUI_BIN_SEARCH_PATH}
	  PATH_SUFFIXES "" release relwithdebinfo minsizerel)
	find_file(CEGUI_BINARY_DBG NAMES "CEGUIBase_d.dll" HINTS ${CEGUI_BIN_SEARCH_PATH}
	  PATH_SUFFIXES "" debug )
endif()

mark_as_advanced(CEGUI_BINARY_REL CEGUI_BINARY_DBG)

findpkg_finish(CEGUI)

# add parent of CEGUI folder to support CEGUI/CEGUI.h
add_parent_dir(CEGUI_INCLUDE_DIRS CEGUI_INCLUDE_DIR)

# Reset framework finding
set(CMAKE_FIND_FRAMEWORK "FIRST")
