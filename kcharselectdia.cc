/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/*         RTL support by Bryce Nesbitt                           */
/******************************************************************/

#include "kcharselectdia.moc"

#include <stdlib.h>

#include <kapplication.h>
#include <kaccel.h>
#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kiconloader.h>
#include <kmenubar.h>


/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

//==================================================================
KCharSelectDia::KCharSelectDia(QWidget *parent,const char *name,
			       const QChar &_chr,const QString &_font,
			       int _tableNum, bool direction)
  : KDialog(parent,name,false), vChr(_chr), vFont(_font)
{
  setCaption(QString::null); // Standard caption

  grid = new QGridLayout( this, 3, 4, marginHint(), spacingHint() );

  // Add character selection widget from library kdeui
  charSelect = new KCharSelect(this,"",vFont,vChr,_tableNum);
  charSelect->resize(charSelect->sizeHint());
  connect(charSelect,SIGNAL(highlighted(const QChar &)),
	  this,SLOT(charChanged(const QChar &)));
  connect(charSelect,SIGNAL(activated(const QChar &)),
	  this,SLOT(add(const QChar &)));
  connect(charSelect,SIGNAL(fontChanged(const QString &)),
	  this,SLOT(fontSelected(const QString &)));
  grid->addMultiCellWidget(charSelect, 0, 0, 0, 3);

  // Build line editor
  lined = new QLineEdit(this);
  lined->resize(lined->sizeHint());

  lined->setFont(QFont(vFont));
  connect(lined,SIGNAL(textChanged(const QString &)),
	  this,SLOT(lineEditChanged(void)));
  grid->addMultiCellWidget(lined, 1, 1, 0, 3);

  // Build some buttons
  bHelp = new KPushButton( KStdGuiItem::help(), this );
  connect(bHelp,SIGNAL(clicked()),this,SLOT(help()));
  bHelp->setFixedSize( bHelp->sizeHint() );
  grid->addWidget( bHelp, 2, 0 );

  QSpacerItem *space = new QSpacerItem( 20, 20, QSizePolicy::Expanding );
  grid->addItem( space, 2, 1 );

  bClear = new KPushButton( KStdGuiItem::clear(), this );
  connect(bClear,SIGNAL(clicked()),this,SLOT(clear()));
  bClear->setFixedSize( bClear->sizeHint() );
  grid->addWidget( bClear, 2, 2 );

  bClip = new KPushButton( KGuiItem( i18n( "&To Clipboard" ), 
            "editcopy" ), this );
  bClip->setFixedSize( bClip->sizeHint() );
  bClip->setDefault(true);
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClip()));
  grid->addWidget( bClip, 2, 3 );
  
  // Build menu
  int id;
  KAccel *keys = new KAccel( this );

  QPopupMenu *file = new QPopupMenu( this );
  id = file->insertItem( SmallIcon( "exit" ), i18n("&Quit"), 
            this, SLOT(_exit())     );
  keys->changeMenuAccel(file, id, KStdAccel::Quit);

  keys->connectItem( KStdAccel::Paste, this, SLOT(fromClip()));
  keys->connectItem( KStdAccel::Copy , this, SLOT(toClip()));
  keys->connectItem( KStdAccel::Quit , this, SLOT(_exit()));
  keys->connectItem( KStdAccel::Help , this, SLOT(help()));

  QPopupMenu *edit = new QPopupMenu( this );
  id = edit->insertItem( SmallIcon( "editcopy" ), i18n("&To Clipboard"), 
            this, SLOT(toClip()) );
  keys->changeMenuAccel(edit, id, KStdAccel::Copy);
  id = edit->insertItem( i18n("To Clipboard &UTF-8"), 
            this, SLOT(toClipUTF8()) );
  id = edit->insertItem( i18n("To Clipboard &HTML"), 
            this, SLOT(toClipHTML()) );
  id = edit->insertItem( SmallIcon( "editpaste"), i18n("From Clipboard"), 
            this, SLOT(fromClip()) );
  keys->changeMenuAccel(edit, id, KStdAccel::Paste);
  id = edit->insertItem( i18n("From Clipboard UTF-8"), 
            this, SLOT(fromClipUTF8()) );
  i18n("From Clipboard HTML");      // Intended for future use
  id = edit->insertSeparator();
  id = edit->insertItem( SmallIcon( "locationbar_erase" ), i18n("&Clear"), 
            this, SLOT(clear())     );
  id = edit->insertItem( i18n("&Flip"), this, SLOT(flipText())  );
  id = edit->insertItem( i18n("&Alignment"), 
            this, SLOT(toggleEntryDirection()) );
  id = edit->insertSeparator();
  id = edit->insertItem( SmallIcon( "kcharselect" ), 
            i18n("A&bout KCharSelect..."), 
            this, SLOT(about())     );

  KMenuBar *menu = new KMenuBar( this );
  menu->insertItem( i18n("&File"), file );
  menu->insertItem( i18n("&Edit"), edit );
  grid->setMenuBar( menu );

  charSelect->setFocus();

  resize(grid->sizeHint());

  entryDirection = direction;
  if( entryDirection )
    lined->setAlignment( Qt::AlignRight );
  else
    lined->setAlignment( Qt::AlignLeft );
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
  lined->setFont(QFont(_font));
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
  cb->setSelectionMode( true );
  cb->setText(lined->text());
  cb->setSelectionMode( false );
  cb->setText(lined->text());
}

//==================================================================
// UTF-8 is rapidly becomming the favored 8-bit encoding for
// Unicode (iso10646-1).
//
void KCharSelectDia::toClipUTF8()
{
  QClipboard *cb = QApplication::clipboard();
  QString str = lined->text();
  cb->setText(str.utf8());
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
  uint i;

  input = lined->text();
  for(i=0; i< input.length(); i++ )
    {
      tempchar = input.at(i);
      if(  tempchar.latin1() && ((tempchar.unicode() < 128) || (tempchar.unicode() >= 128+32)) )
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
// UTF-8 is rapidly becomming the favored 8-bit encoding for
// Unicode (iso10646-1).  This function is handy for decoding
// UTF-8 found in legacy applications, consoles, filenames, webpages,
// etc.
//
void KCharSelectDia::fromClipUTF8()
{
  QClipboard *cb = QApplication::clipboard();
  QString str = cb->text();

  lined->setText( str.fromUtf8( str.latin1() ) );
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
  uint i;
 
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
void KCharSelectDia::lineEditChanged(void)
{
    if( entryDirection )
      {
        if(lined->cursorPosition())
            lined->setCursorPosition( lined->cursorPosition() - 1 );
      }
}

//==================================================================
void KCharSelectDia::about()
{
  KMessageBox::about(0L,i18n("KCharSelect") + "\n" + version +
						 i18n(" Unicode\n"
						 "(c) by Reginald Stadlbauer 1999\n"
						 "Email: reggie@kde.org\n"
						 "Maintainer: bryce@obviously.com\n"
						 "License: GNU GPL"));
}

//==================================================================
void KCharSelectDia::_exit()
{
  KConfig *config = kapp->config();

  config->setGroup("General");
  config->writeEntry("font",vFont);
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
  kapp->invokeHelp();
}
