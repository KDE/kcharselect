/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/******************************************************************/

#include "kcharselectdia.h"

#include <kapplication.h>
#include <kconfig.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kglobalsettings.h>

static const char description[] = 
	I18N_NOOP("KDE character selection utility");

/*================================================================*/
int main(int argc, char **argv)
{
  KAboutData aboutData( "kcharselect", I18N_NOOP("KCharSelect"),
    version, description, KAboutData::License_GPL,
    "(c) 1999, Reginald Stadlbauer");
  aboutData.addAuthor("Reginald Stadlbauer",0, "reggie@kde.org");
  aboutData.addCredit( "Nadeem Hasan", I18N_NOOP( "GUI cleanup and fixes" ),
        "nhasan@kde.org" );
  aboutData.addCredit( "Ryan Cumming", I18N_NOOP( "GUI cleanup and fixes" ),
        "bodnar42@phalynx.dhs.org" );
  aboutData.addCredit("Benjamin C. Meyer",I18N_NOOP("XMLUI conversion"),"ben+kcharselect@meyerhome.net");
	KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app;

  KConfig *config = kapp->config();

  config->setGroup("General");
  QString font(config->readEntry("selectedFont", KGlobalSettings::generalFont().family()));
  QChar c = QChar(static_cast<unsigned short>(config->readNumEntry("char",33)));
  int tn = config->readNumEntry("table",0);
  bool direction = config->readNumEntry("entryDirection",0);
  
  KCharSelectDia *dia = new KCharSelectDia(0L,"",c,font,tn,direction);

  app.setMainWidget(dia);
  dia->show();

  return app.exec();
}

