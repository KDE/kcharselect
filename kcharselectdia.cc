/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/******************************************************************/

#include "kcharselectdia.h"
#include "kcharselectdia.moc"

#include <klocale.h>
#include <kconfig.h>

#include <qfont.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

//==================================================================
KCharSelectDia::KCharSelectDia(QWidget *parent,const char *name,const QChar &_chr,const QString &_font,int _tableNum)
  : QDialog(parent,name,false), vChr(_chr), vFont(_font)
{
  setCaption("KCharSelect");

  grid = new QGridLayout(this,4,1,5,5);

  charSelect = new KCharSelect(this,"",vFont,vChr,_tableNum);
  charSelect->resize(charSelect->sizeHint());
  grid->addWidget(charSelect,0,0);
  connect(charSelect,SIGNAL(highlighted(const QChar &)),this,SLOT(charChanged(const QChar &)));
  connect(charSelect,SIGNAL(activated(const QChar &)),this,SLOT(add(const QChar &)));
  connect(charSelect,SIGNAL(fontChanged(const QString &)),this,SLOT(fontSelected(const QString &)));

  lined = new QLineEdit(this);
  lined->resize(lined->sizeHint());
  lined->setFont(QFont(vFont));
  grid->addWidget(lined,1,0);

  bbox = new KButtonBox(this,KButtonBox::HORIZONTAL);
  bAdd = bbox->addButton(i18n("&Add"));
  connect(bAdd,SIGNAL(clicked()),this,SLOT(add()));
  bAdd->setDefault(true);
  bClip = bbox->addButton(i18n("&To Clipboard"));
  connect(bClip,SIGNAL(clicked()),this,SLOT(toClip()));
  bbox->addStretch();
  bClear = bbox->addButton(i18n("&Clear"));
  connect(bClear,SIGNAL(clicked()),this,SLOT(clear()));
  bbox->addStretch();
  bAbout = bbox->addButton(i18n("A&bout..."));
  connect(bAbout,SIGNAL(clicked()),this,SLOT(about()));
  bExit = bbox->addButton(i18n("E&xit"));
  connect(bExit,SIGNAL(clicked()),this,SLOT(_exit()));
  bbox->layout();
  grid->addWidget(bbox,2,0);

  grid->addColSpacing(0,charSelect->width());
  grid->addColSpacing(0,1);

  grid->addRowSpacing(0,charSelect->height());
  grid->addRowSpacing(1,lined->height());
  grid->addRowSpacing(2,bbox->height());
  grid->setRowStretch(0,1);
  grid->activate();

  charSelect->setFocus();

  resize(grid->sizeHint());
}

//==================================================================
bool KCharSelectDia::selectChar(QString &_font,QChar &_chr,int _tableNum)
{
  bool res = false;

  KCharSelectDia *dlg = new KCharSelectDia(0L,"",_chr,_font,_tableNum);
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
  
  QString str = lined->text();
  str += vChr;
  lined->setText(str);
}

//==================================================================
void KCharSelectDia::toClip()
{
  QClipboard *cb = QApplication::clipboard();
  cb->setText(lined->text());
}

//==================================================================
void KCharSelectDia::about()
{
  QMessageBox::information(0L,i18n("About"),i18n("KCharSelect 0.1.0\n\n"
						 "(c) by Reginald Stadlbauer 1999\n\n"
						 "E-Mail: reggie@kde.org\n\n"
						 "License: GNU GPL"),i18n("OK"));
}

//==================================================================
void KCharSelectDia::_exit()
{
  KConfig *config = kapp->getConfig();

  config->setGroup("General");
  config->writeEntry("font",vFont);
  config->writeEntry("char",static_cast<int>(vChr.unicode()));
  config->writeEntry("table",charSelect->tableNum());
  config->sync();

  delete this;
  exit(0);
}

//==================================================================
void KCharSelectDia::clear()
{
  lined->setText("");
}

