/******************************************************************/
/* Copyright 1999 Reginald Stadlbauer <reggie@kde.org>            */
/*         RTL support by Bryce Nesbitt                           */
/******************************************************************/

#include "kcharselectdia.moc"

#include <stdlib.h>
#include <kstandardshortcut.h>
#include <kdialog.h>
#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>
#include <kaction.h>
#include <ktoolinvocation.h>
#include <kglobal.h>
#include <kstandardaction.h>
#include <kicon.h>
#include <kactioncollection.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

//==================================================================
KCharSelectDia::KCharSelectDia(QWidget *parent,
			       const QChar &_chr,const QFont &_font,
			       bool direction)
  : KXmlGuiWindow(parent), vChr(_chr), vFont(_font)
{
  QWidget *mainWidget = new QWidget(this);
  setCentralWidget(mainWidget);

  grid = new QGridLayout( mainWidget );
  grid->setMargin( KDialog::marginHint() );
  grid->setSpacing( KDialog::spacingHint() );

  // Add character selection widget from library kdeui
  charSelect = new KCharSelect(mainWidget);
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
            "edit-copy" ), mainWidget );
  bClip->setFixedSize( bClip->sizeHint() );
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClip()));
  grid->addWidget(bClip, 1, 3);

  // Build menu
  KStandardAction::quit( this, SLOT(_exit()), actionCollection() );

  QAction *action = actionCollection()->addAction( "copy_clip" );
  action->setText( i18n("&To Clipboard") );
  action->setIcon( KIcon("edit-copy") );
  connect(action, SIGNAL(triggered(bool)), SLOT(toClip()));
  action->setShortcuts(KStandardShortcut::shortcut(KStandardShortcut::Copy));

  action = actionCollection()->addAction( "copy_utf_8" );
  action->setText( i18n("To Clipboard &UTF-8") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toClipUTF8()));
  action = actionCollection()->addAction( "copy_html" );
  action->setText( i18n("To Clipboard &HTML") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toClipHTML()));

  action = actionCollection()->addAction( "from_clip" );
  action->setText( i18n("&From Clipboard") );
  action->setIcon( KIcon("edit-paste") );
  connect(action, SIGNAL(triggered(bool)), SLOT(fromClip()));
  action->setShortcuts(KStandardShortcut::shortcut(KStandardShortcut::Paste));
  action = actionCollection()->addAction( "from_clip_utf8" );
  action->setText( i18n( "From Clipboard UTF-8") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(fromClipUTF8()));

  i18n("From Clipboard HTML");      // Intended for future use

  action = actionCollection()->addAction( "flip" );
  action->setText( i18n("&Flip") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(flipText()));
  action = actionCollection()->addAction( "alignment" );
  action->setText( i18n("&Alignment") );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toggleEntryDirection()));

  charSelect->setFocus();

  entryDirection = direction;
  if( entryDirection )
    lined->setAlignment( Qt::AlignRight );
  else
    lined->setAlignment( Qt::AlignLeft );

  setupGUI(Keys|Save|Create);
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
  QString str;
  int cursorPos;

  charChanged(_chr);

  str       = lined->text();
  cursorPos = lined->cursorPosition();
  str.insert( cursorPos, vChr );
  lined->setText(str);
  cursorPos++;
  lined->setCursorPosition( cursorPos );
}

//==================================================================
void KCharSelectDia::toClip()
{
  QClipboard *cb = QApplication::clipboard();
  cb->setText(lined->text(),QClipboard::Clipboard);
  cb->setText(lined->text(),QClipboard::Selection);
}

//==================================================================
// UTF-8 is rapidly becoming the favored 8-bit encoding for
// Unicode (iso10646-1).
//
void KCharSelectDia::toClipUTF8()
{
  QClipboard *cb = QApplication::clipboard();
  QString str = lined->text();
  cb->setText(str.toUtf8());
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
  int i;

  input = lined->text();
  for(i=0; i< input.length(); i++ )
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
  QString str = cb->text();

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
  for(i=0; i< input.length(); i++ )
    {
      output.prepend( input.at(i) );
    }
  lined->setText(output);
}

//==================================================================
void KCharSelectDia::toggleEntryDirection()
{
    entryDirection ^= 1;
    if( entryDirection )
        lined->setAlignment( Qt::AlignRight );
    else
        lined->setAlignment( Qt::AlignLeft );
}

//==================================================================
void KCharSelectDia::lineEditChanged()
{
    if( entryDirection )
      {
        if(lined->cursorPosition())
            lined->setCursorPosition( lined->cursorPosition() - 1 );
      }
}

//==================================================================
void KCharSelectDia::_exit()
{
  KSharedConfig::Ptr config = KGlobal::config();
  KConfigGroup gr = config->group("General");

  gr.writeEntry("selectedFont",vFont);
  gr.writeEntry("char",static_cast<int>(vChr.unicode()));
  gr.writeEntry("entryDirection",entryDirection);
  gr.sync();

  delete this;
  exit(0);
}
