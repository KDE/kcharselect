/******************************************************************/
/* KCharSelectDia - (c) by Reginald Stadlbauer 1999               */
/* Author: Reginald Stadlbauer                                    */
/* E-Mail: reggie@kde.org                                         */
/******************************************************************/

#ifndef kcharselectdia_h
#define kcharselectdia_h

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qclipboard.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qwidget.h>

#include <kbuttonbox.h>
#include <kapp.h>
#include <kcharselect.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

class KCharSelectDia : public QDialog
{
  Q_OBJECT

public:
  KCharSelectDia(QWidget *parent,const char *name,const QChar &_chr,const QString &_font,int _tableNum);

  static bool selectChar(QString &_font,QChar &_chr,int _tableNum);

  int chr() { return vChr; }
  QString font() { return vFont; }

protected:
  void closeEvent(QCloseEvent *) { _exit(); }

  QGridLayout *grid;
  KButtonBox *bbox;
  QPushButton *bAbout,*bExit,*bAdd,*bClip,*bClear;
  KCharSelect *charSelect;
  QLineEdit *lined;

  QChar vChr;
  QString vFont;

protected slots:
  void charChanged(const QChar &_chr);
  void fontSelected(const QString &_font);
  void add()
  { add(vChr); }
  void add(const QChar &_chr);
  void toClip();
  void about();
  void _exit();
  void clear();

};

#endif
