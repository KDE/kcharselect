cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "25")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kcharselect VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "6.7.0")
set (KF_MIN_VERSION "6.6.0")

find_package (ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMAddAppIcon)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMDeprecationSettings)

find_package (Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Bookmarks
    CoreAddons
    Crash
    I18n
    WidgetsAddons
    XmlGui
    ConfigWidgets
    IconThemes
)

find_package(KF6DocTools ${KF_MIN_VERSION})
set_package_properties(KF6DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KCHARSELECT VERSION_HEADER kcharselect_version.h)

ecm_set_disabled_deprecation_versions(QT 6.10.0 KF 6.18.0)

add_executable(kcharselect)

ecm_add_app_icon(kcharselect
    ICONS
        pics/16-apps-kcharselect.png
        pics/32-apps-kcharselect.png
        pics/64-apps-kcharselect.png
        pics/128-apps-kcharselect.png
        pics/256-apps-kcharselect.png
        pics/512-apps-kcharselect.png
        pics/1024-apps-kcharselect.png
)

target_sources(kcharselect PRIVATE
    kcharselectdia.cc
    kcharselectdia.h
    main.cc

    kcharselect.qrc
)

target_link_libraries(kcharselect
    KF6::Bookmarks
    KF6::CoreAddons
    KF6::Crash
    KF6::I18n
    KF6::WidgetsAddons
    KF6::XmlGui
    KF6::ConfigWidgets
    KF6::IconThemes
    KF6::BookmarksWidgets
)

install(TARGETS kcharselect ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES org.kde.kcharselect.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kcharselect.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

if (KF6DocTools_FOUND)
    add_subdirectory(doc)
    kdoctools_install(po)
endif()

ki18n_install(po)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cc *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
