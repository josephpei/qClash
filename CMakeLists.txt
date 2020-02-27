cmake_minimum_required(VERSION 3.6.0)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if(DEFINED ENV{QT_ROOT})
  #list(APPEND CMAKE_MODULE_PATH $ENV{QT_ROOT}/lib/cmake)
  #set(Qt5_DIR $ENV{QT_ROOT}/lib/cmake/Qt5)
  set(CMAKE_PREFIX_PATH $ENV{QT_ROOT})
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

project(qClash VERSION 0.1.0)

if (DEFINED VERSION_SUFIX AND NOT "${VERSION_SUFIX}" STREQUAL "")
    set(QCLASH_VERSION_SUFIX "-${VERSION_SUFIX}")
endif()

set(QCLASH_VERSION "${PROJECT_VERSION}${QCLASH_VERSION_SUFIX}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include(CTest)
#enable_testing()

set(QtMinVersion "5.9.0")

find_package(Qt5 ${QtMinVersion} REQUIRED COMPONENTS Core Widgets Network LinguistTools)
if (WIN32)
    find_package(Qt5 ${QT_MIN_VERSION} REQUIRED WinExtras)
endif()

################################################################################
# main library sources
################################################################################
qt5_wrap_ui(${PROJECT_NAME}_FORMS
  src/mainwindow.ui
)
qt5_add_translation(${PROJECT_NAME}_QM_FILES
)
qt5_add_resources(${PROJECT_NAME}_RESOURCES
)
set(${PROJECT_NAME}_SOURCES
  src/mainwindow.h
  src/mainwindow.cpp
)

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_FORMS}
  src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

# auto copy dlls if on windows
if(WIN32)
  find_program(QT5_DEPLOYQT_BIN
          windeployqt
          PATHS "$ENV{QT_ROOT}/bin"
          NO_CMAKE_PATH
          NO_CMAKE_ENVIRONMENT_PATH
          NO_SYSTEM_ENVIRONMENT_PATH
          NO_CMAKE_SYSTEM_PATH)
  if(NOT QT5_DEPLOYQT_BIN)
    message(FATAL_ERROR "No Qt Deploy command found in : $ENV{QT_ROOT}/bin")
  endif()
endif(WIN32)

if(WIN32)
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt)

  if(CMKAE_BUILD_TYPE STREQUAL "Debug")
    set(QCLASH_DEPLOYQT_TYPE "--debug")
  else()
    set(QCLASH_DEPLOYQT_TYPE "--release")
  endif()
  add_custom_command(TARGET ${PROJECT_NAME}
          POST_BUILD
          COMMAND ${QT5_DEPLOYQT_BIN} $<TARGET_FILE:${PROJECT_NAME}> ${QCLASH_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR})
endif(WIN32)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
