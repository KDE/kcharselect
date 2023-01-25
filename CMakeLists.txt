cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "23")
set(RELEASE_SERVICE_VERSION_MINOR "03")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kcharselect VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.15.2")
set (KF_MIN_VERSION "5.102.0")

find_package (ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMDeprecationSettings)

find_package (Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_MIN_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
else()
    set(KF_MAJOR_VERSION "5")
endif()

find_package(KF${KF_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Bookmarks
    CoreAddons
    Crash
    I18n
    WidgetsAddons
    XmlGui
)

find_package(KF${KF_MAJOR_VERSION}DocTools ${KF_MIN_VERSION})
set_package_properties(KF5DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)


ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KCHARSELECT VERSION_HEADER kcharselect_version.h)

ecm_set_disabled_deprecation_versions(QT 6.4 KF 5.101.0)

add_executable(kcharselect)

target_sources(kcharselect PRIVATE
    kcharselectdia.cc
    kcharselectdia.h
    main.cc

    kcharselect.qrc
)

target_link_libraries(kcharselect
    KF${KF_MAJOR_VERSION}::Bookmarks
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KF${KF_MAJOR_VERSION}::XmlGui
)

install(TARGETS kcharselect ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES org.kde.kcharselect.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kcharselect.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

if (KF${KF_MAJOR_VERSION}DocTools_FOUND)
    add_subdirectory(doc)
    kdoctools_install(po)
endif()

ki18n_install(po)


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cc *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
