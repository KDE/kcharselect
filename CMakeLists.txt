cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(KDE_APPLICATIONS_VERSION_MAJOR "19")
set(KDE_APPLICATIONS_VERSION_MINOR "07")
set(KDE_APPLICATIONS_VERSION_MICRO "80")
set(KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

project(kcharselect VERSION ${KDE_APPLICATIONS_VERSION})

set (QT_MIN_VERSION "5.9.0")
set (KF5_MIN_VERSION "5.46.0")

find_package (ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMSetupVersion)

find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Bookmarks
    Crash
    DocTools
    I18n
    WidgetsAddons
    XmlGui
)


ecm_setup_version(${KDE_APPLICATIONS_VERSION} VARIABLE_PREFIX KCHARSELECT VERSION_HEADER kcharselect_version.h)

add_definitions( -DQT_DISABLE_DEPRECATED_BEFORE=0x060000 )
if (KF5Config_VERSION VERSION_GREATER "5.56.0")
        add_definitions(-DQT_NO_FOREACH)
        MESSAGE(STATUS "compile without foreach")
endif()


set(kcharselect_SRCS kcharselectdia.cc main.cc )
qt5_add_resources(kcharselect_SRCS kcharselect.qrc)


#kde4_add_app_icon(kcharselect_SRCS "${KDE4_INSTALL_DIR}/share/icons/oxygen/*/apps/accessories-character-map.png")

add_executable(kcharselect ${kcharselect_SRCS})

target_link_libraries(kcharselect
    KF5::Bookmarks
    KF5::Crash
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui
)

install(TARGETS kcharselect  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( PROGRAMS org.kde.kcharselect.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kcharselect.appdata.xml DESTINATION ${CMAKE_INSTALL_METAINFODIR})

add_subdirectory(doc)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
