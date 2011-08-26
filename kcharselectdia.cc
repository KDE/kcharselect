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

#include <KAction>
#include <KActionCollection>
#include <KApplication>
#include <KConfig>
#include <KDebug>
#include <KDialog>
#include <KGlobal>
#include <KIcon>
#include <KLocale>
#include <KStandardAction>
#include <KStandardShortcut>
#include <KToggleAction>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

//==================================================================
KCharSelectDia::KCharSelectDia()
    : KXmlGuiWindow()
{
  KSharedConfig::Ptr config = KGlobal::config();
  KConfigGroup gr = config->group("General");

  vFont = gr.readEntry("selectedFont", KGlobalSettings::generalFont());
  vChr = QChar(static_cast<unsigned short>(gr.readEntry("char", 33)));
  _rtl = gr.readEntry("rtl", false);

  QWidget *mainWidget = new QWidget(this);
  setCentralWidget(mainWidget);

  grid = new QGridLayout( mainWidget );
  grid->setMargin( KDialog::marginHint() );
  grid->setSpacing( KDialog::spacingHint() );

  // Add character selection widget from library kdeui
  charSelect = new KCharSelect(mainWidget, actionCollection());
  charSelect->setCurrentChar(vChr);
  charSelect->setCurrentFont(vFont);
  charSelect->resize(charSelect->sizeHint());
  connect(charSelect,SIGNAL(currentCharChanged(const QChar &)),
	  SLOT(charChanged(const QChar &)));
  connect(charSelect,SIGNAL(charSelected(const QChar &)),
	  SLOT(add(const QChar &)));
  connect(charSelect,SIGNAL(currentFontChanged(const QFont &)),
	  SLOT(fontSelected(const QFont &)));
  grid->addWidget(charSelect, 0, 0, 1, 4);

  // Build line editor
  lined = new KLineEdit(mainWidget);
  lined->resize(lined->sizeHint());
  lined->setClearButtonShown(true);

  lined->setFont( vFont );

  connect(lined,SIGNAL(textChanged(const QString &)),
	  SLOT(lineEditChanged()));
  grid->addWidget(lined, 1, 0, 1, 3);

  bClip = new KPushButton( KGuiItem( i18n( "&To Clipboard" ),
            QLatin1String( "edit-copy" ) ), mainWidget );
  bClip->setFixedSize( bClip->sizeHint() );
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClip()));
  grid->addWidget(bClip, 1, 3);

  // Build menu
  KStandardAction::quit( this, SLOT(close()), actionCollection() );

  KAction *action = actionCollection()->addAction( QLatin1String( "copy_clip" ) );
  action->setText( i18n("&To Clipboard") );
  action->setIcon( KIcon( QLatin1String( "edit-copy" )) );
  connect(action, SIGNAL(triggered(bool)), SLOT(toClip()));
  action->setShortcuts(KStandardShortcut::shortcut(KStandardShortcut::Copy));

  action = actionCollection()->addAction( QLatin1String( "copy_utf_8" ) );
  action->setText( i18n("To Clipboard &UTF-8") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toClipUTF8()));
  action = actionCollection()->addAction( QLatin1String( "copy_html" ) );
  action->setText( i18n("To Clipboard &HTML") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toClipHTML()));

  action = actionCollection()->addAction( QLatin1String( "from_clip" ) );
  action->setText( i18n("&From Clipboard") );
  action->setIcon( KIcon( QLatin1String( "edit-paste" )) );
  connect(action, SIGNAL(triggered(bool)), SLOT(fromClip()));
  action->setShortcuts(KStandardShortcut::shortcut(KStandardShortcut::Paste));
  action = actionCollection()->addAction( QLatin1String( "from_clip_utf8" ) );
  action->setText( i18n( "From Clipboard UTF-8") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(fromClipUTF8()));

  i18n("From Clipboard HTML");      // Intended for future use

  action = actionCollection()->addAction( QLatin1String( "flip" ) );
  action->setText( i18n("&Flip Text") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(flipText()));

  action = new KToggleAction( i18n("&Reverse Direction"), this );
  action->setChecked(_rtl);
  actionCollection()->addAction( QLatin1String( "rtl" ), action );
  connect(action, SIGNAL(toggled(bool) ), SLOT(setRtl(bool)));

  charSelect->setFocus();

  if(_rtl)
    lined->setAlignment( Qt::AlignRight );
  else
    lined->setAlignment( Qt::AlignLeft );

  setupGUI(ToolBar|Keys|Save|Create);
}

//==================================================================
bool KCharSelectDia::queryExit()
{
  KSharedConfig::Ptr config = KGlobal::config();
  KConfigGroup gr = config->group("General");

  gr.writeEntry("selectedFont", vFont);
  gr.writeEntry("char", static_cast<int>(vChr.unicode()));
  gr.writeEntry("rtl", _rtl);

  return true;
}

//==================================================================
void KCharSelectDia::charChanged(const QChar &_chr)
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
void KCharSelectDia::add(const QChar &_chr)
{
  charChanged(_chr);

  QString str = lined->text();
  const int pos = lined->cursorPosition();
  str.insert(pos, _chr);
  lined->setText(str);
  lined->setCursorPosition(pos + 1);
}

//==================================================================
void KCharSelectDia::toClip()
{
  QString str = lined->text();
  if (str.isEmpty())
    str = vChr;
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
    str = vChr;
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
  QString tempstring;
  QChar   tempchar;
  int i = 0;

  input = lined->text();
  if (input.isEmpty())
    input = vChr;
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
          html.append(tempstring.sprintf("&#x%x;", tempchar.unicode()));
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

#include "kcharselectdia.moc"
