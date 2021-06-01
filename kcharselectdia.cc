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

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFontDatabase>
#include <QGridLayout>
#include <QIcon>
#include <QMenu>

#include <KActionCollection>
#include <kbookmarks_version.h>
#include <KBookmarkManager>
#include <KBookmarkMenu>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardShortcut>
#include <KSharedConfig>
#include <KToggleAction>
#include <KBookmarkOwner>

class KCharSelectBookmarkOwner : public KBookmarkOwner
{
public:
    KCharSelectBookmarkOwner(KCharSelectDia *dia)
        : d(dia) { }
    ~KCharSelectBookmarkOwner() { }

    QString currentTitle() const override
    {
        uint ucs4 = d->charSelect->currentCodePoint();
        if (QChar::isPrint(ucs4)) {
            return QString::fromUcs4(&ucs4, 1) + QLatin1Char(' ') + formatCodePoint(ucs4);
        } else {
            return formatCodePoint(ucs4);
        }
    }

    QUrl currentUrl() const override
    {
        return QUrl(formatCodePoint(d->charSelect->currentCodePoint()));
    }

    void openBookmark(const KBookmark &bm, Qt::MouseButtons, Qt::KeyboardModifiers) override
    {
        QString c = bm.url().toString(QUrl::PreferLocalFile | QUrl::RemoveScheme);
        if (c.startsWith(QLatin1String("U+"))) {
            uint uc = c.midRef(2).toUInt(Q_NULLPTR, 16);
            d->charSelect->setCurrentCodePoint(uc);
        }
    }

private:
    static QString formatCodePoint(uint ucs4)
    {
        QString c = QString::number(ucs4, 16).toUpper();
        while (c.length() < 4) {
            c.prepend(QLatin1Char('0'));
        }
        return QStringLiteral("U+") + c;
    }

private:
    KCharSelectDia *d;
};

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

//==================================================================
KCharSelectDia::KCharSelectDia(QWidget *parent)
    : KXmlGuiWindow(parent)
{
  KSharedConfig::Ptr config = KSharedConfig::openConfig();
  KConfigGroup gr = config->group("General");

  vFont = gr.readEntry("selectedFont", QFontDatabase::systemFont(QFontDatabase::GeneralFont));
  vChr = gr.readEntry("char", 33);
  _rtl = gr.readEntry("rtl", false);

  QWidget *mainWidget = new QWidget(this);
  setCentralWidget(mainWidget);

  grid = new QGridLayout( mainWidget );

  // Add character selection widget from library kdeui
  charSelect = new KCharSelect(mainWidget, actionCollection());
  charSelect->setAllPlanesEnabled(true);
  charSelect->setCurrentCodePoint(vChr);
  charSelect->setCurrentFont(vFont);
  charSelect->resize(charSelect->sizeHint());
  connect(charSelect, &KCharSelect::currentCodePointChanged, this, &KCharSelectDia::charChanged);
  connect(charSelect,SIGNAL(codePointSelected(uint)),
         SLOT(add(uint)));

  connect(charSelect, &KCharSelect::currentFontChanged, this, &KCharSelectDia::fontSelected);
  grid->addWidget(charSelect, 0, 0, 1, 4);

  // Build line editor
  lined = new QLineEdit(mainWidget);
  lined->resize(lined->sizeHint());
  lined->setClearButtonEnabled(true);

  lined->setFont( vFont );

  connect(lined, &QLineEdit::textChanged, this, &KCharSelectDia::lineEditChanged);
  grid->addWidget(lined, 1, 0, 1, 3);

  bClip = new QPushButton( i18n( "&To Clipboard" ), mainWidget );
  bClip->setIcon( QIcon::fromTheme( QStringLiteral( "edit-copy" ) ));
  bClip->setFixedSize( bClip->sizeHint() );
  connect(bClip, &QPushButton::clicked, this, &KCharSelectDia::toClip);
  grid->addWidget(bClip, 1, 3);

  // Build menu
  KStandardAction::quit( this, SLOT(close()), actionCollection() );

  QAction *action = actionCollection()->addAction( QStringLiteral( "copy_clip" ) );
  action->setText( i18n("&To Clipboard") );
  action->setIcon( QIcon::fromTheme( QStringLiteral( "edit-copy" )) );
  connect(action, &QAction::triggered, this, &KCharSelectDia::toClip);
  actionCollection()->setDefaultShortcuts(action, KStandardShortcut::shortcut(KStandardShortcut::Copy));

  action = actionCollection()->addAction( QStringLiteral( "copy_utf_8" ) );
  action->setText( i18n("To Clipboard &UTF-8") );
  connect(action, &QAction::triggered, this, &KCharSelectDia::toClipUTF8);
  action = actionCollection()->addAction( QStringLiteral( "copy_html" ) );
  action->setText( i18n("To Clipboard &HTML") );
  connect(action, &QAction::triggered, this, &KCharSelectDia::toClipHTML);

  action = actionCollection()->addAction( QStringLiteral( "from_clip" ) );
  action->setText( i18n("&From Clipboard") );
  action->setIcon( QIcon::fromTheme( QStringLiteral( "edit-paste" )) );
  connect(action, &QAction::triggered, this, &KCharSelectDia::fromClip);
  actionCollection()->setDefaultShortcuts(action, KStandardShortcut::shortcut(KStandardShortcut::Paste));
  action = actionCollection()->addAction( QStringLiteral( "from_clip_utf8" ) );
  action->setText( i18n( "From Clipboard UTF-8") );
  connect(action, &QAction::triggered, this, &KCharSelectDia::fromClipUTF8);

  i18n("From Clipboard HTML");      // Intended for future use

  action = actionCollection()->addAction( QStringLiteral( "flip" ) );
  action->setText( i18n("&Flip Text") );
  connect(action, &QAction::triggered, this, &KCharSelectDia::flipText);

  action = new KToggleAction( i18n("&Reverse Direction"), this );
  action->setChecked(_rtl);
  actionCollection()->addAction( QStringLiteral( "rtl" ), action );
  connect(action, &QAction::toggled, this, &KCharSelectDia::setRtl);

  charSelect->setFocus();

  if(_rtl)
    lined->setAlignment( Qt::AlignRight );
  else
    lined->setAlignment( Qt::AlignLeft );

  QString filename = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kcharselect/bookmarks.xml"));
  if (filename.isEmpty()) {
    filename = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/kcharselect");
    QDir().mkpath(filename);
    filename += QStringLiteral("/bookmarks.xml");
  }
  KBookmarkManager* manager = KBookmarkManager::managerForFile(filename, QStringLiteral("kcharselect"));

  action = actionCollection()->addAction(QStringLiteral("bookmarks"));
  action->setText(i18n("Bookmarks"));
  QMenu *bmmenu = new QMenu(this);
  action->setMenu(bmmenu);

  KBookmarkMenu *bm = new KBookmarkMenu(manager, new KCharSelectBookmarkOwner(this), bmmenu);
  actionCollection()->addActions(bmmenu->actions());

  bm->setParent(this);

  setupGUI(ToolBar|Keys|Save|Create);
}

