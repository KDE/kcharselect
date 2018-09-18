cmake_minimum_required (VERSION 3.0 FATAL_ERROR)
set(KCHARSELECT_VERSION "1.13")
project(kcharselect VERSION ${KCHARSELECT_VERSION})

set (QT_MIN_VERSION "5.5.0")
set (KF5_MIN_VERSION "5.25.0")

option(RUN_UNINSTALL "test run uninstalled apps (experimental)" FALSE)

set(ECM_VERSION "5.30.0")
# ECM setup
if (RUN_UNINSTALL)
   set(ECM_VERSION "5.38.0")
endif()

find_package (ECM ${ECM_VERSION} REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

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


configure_file(config-kcharselect.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kcharselect.h)

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
