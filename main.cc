/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/******************************************************************/

#include "kcharselectdia.h"

#include <kapp.h>
#include <qstring.h>
#include <kconfig.h>
#include <klocale.h>
#include <kcmdlineargs.h>

static const char *description = 
	I18N_NOOP("KDE character selection utility");

static const char *version = "v0.0.1";


/*================================================================*/
int main(int argc, char **argv)
{
  KCmdLineArgs::init(argc, argv, "kcharselect", description, version);

  KApplication app;

  KConfig *config = kapp->config();

  config->setGroup("General");
  QString font(config->readEntry("font","times"));
  QChar c = QChar(static_cast<unsigned short>(config->readNumEntry("char",33)));
  int tn = config->readNumEntry("table",0);
  
  KCharSelectDia *dia = new KCharSelectDia(0L,"",c,font,tn);

  app.setMainWidget(dia);
  dia->show();

  return app.exec();
}

