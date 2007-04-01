/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/******************************************************************/

#ifndef kcharselectdia_h
#define kcharselectdia_h


#include <QLayout>
#include <KLineEdit>
#include <qclipboard.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QCloseEvent>

#include <kpushbutton.h>
#include <kcharselect.h>
#include <kmainwindow.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

class KCharSelectDia : public KMainWindow
{
  Q_OBJECT

public:
  KCharSelectDia(QWidget *parent,const QChar &_chr,const QFont &_font, bool direction);

  static bool selectChar(QString &_font,QChar &_chr,int _tableNum);

  QChar chr() { return vChr; }
  QFont font() { return vFont; }

protected:
  void closeEvent(QCloseEvent *) { _exit(); }

  QGridLayout *grid;
  KCharSelect *charSelect;
  KLineEdit   *lined;
  KPushButton *bClip;
  KPushButton *bHelp;

  QChar vChr;
  QFont vFont;
  int pointSize;
  bool entryDirection;

protected slots:
  void charChanged(const QChar &_chr);
  void fontSelected(const QFont &_font);
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
