project(kcharselect)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.2.0")

find_package (ECM 1.3.0 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

add_definitions(-DTRANSLATION_DOMAIN="kcharselect")

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF5 REQUIRED COMPONENTS
    DocTools
    I18n
    WidgetsAddons
    XmlGui
)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

set(kcharselect_SRCS kcharselectdia.cc main.cc )

#kde4_add_app_icon(kcharselect_SRCS "${KDE4_INSTALL_DIR}/share/icons/oxygen/*/apps/accessories-character-map.png")

add_executable(kcharselect ${kcharselect_SRCS})

target_link_libraries(kcharselect  KF5::I18n KF5::WidgetsAddons KF5::XmlGui )

install(TARGETS kcharselect  ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( PROGRAMS KCharSelect.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES kcharselectui.rc  DESTINATION ${KXMLGUI_INSTALL_DIR}/kcharselect)

add_subdirectory(doc)
