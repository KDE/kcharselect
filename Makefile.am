INCLUDES = $(all_includes)
LDADD	= $(LIB_KFILE)
bin_PROGRAMS = kcharselect

kcharselect_SOURCES = kcharselectdia.cc main.cc
kcharselect_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kcharselect_METASOURCES = kcharselectdia.moc

updatedir = $(kde_datadir)/kconf_update
update_DATA = kcharselect.upd

KDE_ICON = kcharselect

xdg_apps_DATA = KCharSelect.desktop

rcdir = $(kde_datadir)/kcharselect
rc_DATA = kcharselectui.rc

messages: rc.cpp
	$(XGETTEXT) *.cc *.cpp -o $(podir)/kcharselect.pot 

