
INCLUDES = $(all_includes)
LDADD	= $(LIB_KFILE)
bin_PROGRAMS = kcharselect

kcharselect_SOURCES = kcharselectdia.cc main.cc
kcharselect_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kcharselect_METASOURCES = kcharselectdia.moc

KDE_ICON = kcharselect

appsdir = $(kde_appsdir)/Utilities
apps_DATA = KCharSelect.desktop

messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(kcharselect_SOURCES) && mv messages.po $(podir)/kcharselect.pot 

