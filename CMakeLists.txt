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

project(qClash VERSION 0.1.1)

if (DEFINED VERSION_SUFIX AND NOT "${VERSION_SUFIX}" STREQUAL "")
    set(QCLASH_VERSION_SUFIX "-${VERSION_SUFIX}")
endif()

set(QCLASH_VERSION "${PROJECT_VERSION}${QCLASH_VERSION_SUFIX}")

configure_file(src/BuildConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/BuildConfig.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include(CTest)
enable_testing()

set(QtMinVersion "5.9.0")

find_package(Qt5 ${QtMinVersion} REQUIRED COMPONENTS Core Widgets Network LinguistTools)
find_package(Qt5Test REQUIRED)
if (WIN32)
    find_package(Qt5 ${QT_MIN_VERSION} REQUIRED WinExtras)
endif()

# singleton app
set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")
add_subdirectory(src/third_party/singleapplication SingleApplication)

# yaml cpp
find_package(yaml-cpp CONFIG REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})
link_libraries(${YAML_CPP_LIBRARIES})

################################################################################
# main library sources
################################################################################
qt5_wrap_ui(${PROJECT_NAME}_FORMS
  src/mainwindow.ui
)

qt5_add_resources(${PROJECT_NAME}_RESOURCES
  resources.qrc
)
set(${PROJECT_NAME}_SOURCES
  src/mainwindow.h
  src/mainwindow.cpp
  src/core/configurator.h
  src/core/configurator.cpp
  src/core/clashCore.h
  src/core/clashCore.cpp
  src/core/clashApi.h
  src/core/clashApi.cpp
  src/core/common.h
  src/core/common.cpp
  src/dialogs/subscribeDialog.h
  src/dialogs/subscribeDialog.cpp
  src/dialogs/subscribeNewDialog.cpp
  src/dialogs/subscribeNewDialog.h
  src/dialogs/aboutDialog.h
  src/dialogs/aboutDialog.cpp
  src/utils/httputil.h
  src/utils/httputil.cpp
  src/utils/networkproxy.h
  src/utils/networkproxy.cpp
  src/utils/utility.h
  src/utils/utility.cpp
  src/utils/iconsForkAwesome.h)

set(QCLASH_SRCS
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_QM_FILES}
  ${${PROJECT_NAME}_RESOURCES}
  ${${PROJECT_NAME}_FORMS}
  src/main.cpp)

set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM TRUE)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/lang)
file(GLOB TS_FILES lang/*.ts)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "lang")

option (UPDATE_TRANSLATIONS "Update source translation lang/*.ts files")
set(FILES_TO_TRANSLATE ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_FORMS})
if (UPDATE_TRANSLATIONS)
  qt5_create_translation(${PROJECT_NAME}_QM_FILES ${FILES_TO_TRANSLATE} ${TS_FILES})
else (UPDATE_TRANSLATIONS)
  qt5_add_translation(${PROJECT_NAME}_QM_FILES ${TS_FILES})
endif ()
add_custom_target(translations_target ALL DEPENDS ${${PROJECT_NAME}_QM_FILES})

if(WIN32)
  add_executable(${PROJECT_NAME} ${QCLASH_SRCS} ${CMAKE_SOURCE_DIR}/assets/qclash_icons.qrc
    ${CMAKE_SOURCE_DIR}/assets/qclash_windows_icons.rc)
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
elseif(APPLE)
  set(MACOSX_BUNDLE_EXECUTABLE_NAME "qClash")
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.qclash")
  set(MACOSX_BUNDLE_ICON_FILE "qClash.icns")
  set(MACOSX_BUNDLE_INFO_STRING "Clash GUI Client")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION})
  set(MACOSX_BUNDLE_LONG_VERSION_STRING ${KSNIP_VERSION})

  set(MACOSX_ICON ${CMAKE_SOURCE_DIR}/assets/qClash.icns)
  set_source_files_properties(${MACOSX_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${QCLASH_SRCS}
    ${CMAKE_SOURCE_DIR}/assets/qclash_icons.qrc ${MACOSX_ICON})
else()
  add_executable(${PROJECT_NAME} ${QCLASH_SRCS} ${CMAKE_SOURCE_DIR}/assets/qclash_icons.qrc)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt5::Widgets
  Qt5::Network
  SingleApplication
  yaml-cpp)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/config/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/config )

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
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Network> $<TARGET_FILE_DIR:${PROJECT_NAME}>
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Test> $<TARGET_FILE_DIR:${PROJECT_NAME}> )
endif(WIN32)

function(auto_test subsystem module)
  add_executable(test_${module}
    ${${PROJECT_NAME}_SOURCES}
    test/${subsystem}/${module}_test.cpp)
  target_link_libraries(test_${module} PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Test)
  add_test(
    NAME test_${module}
    COMMAND test_${module})
endfunction()

auto_test(utils http)

if (WIN32)
  set(QCLASH_LANG_INSTALL_DIR "lang")
elseif (APPLE)
  set(QCLASH_LANG_INSTALL_DIR "../Resources")
elseif (UNIX)
  set(CMAKE_INSTALL_PREFIX "/opt/qClash")
  set(QCLASH_LANG_INSTALL_DIR "/opt/qClash/lang")
endif()
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ./config/ DESTINATION ${CMAKE_INSTALL_PREFIX}/config/)
install(FILES ${${PROJECT_NAME}_QM_FILES} DESTINATION ${QCLASH_LANG_INSTALL_DIR})

add_subdirectory(assets)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "josephpei josephpei@gmail.com")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5gui5 libqt5widgets5 libqt5network5 libqt5svg5 libyaml-cpp0.6")
include(CPack)
