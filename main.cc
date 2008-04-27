/*
 * Copyright 1999  Reginald Stadlbauer <reggie@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kcharselectdia.h"

#include <kapplication.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kglobalsettings.h>
#include <kglobal.h>
#include <klocale.h>

static const char *version = "v1.2";
static const char description[] = I18N_NOOP("KDE character selection utility");
static const char text[] = I18N_NOOP("A wrapper around the KCharSelect widget.");

/*================================================================*/
int main(int argc, char **argv)
{
  KAboutData aboutData( "kcharselect", 0, ki18n("KCharSelect"),
    version, ki18n(description), KAboutData::License_GPL,
    KLocalizedString(), ki18n(text) );

  aboutData.addAuthor(ki18n("Constantin Berzan"), ki18n("Maintainer"), "exit3219@gmail.com");
  aboutData.addAuthor(ki18n("Reginald Stadlbauer"), ki18n("Author"), "reggie@kde.org");

  aboutData.addCredit( ki18n("Daniel Laidig"), ki18n("New GUI, Unicode information, and general improvements"),
        "d.laidig@gmx.de" );
  aboutData.addCredit( ki18n("Nadeem Hasan"), ki18n( "GUI cleanup and fixes" ),
        "nhasan@kde.org" );
  aboutData.addCredit( ki18n("Ryan Cumming"), ki18n( "GUI cleanup and fixes" ),
        "bodnar42@phalynx.dhs.org" );
  aboutData.addCredit(ki18n("Benjamin C. Meyer"),ki18n("XMLUI conversion"),"ben+kcharselect@meyerhome.net");
  aboutData.addCredit( ki18n("Bryce Nesbitt"), ki18n("RTL support") );

  aboutData.setProgramIconName("accessories-character-map");
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
