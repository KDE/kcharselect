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

#ifndef KCHARSELECTDIA_H
#define KCHARSELECTDIA_H

#include <QLayout>
#include <qclipboard.h>

#include <kpushbutton.h>
#include <kcharselect.h>
#include <KLineEdit>
#include <kxmlguiwindow.h>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/

class KCharSelectDia : public KXmlGuiWindow
{
  Q_OBJECT

public:
  KCharSelectDia(QWidget *parent,const QChar &_chr,const QFont &_font, bool direction);

  static bool selectChar(QString &_font,QChar &_chr,int _tableNum);

  QChar chr() { return vChr; }
  QFont font() { return vFont; }

protected:
  virtual bool queryExit();

  QGridLayout *grid;
  KCharSelect *charSelect;
  KLineEdit   *lined;
  KPushButton *bClip;

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

};

#endif
