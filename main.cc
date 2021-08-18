/*
 * SPDX-FileCopyrightText: 1999 Reginald Stadlbauer <reggie@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <Kdelibs4ConfigMigrator>
#include "kcharselectdia.h"
#include "kcharselect_version.h"

int main(int argc, char **argv)
{
  QApplication app( argc, argv );
  KCrash::initialize();

  KLocalizedString::setApplicationDomain("kcharselect");

  /**
   * enable high dpi support
   */
  app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

  Kdelibs4ConfigMigrator migrate(QStringLiteral("kcharselect"));
  migrate.setConfigFiles(QStringList() << QStringLiteral("kcharselectrc"));
  migrate.setUiFiles(QStringList() << QStringLiteral("kcharselectui.rc"));
  migrate.migrate();

  KAboutData aboutData( QStringLiteral("kcharselect"), i18n("KCharSelect"),
    QStringLiteral(KCHARSELECT_VERSION_STRING), i18n("KDE character selection utility"), KAboutLicense::GPL,
    QString(), i18n("A wrapper around the KCharSelect widget."), QStringLiteral("https://utils.kde.org/projects/kcharselect/") );

  aboutData.addAuthor(i18n("Christoph Feck"), i18n("KF5 port and current maintainer"), QStringLiteral("cfeck@kde.org"));
  aboutData.addAuthor(i18n("Daniel Laidig"), i18n("Author and previous maintainer"), QStringLiteral("d.laidig@gmx.de") );
  aboutData.addAuthor(i18n("Reginald Stadlbauer"), i18n("Author"), QStringLiteral("reggie@kde.org"));

  aboutData.addCredit( i18n("Daniel Laidig"), i18n("New GUI, Unicode information, incremental search,"
        " and general improvements"), QStringLiteral("d.laidig@gmx.de") );
  aboutData.addCredit( i18n("Laurent Montel"), i18n("Porting help"), QStringLiteral("montel@kde.org") );
  aboutData.addCredit( i18n("Constantin Berzan"), i18n("Previous maintainer"), QStringLiteral("exit3219@gmail.com") );
  aboutData.addCredit( i18n("Nadeem Hasan"), i18n( "GUI cleanup and fixes" ),
        QStringLiteral("nhasan@kde.org") );
  aboutData.addCredit( i18n("Ryan Cumming"), i18n( "GUI cleanup and fixes" ),
        QStringLiteral("bodnar42@phalynx.dhs.org") );
  aboutData.addCredit( i18n("Benjamin C. Meyer"), i18n("XMLUI conversion"),
        QStringLiteral("ben+kcharselect@meyerhome.net" ));
  aboutData.addCredit( i18n("Bryce Nesbitt"), i18n("RTL support") );

  KAboutData::setApplicationData( aboutData );
  app.setWindowIcon( QIcon::fromTheme( QStringLiteral( "accessories-character-map" )));


  QCommandLineParser parser;
  aboutData.setupCommandLine(&parser);
  parser.process(app);
  aboutData.processCommandLine(&parser);

  KCharSelectDia *dia = new KCharSelectDia;
  dia->show();

  return app.exec();
}
