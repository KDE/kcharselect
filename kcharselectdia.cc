/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/*         RTL support by Bryce Nesbitt                           */
/******************************************************************/
/*
    TODO:   It would be nice if hitting the spacebar inserted
    space, rather than another copy of the current character
*/

#include "kcharselectdia.h"
#include "kcharselectdia.moc"

#include <stdlib.h>

#include <qfont.h>

#include <kconfig.h>
#include <kdialog.h>
#include <klocale.h>
#include <kmessagebox.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

//==================================================================
KCharSelectDia::KCharSelectDia(QWidget *parent,const char *name,
			       const QChar &_chr,const QString &_font,
			       int _tableNum, bool direction)
  : KDialog(parent,name,false), vChr(_chr), vFont(_font)
{
  //setCaption("KCharSelect");
  setCaption(QString::null); // Standard caption

  grid = new QGridLayout( this, 4, 1, marginHint(), spacingHint() );

  charSelect = new KCharSelect(this,"",vFont,vChr,_tableNum);
  charSelect->resize(charSelect->sizeHint());
  grid->addWidget(charSelect,0,0);
  connect(charSelect,SIGNAL(highlighted(const QChar &)),
	  this,SLOT(charChanged(const QChar &)));
  connect(charSelect,SIGNAL(activated(const QChar &)),
	  this,SLOT(add(const QChar &)));
  connect(charSelect,SIGNAL(fontChanged(const QString &)),
	  this,SLOT(fontSelected(const QString &)));

  lined = new QLineEdit(this);
  lined->resize(lined->sizeHint());
  lined->setFont(QFont(vFont));
  grid->addWidget(lined,1,0);
  connect(lined,SIGNAL(textChanged(const QString &)),
	  this,SLOT(lineEditChanged(void)));

  bbox1 = new KButtonBox(this,Horizontal);
//bAdd = bbox1->addButton(i18n("&Add"));
//connect(bAdd,SIGNAL(clicked()),this,SLOT(add()));
//bbox1->addStretch();
//bAdd->setDefault(true);
  bClear = bbox1->addButton(i18n("&Clear"));
  connect(bClear,SIGNAL(clicked()),this,SLOT(clear()));
  bbox1->addStretch();
  bAbout = bbox1->addButton(i18n("&Flip"));
  connect(bAbout,SIGNAL(clicked()),this,SLOT(flipText()));
  bbox1->addStretch();
  bAbout = bbox1->addButton(i18n("Force &Direction"));
  connect(bAbout,SIGNAL(clicked()),this,SLOT(toggleEntryDirection()));
  bbox1->addStretch();
  bAbout = bbox1->addButton(i18n("A&bout..."));
  connect(bAbout,SIGNAL(clicked()),this,SLOT(about()));
//bbox1->addStretch();
  bbox1->layout();
  grid->addWidget(bbox1,2,0);

  bbox2 = new KButtonBox(this,Horizontal);
  bClip = bbox2->addButton(i18n("&To Clipboard"));
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClip()));
  bClip->setDefault(true);
  bbox2->addStretch();
  bClip = bbox2->addButton(i18n("To Clipboard &UTF-8"));
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClipUTF8()));
  bbox2->addStretch();
  bClip = bbox2->addButton(i18n("To Clipboard &HTML"));
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClipHTML()));
  bbox2->addStretch();
  bExit = bbox2->addButton(i18n("E&xit"));
  connect(bExit,SIGNAL(clicked()),this,SLOT(_exit()));
//bbox2->addStretch();
  bbox2->layout(); 
  grid->addWidget(bbox2,3,0);

  grid->addColSpacing(0,charSelect->width());
  grid->addColSpacing(0,1);

  grid->addRowSpacing(0,charSelect->height());
  grid->addRowSpacing(1,lined->height());
  grid->addRowSpacing(2,bbox1->height());
  grid->addRowSpacing(3,bbox2->height());
  grid->setRowStretch(0,1);

  charSelect->setFocus();

  resize(grid->sizeHint());

  entryDirection = direction;
  if( entryDirection )
    lined->setAlignment( Qt::AlignRight );
  else
    lined->setAlignment( Qt::AlignLeft );
}

//==================================================================
bool KCharSelectDia::selectChar(QString &_font,QChar &_chr,int _tableNum)
{
  bool res = false;

  KCharSelectDia *dlg = new KCharSelectDia(0L,"",_chr,_font,_tableNum,0);
  if (dlg->exec() == QDialog::Accepted)
    {
      _font = dlg->font();
      _chr = dlg->chr();
      res = true;
    }

  delete dlg;

  return res;
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
  charChanged(_chr);

  QString str   = lined->text();
  int cursorPos = lined->cursorPosition();

  str.insert( cursorPos, vChr );
  lined->setText(str);
  if( entryDirection == 0 )
    cursorPos++;
  lined->setCursorPosition( cursorPos );
}

//==================================================================
void KCharSelectDia::toClip()
{
  QClipboard *cb = QApplication::clipboard();
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
//  Since we add characters one-by-one, we bypass the usual processing
//  of right-to-left languages (assuming, that is, the version of QT
//  even supports that).  So we can manually force which end of the
//  string to append to.  Even with QT 3.0, this will still be
//  important for creating script for use in "visual" order (e.g. Hebrew
//  as usually used with iso-8859-8 encoding).
//
void KCharSelectDia::toggleEntryDirection()
{   
    entryDirection ^= 1;
    if( entryDirection )
      {
        lined->setAlignment( Qt::AlignRight );
        lined->setCursorPosition( 0 );
      }
    else
      {
        lined->setAlignment( Qt::AlignLeft );
      }
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
  KMessageBox::about(0L,i18n("KCharSelect\n\n"
						 "v0.5 Unicode\n\n"
						 "(c) by Reginald Stadlbauer 1999\n"
						 "E-Mail: reggie@kde.org\n"
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
