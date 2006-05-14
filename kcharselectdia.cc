/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/*         RTL support by Bryce Nesbitt                           */
/******************************************************************/

#include "kcharselectdia.moc"

#include <stdlib.h>
#include <kstdaccel.h>
#include <kdialog.h>
#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>
#include <kaction.h>
//Added by qt3to4:
#include <QGridLayout>
#include <ktoolinvocation.h>
#include <kglobal.h>
#include <kstdaction.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

//==================================================================
KCharSelectDia::KCharSelectDia(QWidget *parent,const char *name,
			       const QChar &_chr,const QString &_font,
			       int _tableNum, bool direction)
  : KMainWindow(parent,name), vChr(_chr), vFont(_font)
{
  setCaption(QString::null); // Standard caption

  QWidget *mainWidget = new QWidget(this);
  setCentralWidget(mainWidget);

  grid = new QGridLayout( mainWidget );
  grid->setMargin( KDialog::marginHint() );
  grid->setSpacing( KDialog::spacingHint() );

  // Add character selection widget from library kdeui
  charSelect = new KCharSelect(mainWidget,vFont,vChr,_tableNum);
  charSelect->resize(charSelect->sizeHint());
  connect(charSelect,SIGNAL(highlighted(const QChar &)),
	  SLOT(charChanged(const QChar &)));
  connect(charSelect,SIGNAL(activated(const QChar &)),
	  SLOT(add(const QChar &)));
  connect(charSelect,SIGNAL(fontChanged(const QString &)),
	  SLOT(fontSelected(const QString &)));
  grid->addMultiCellWidget(charSelect, 0, 0, 0, 3);

  // Build line editor
  lined = new QLineEdit(mainWidget);
  lined->resize(lined->sizeHint());

  QFont font = lined->font();
  font.setFamily( vFont );
  lined->setFont( font );

  connect(lined,SIGNAL(textChanged(const QString &)),
	  SLOT(lineEditChanged()));
  grid->addMultiCellWidget(lined, 1, 1, 0, 3);

  // Build some buttons
  bHelp = new KPushButton( KStdGuiItem::help(), mainWidget );
  connect(bHelp,SIGNAL(clicked()),this,SLOT(help()));
  bHelp->setFixedSize( bHelp->sizeHint() );
  grid->addWidget( bHelp, 2, 0 );

  QSpacerItem *space = new QSpacerItem( 20, 20, QSizePolicy::Expanding );
  grid->addItem( space, 2, 1 );

  bClear = new KPushButton( KStdGuiItem::clear(), mainWidget );
  connect(bClear,SIGNAL(clicked()),this,SLOT(clear()));
  bClear->setFixedSize( bClear->sizeHint() );
  grid->addWidget( bClear, 2, 2 );

  bClip = new KPushButton( KGuiItem( i18n( "&To Clipboard" ),
            "editcopy" ), mainWidget );
  bClip->setFixedSize( bClip->sizeHint() );
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClip()));
  grid->addWidget( bClip, 2, 3 );

  // Build menu
  KStdAction::quit( this, SLOT(_exit()), actionCollection() );

  KAction *action = new KAction(KIcon("editcopy"), i18n("&To Clipboard"), actionCollection(), "copy_clip" );
  connect(action, SIGNAL(triggered(bool)), SLOT(toClip()));
  action->setShortcut(KStdAccel::shortcut(KStdAccel::Copy));

  action = new KAction(i18n("To Clipboard &UTF-8"), actionCollection(), "copy_utf_8" );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toClipUTF8()));
  action = new KAction(i18n("To Clipboard &HTML"), actionCollection(), "copy_html" );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toClipHTML()));

  action = new KAction(KIcon("editpaste"), i18n("&From Clipboard"), actionCollection(), "from_clip" );
  connect(action, SIGNAL(triggered(bool)), SLOT(fromClip()));
  action->setShortcut(KStdAccel::shortcut(KStdAccel::Paste));
  action = new KAction(i18n("From Clipboard UTF-8"), actionCollection(), "from_clip_utf8" );
  connect(action, SIGNAL(triggered(bool) ), SLOT(fromClipUTF8()));

  i18n("From Clipboard HTML");      // Intended for future use

  KStdAction::clear(this, SLOT(clear()), actionCollection(), "clear");
  action = new KAction(i18n("&Flip"), actionCollection(), "flip" );
  connect(action, SIGNAL(triggered(bool) ), SLOT(flipText()));
  action = new KAction(i18n("&Alignment"), actionCollection(), "alignment" );
  connect(action, SIGNAL(triggered(bool) ), SLOT(toggleEntryDirection()));

  charSelect->setFocus();

  entryDirection = direction;
  if( entryDirection )
    lined->setAlignment( Qt::AlignRight );
  else
    lined->setAlignment( Qt::AlignLeft );

  setupGUI(Keys|StatusBar|Save|Create);
}

//==================================================================
void KCharSelectDia::charChanged(const QChar &_chr)
{
  vChr = _chr;
}

//==================================================================
void KCharSelectDia::fontSelected(const QString &_font)
{
  charSelect->setFont(_font);

  QFont font = lined->font();
  font.setFamily( _font );
  lined->setFont( font );

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
#warning "Port kde4/qt4 correct ????"
  //cb->setSelectionMode( true );
  cb->setText(lined->text(),QClipboard::Clipboard);
  //cb->setSelectionMode( false );
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
  KConfig *config = KGlobal::config();

  config->setGroup("General");
  config->writeEntry("selectedFont",vFont);
  config->writeEntry("char",static_cast<int>(vChr.unicode()));
  config->writeEntry("table",charSelect->tableNum());
  config->writeEntry("entryDirection",entryDirection);
  config->sync();

  delete this;
  exit(0);
}

//==================================================================
void KCharSelectDia::clear()
{
  lined->clear();
}

//==================================================================
void KCharSelectDia::help()
{
  KToolInvocation::invokeHelp();
}

