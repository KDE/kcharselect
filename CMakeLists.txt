project(kcharselect)




########### next target ###############

set(kcharselect_SRCS kcharselectdia.cc main.cc )

kde4_automoc(${kcharselect_SRCS})

kde4_add_executable(kcharselect ${kcharselect_SRCS})

target_link_libraries(kcharselect  ${KDE4_KDEUI_LIBS} )

install(TARGETS kcharselect  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES KCharSelect.desktop  DESTINATION ${XDG_APPS_DIR})
install( FILES kcharselectui.rc  DESTINATION ${DATA_INSTALL_DIR}/kcharselect)
install( FILES kcharselect.upd  DESTINATION ${DATA_INSTALL_DIR}/kconf_update)

kde4_install_icons( ${ICON_INSTALL_DIR}   )




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#LDADD	= $(LIB_KFILE)
#bin_PROGRAMS = kcharselect
#
#kcharselect_SOURCES = kcharselectdia.cc main.cc
#kcharselect_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kcharselect_METASOURCES = kcharselectdia.moc
#
#updatedir = $(kde_datadir)/kconf_update
#update_DATA = kcharselect.upd
#
#KDE_ICON = kcharselect
#
#xdg_apps_DATA = KCharSelect.desktop
#
#rcdir = $(kde_datadir)/kcharselect
#rc_DATA = kcharselectui.rc
#
#messages: rc.cpp
#	$(XGETTEXT) *.cc *.cpp -o $(podir)/kcharselect.pot 
#
