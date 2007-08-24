project(kcharselect)

########### next target ###############

set(kcharselect_SRCS kcharselectdia.cc main.cc )

kde4_add_executable(kcharselect ${kcharselect_SRCS})

target_link_libraries(kcharselect  ${KDE4_KDEUI_LIBS} )

install(TARGETS kcharselect  DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES KCharSelect.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES kcharselectui.rc  DESTINATION ${DATA_INSTALL_DIR}/kcharselect)
install( FILES kcharselect.upd  DESTINATION ${DATA_INSTALL_DIR}/kconf_update)

kde4_install_icons( ${ICON_INSTALL_DIR}   )
