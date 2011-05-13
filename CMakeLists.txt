project(kcharselect)

########### next target ###############

find_package(KDE4)
include( KDE4Defaults )
include_directories(${KDE4_INCLUDES})

set(kcharselect_SRCS kcharselectdia.cc main.cc )

kde4_add_app_icon(kcharselect_SRCS "${KDE4_INSTALL_DIR}/share/icons/oxygen/*/apps/accessories-character-map.png")

kde4_add_executable(kcharselect ${kcharselect_SRCS})

target_link_libraries(kcharselect  ${KDE4_KDEUI_LIBS} )

install(TARGETS kcharselect  ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( PROGRAMS KCharSelect.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES kcharselectui.rc  DESTINATION ${DATA_INSTALL_DIR}/kcharselect)
install( FILES kcharselect.upd  DESTINATION ${DATA_INSTALL_DIR}/kconf_update)

kde4_install_icons( ${ICON_INSTALL_DIR}   )
