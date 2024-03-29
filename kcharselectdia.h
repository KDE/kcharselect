/*
 * SPDX-FileCopyrightText: 1999 Reginald Stadlbauer <reggie@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KCHARSELECTDIA_H
#define KCHARSELECTDIA_H

#include <QLineEdit>
#include <QPushButton>

#include <KBookmarkManager>
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
    explicit KCharSelectDia(QWidget *parent = nullptr);

    friend class KCharSelectBookmarkOwner;

protected:
    void closeEvent(QCloseEvent *event) override;

    QGridLayout *grid;
    KCharSelect *charSelect;
    QLineEdit *lined;
    QPushButton *bClip;

    char32_t vChr;
    QFont vFont;
    bool _rtl;
    KBookmarkManager *bookmarkManager;

protected Q_SLOTS:
    void charChanged(char32_t _chr);
    void fontSelected(const QFont &_font);
    void add()
    {
        add(vChr);
    }
    void add(char32_t _chr);
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
