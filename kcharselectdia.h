/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/******************************************************************/

#ifndef kcharselectdia_h
#define kcharselectdia_h


#include <qlayout.h>
#include <qlineedit.h>
#include <qclipboard.h>

#include <kpushbutton.h>
#include <kcharselect.h>
#include <kmainwindow.h>

static const char *version = "v1.1"; 

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

class KCharSelectDia : public KMainWindow
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
  KPushButton *bClip,*bClear;
  KPushButton *bHelp;

  QChar vChr;
  QString vFont;
  int pointSize;
  bool entryDirection;

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
  void lineEditChanged();
  void _exit();
  void clear();
  void help();

};

#endif
