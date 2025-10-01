/*
 * SPDX-FileCopyrightText: 1999 Reginald Stadlbauer <reggie@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QApplication>
using namespace Qt::Literals::StringLiterals;

#include <QCommandLineParser>

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <KIconTheme>
#include <KStyleManager>

#include "kcharselect_version.h"
#include "kcharselectdia.h"

int main(int argc, char **argv)
{
    KIconTheme::initTheme();

    QApplication app(argc, argv);
    KStyleManager::initStyle();

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("kcharselect"));

    KAboutData aboutData(u"kcharselect"_s,
                         i18n("KCharSelect"),
                         QStringLiteral(KCHARSELECT_VERSION_STRING),
                         i18n("KDE character selection utility"),
                         KAboutLicense::GPL,
                         QString(),
                         i18n("A wrapper around the KCharSelect widget."),
                         u"https://apps.kde.org/kcharselect"_s);

    aboutData.addAuthor(i18n("Christoph Feck"), i18n("KF5 port and current maintainer"), u"cfeck@kde.org"_s);
    aboutData.addAuthor(i18n("Daniel Laidig"), i18n("Author and previous maintainer"), u"d.laidig@gmx.de"_s);
    aboutData.addAuthor(i18n("Reginald Stadlbauer"), i18n("Author"), u"reggie@kde.org"_s);

    aboutData.addCredit(i18n("Daniel Laidig"),
                        i18n("New GUI, Unicode information, incremental search,"
                             " and general improvements"),
                        u"d.laidig@gmx.de"_s);
    aboutData.addCredit(i18n("Laurent Montel"), i18n("Porting help"), u"montel@kde.org"_s);
    aboutData.addCredit(i18n("Constantin Berzan"), i18n("Previous maintainer"), u"exit3219@gmail.com"_s);
    aboutData.addCredit(i18n("Nadeem Hasan"), i18n("GUI cleanup and fixes"), u"nhasan@kde.org"_s);
    aboutData.addCredit(i18n("Ryan Cumming"), i18n("GUI cleanup and fixes"), u"bodnar42@phalynx.dhs.org"_s);
    aboutData.addCredit(i18n("Benjamin C. Meyer"), i18n("XMLUI conversion"), u"ben+kcharselect@meyerhome.net"_s);
    aboutData.addCredit(i18n("Bryce Nesbitt"), i18n("RTL support"));

    KAboutData::setApplicationData(aboutData);
    app.setWindowIcon(QIcon::fromTheme(u"accessories-character-map"_s));
    KCrash::initialize();

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KCharSelectDia *dia = new KCharSelectDia;
    dia->show();

    return app.exec();
}
