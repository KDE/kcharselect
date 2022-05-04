cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "22")
set(RELEASE_SERVICE_VERSION_MINOR "07")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kcharselect VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.15.0")
set (KF5_MIN_VERSION "5.89.0")

find_package (ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMInstallIcons)

find_package (Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Bookmarks
    CoreAddons
    Crash
    I18n
    WidgetsAddons
    XmlGui
)

find_package(KF5DocTools ${KF5_MIN_VERSION})
set_package_properties(KF5DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)


ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KCHARSELECT VERSION_HEADER kcharselect_version.h)

add_definitions(
    -DQT_DISABLE_DEPRECATED_BEFORE=0x060000
    -DQT_DEPRECATED_WARNINGS_SINCE=0x060000
    -DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055900
    -DKF_DEPRECATED_WARNINGS_SINCE=0x060000
)

add_executable(kcharselect)

target_sources(kcharselect PRIVATE
    kcharselectdia.cc
    kcharselectdia.h
    main.cc

    kcharselect.qrc
)

target_link_libraries(kcharselect
    KF5::Bookmarks
    KF5::CoreAddons
    KF5::Crash
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui
)

install(TARGETS kcharselect ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES org.kde.kcharselect.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kcharselect.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

if (KF5DocTools_FOUND)
    add_subdirectory(doc)
endif()

ki18n_install(po)

ecm_install_icons(ICONS sc-apps-org.kde.kcharselect.svg DESTINATION ${KDE_INSTALL_ICONDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cc *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
