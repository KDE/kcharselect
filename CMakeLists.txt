kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kcharselect_SRCS kcharselectdia.cc main.cc )

kde4_automoc(${kcharselect_SRCS})

kde4_add_executable(kcharselect ${kcharselect_SRCS})

target_link_libraries(kcharselect  ${QT_AND_KDECORE_LIBS} )

install_targets(/bin kcharselect )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES KCharSelect.desktop )
install_files( ${DATA_INSTALL_DIR}/kcharselect FILES kcharselectui.rc )
install_files( ${DATA_INSTALL_DIR}/kconf_update FILES kcharselect.upd )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



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