//==================================================================
void KCharSelectDia::closeEvent(QCloseEvent *event)
{
  KSharedConfig::Ptr config = KSharedConfig::openConfig();
  KConfigGroup gr = config->group("General");

  gr.writeEntry("selectedFont", vFont);
  gr.writeEntry("char", vChr);
  gr.writeEntry("rtl", _rtl);

  KXmlGuiWindow::closeEvent(event);
}

//==================================================================
void KCharSelectDia::charChanged(uint _chr)
{
  vChr = _chr;
}

//==================================================================
void KCharSelectDia::fontSelected(const QFont &_font)
{
  lined->setFont( _font );

  vFont = _font;
}

//==================================================================
void KCharSelectDia::add(uint _chr)
{
  charChanged(_chr);

  QString str = lined->text();
  const int pos = lined->cursorPosition();
  str.insert(pos, QString::fromUcs4(&_chr, 1));
  lined->setText(str);
  lined->setCursorPosition(pos + QString::fromUcs4(&_chr, 1).size());
}

//==================================================================
void KCharSelectDia::toClip()
{
  QString str = lined->text();
  if (str.isEmpty())
    str = QString::fromUcs4(&vChr, 1);
  QClipboard *cb = QApplication::clipboard();
  cb->setText(str,QClipboard::Clipboard);
  cb->setText(str,QClipboard::Selection);
}

//==================================================================
// UTF-8 is rapidly becoming the favored 8-bit encoding for
// Unicode (iso10646-1).
//
void KCharSelectDia::toClipUTF8()
{
  QClipboard *cb = QApplication::clipboard();
  QString str = lined->text();
  if (str.isEmpty())
    str = QString::fromUcs4(&vChr, 1);
  cb->setText(QLatin1String( str.toUtf8() ));
}

//==================================================================
//  Put valid HTML 4.0 into the clipboard.  Valid ISO-8859-1 Latin 1
//  characters are left undisturbed.  Everything else, including the
//  "c0 control characters" often used by Windows, are clipped
//  as a HTML entity.
//
void KCharSelectDia::toClipHTML()
{
  QClipboard *cb = QApplication::clipboard();
  QString input;
  QString html;
  QChar   tempchar;
  int i = 0;

  input = lined->text();
  if (input.isEmpty())
    input = QString::fromUcs4(&vChr, 1);
  const int inputLength = input.length();
  for(i=0; i< inputLength; ++i )
    {
      tempchar = input.at(i);
      if(  tempchar.toLatin1() && ((tempchar.unicode() < 128) || (tempchar.unicode() >= 128+32)) )
        {
          html.append(input.at(i));
        }
      else
        {
          html.append(QString::asprintf("&#x%x;", tempchar.unicode()));
        }
    }
  cb->setText(html);
}

//==================================================================
//
void KCharSelectDia::fromClip()
{
  QClipboard *cb = QApplication::clipboard();
  lined->setText( cb->text() );
}

//==================================================================
// UTF-8 is rapidly becoming the favored 8-bit encoding for
// Unicode (iso10646-1).  This function is handy for decoding
// UTF-8 found in legacy applications, consoles, filenames, webpages,
// etc.
//
void KCharSelectDia::fromClipUTF8()
{
  QClipboard *cb = QApplication::clipboard();
  const QString str = cb->text();

  lined->setText( str.fromUtf8( str.toLatin1() ) );
}

//==================================================================
//  Reverse the text held in the line edit buffer.  This is crucial
//  for dealing with visual vs. logical representations of
//  right to left languages, and handy for working around all
//  manner of legacy character order issues.
//
void KCharSelectDia::flipText()
{
  QString input;
  QString output;
  int i;

  input = lined->text();
  const int nbLength = input.length();
  for(i=0; i< nbLength; ++i )
    {
      output.prepend( input.at(i) );
    }
  lined->setText(output);
}

//==================================================================
void KCharSelectDia::setRtl(bool rtl)
{
    _rtl = rtl;
    if(_rtl)
        lined->setAlignment( Qt::AlignRight );
    else
        lined->setAlignment( Qt::AlignLeft );
}

//==================================================================
void KCharSelectDia::lineEditChanged()
{
    if(_rtl)
      {
        if(lined->cursorPosition())
            lined->setCursorPosition( lined->cursorPosition() - 1 );
      }
}
