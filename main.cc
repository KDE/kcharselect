/******************************************************************/
/* Copyright 1999 Reginald Stadlbauer <reggie@kde.org>            */
/******************************************************************/

#include "kcharselectdia.h"

#include <kapplication.h>
#include <kconfig.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kglobalsettings.h>
#include <kglobal.h>
#include <klocale.h>

static const char *version = "v1.2";
static const char description[] = I18N_NOOP("KDE character selection utility");

/*================================================================*/
int main(int argc, char **argv)
{
  KAboutData aboutData( "kcharselect", 0, ki18n("KCharSelect"),
    version, ki18n(description), KAboutData::License_GPL,
    ki18n("(c) 1999, Reginald Stadlbauer"));
  aboutData.addAuthor(ki18n("Reginald Stadlbauer"),KLocalizedString(), "reggie@kde.org");
  aboutData.addCredit( ki18n("Nadeem Hasan"), ki18n( "GUI cleanup and fixes" ),
        "nhasan@kde.org" );
  aboutData.addCredit( ki18n("Ryan Cumming"), ki18n( "GUI cleanup and fixes" ),
        "bodnar42@phalynx.dhs.org" );
  aboutData.addCredit(ki18n("Benjamin C. Meyer"),ki18n("XMLUI conversion"),"ben+kcharselect@meyerhome.net");
  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app;

  KSharedConfig::Ptr config = KGlobal::config();
  KConfigGroup gr = config->group("General");

  QFont font(gr.readEntry("selectedFont", KGlobalSettings::generalFont()));
  QChar c = QChar(static_cast<unsigned short>(gr.readEntry("char",33)));
  bool direction = gr.readEntry("entryDirection",0);
  
  KCharSelectDia *dia = new KCharSelectDia(0L,c,font,direction);
  dia->show();

  return app.exec();
}
