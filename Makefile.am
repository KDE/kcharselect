INCLUDES = $(all_includes)
LDADD	= $(LIB_KFILE)
bin_PROGRAMS = kcharselect

kcharselect_SOURCES = kcharselectdia.cc main.cc
kcharselect_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kcharselect_METASOURCES = kcharselectdia.moc

KDE_ICON = kcharselect

appsdir = $(kde_appsdir)/Utilities/More
apps_DATA = KCharSelect.desktop

install-data-local: uninstall.desktop
	$(mkinstalldirs) $(kde_appsdir)/Utilities
	$(INSTALL_DATA) $(srcdir)/uninstall.desktop $(kde_appsdir)/Utilities/KCharSelect.desktop

messages:
	$(XGETTEXT) $(kcharselect_SOURCES) -o $(podir)/kcharselect.pot 

