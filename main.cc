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

#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KLocalizedString>
#include <Kdelibs4ConfigMigrator>
#include "kcharselectdia.h"

int main(int argc, char **argv)
{
  QApplication app( argc, argv );

  /**
   * enable high dpi support
   */
  app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

  Kdelibs4ConfigMigrator migrate(QLatin1String("kcharselect"));
  migrate.setConfigFiles(QStringList() << QLatin1String("kcharselectrc"));
  migrate.setUiFiles(QStringList() << QLatin1String("kcharselectui.rc"));
  migrate.migrate();

  KAboutData aboutData( "kcharselect", i18n("KCharSelect"),
    "1.12", i18n("KDE character selection utility"), KAboutLicense::GPL,
    QString(), i18n("A wrapper around the KCharSelect widget."), "http://utils.kde.org/projects/kcharselect" );

  aboutData.addAuthor(i18n("Daniel Laidig"), i18n("Author and maintainer"), "d.laidig@gmx.de" );
  aboutData.addAuthor(i18n("Reginald Stadlbauer"), i18n("Author"), "reggie@kde.org");

  aboutData.addCredit( i18n("Daniel Laidig"), i18n("New GUI, Unicode information, incremental search,"
        " and general improvements"), "d.laidig@gmx.de" );
  aboutData.addCredit( i18n("Constantin Berzan"), i18n("Previous maintainer"), "exit3219@gmail.com" );
  aboutData.addCredit( i18n("Nadeem Hasan"), i18n( "GUI cleanup and fixes" ),
        "nhasan@kde.org" );
  aboutData.addCredit( i18n("Ryan Cumming"), i18n( "GUI cleanup and fixes" ),
        "bodnar42@phalynx.dhs.org" );
  aboutData.addCredit( i18n("Benjamin C. Meyer"), i18n("XMLUI conversion"),
        "ben+kcharselect@meyerhome.net" );
  aboutData.addCredit( i18n("Bryce Nesbitt"), i18n("RTL support") );

  KAboutData::setApplicationData( aboutData );
  app.setWindowIcon( QIcon::fromTheme( QLatin1String( "accessories-character-map" )));


  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  aboutData.setupCommandLine(&parser);
  parser.process(app);
  aboutData.processCommandLine(&parser);

  KCharSelectDia *dia = new KCharSelectDia;
  dia->show();

  return app.exec();
}
