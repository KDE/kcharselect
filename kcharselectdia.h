/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/******************************************************************/

#ifndef kcharselectdia_h
#define kcharselectdia_h


#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qclipboard.h>
#include <qstring.h>
#include <qwidget.h>

#include <kapp.h>
#include <kbuttonbox.h>
#include <kcharselect.h>
#include <kdialog.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

class KCharSelectDia : public KDialog
{
  Q_OBJECT

public:
  KCharSelectDia(QWidget *parent,const char *name,const QChar &_chr,const QString &_font,int _tableNum, bool direction);

  static bool selectChar(QString &_font,QChar &_chr,int _tableNum);

  int chr() { return vChr; }
  QString font() { return vFont; }

protected:
  void closeEvent(QCloseEvent *) { _exit(); }

  QGridLayout *grid;
  KCharSelect *charSelect;
  QLineEdit   *lined;
  KButtonBox  *bbox1;
  QPushButton *bClip,*bClear;
  QPushButton *bHelp;

  QChar vChr;
  QString vFont;
  bool  entryDirection;

protected slots:
  void charChanged(const QChar &_chr);
  void fontSelected(const QString &_font);
  void add()
  { add(vChr); }
  void add(const QChar &_chr);
  void toClip();
  void toClipUTF8();
  void toClipHTML();
  void fromClip();
  void fromClipUTF8();
  void flipText();
  void toggleEntryDirection();
  void lineEditChanged(void);
  void about();
  void _exit();
  void clear();
  void help();

};

#endif
