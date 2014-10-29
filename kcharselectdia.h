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

#include <QLineEdit>
#include <QPushButton>

#include <KCharSelect>
#include <KXmlGuiWindow>

/******************************************************************/
/* class KCharSelectDia                                           */
/******************************************************************/
class QGridLayout;
class KCharSelectDia : public KXmlGuiWindow
{
  Q_OBJECT

public:
  KCharSelectDia();

protected:
  void closeEvent(QCloseEvent *event);

  QGridLayout *grid;
  KCharSelect *charSelect;
  QLineEdit   *lined;
  QPushButton *bClip;

  QChar vChr;
  QFont vFont;
  bool _rtl;

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
  void setRtl(bool rtl);
  void lineEditChanged();

};

#endif
