
INCLUDES = $(all_includes)
LDADD	= $(LIB_KFILE)
bin_PROGRAMS = kcharselect

kcharselect_SOURCES = kcharselectdia.cc main.cc
kcharselect_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kcharselect_METASOURCES = kcharselectdia.moc

kde_icon_DATA = kcharselect.xpm

uninstall-local:
	rm -f $(kde_appsdir)/Utilities/KCharSelect.desktop

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Utilities
	$(INSTALL_DATA) $(srcdir)/KCharSelect.desktop $(kde_appsdir)/Utilities


messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(kcharselect_SOURCES) && mv messages.po ../po/kcharselect.pot 
