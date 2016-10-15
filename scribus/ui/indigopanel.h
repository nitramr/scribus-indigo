/*******************************************************
 *
 * Copyright (C) 2016  Martin Reininger
 *
 * This file is part of IndigoDock.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *******************************************************/


#ifndef INDIGOPANEL_H
#define INDIGOPANEL_H

#include <QWidget>
#include <QtGui>
#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QToolButton>
#include <QScrollArea>
#include <QString>
#include <QIcon>
#include <QVBoxLayout>
#include <QBoxLayout>
//#include "uiwidgets/flowlayout.h"

//#include <QCloseEvent>
//#include <QHideEvent>
//#include <QKeySequence>
//#include <QShowEvent>

//#include "scribusapi.h"
class PrefsContext;

class IndigoPanelHandle : public QWidget
{
        Q_OBJECT
public:

    enum IndigoExpanderState{
        Normal = 0,
        Advanced = 1
    };

    IndigoPanelHandle(QWidget* parent);
    QString Caption();
    void setCaption(QString title, int fontSize);
    void setIcon(QIcon icon, int iconSize);
    void setExpanderState(IndigoExpanderState expanderState);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QIcon ico_icon;
    int int_iconSize;
    QString str_title;
    int int_fontSize;
    QToolButton * wdg_btnClose;
    QToolButton * wdg_btnExpander;


signals:

public slots:
};


/**********************************************************/

class IndigoPanel : public QFrame
{
    Q_OBJECT

public:
    enum IndigoDockState{

        HiddenDocked = 0,
        Floating = 1,
        Docked = 2,
        None = 3

    };



    IndigoPanel(QString name, QWidget* dock = 0);
    IndigoPanel(QString name, QIcon icon, int iconSize = 22, QWidget* dock = 0);
    void addWidget(QWidget *content);
    void addWidget(QLayout *content);
    void setWidget(QWidget * widget);
    void addStretch(int stretch = 0);

    QWidget *widget() const;

    IndigoPanelHandle * wdg_handle;

    QString Caption();
    void setCaption(QString title = "", int fontSize = 10);

    QIcon Icon();
    void setIcon(QIcon icon, int iconSize = 22);

    int Index();
    void setIndex(int index);

    IndigoDockState dockState();
    void setDockState(IndigoDockState state);
    void setDockState(int state);

    IndigoPanelHandle::IndigoExpanderState expanderState();
    void setExpanderState(IndigoPanelHandle::IndigoExpanderState expanderState);
    void setExpanderState(int expanderState);

    void setOrientation(Qt::Orientation orientation);
    void setHandleWidth(int width);
    void setGripColor(QColor color);
    void setWindowTitle(const QString &title);
    void setVisible(bool visible);

    void setMinimumResizeHeight(int height);
    void setMinimumResizeWidth(int width);
    int minimumResizeHeight();
    int minimumResizeWidth();

    void setDockWidth(int width);
    void setDockHeight(int height);
    int dockHeight();
    int dockWidth();

    void updateSize();

    bool visibleOnStartup();

protected:
    bool eventFilter(QObject *object, QEvent *e);
    bool mouseInGrip(QPoint mousePos);

    /** @brief Set the Preferences context to be used for storage of startup visibility and position and size */
    virtual void setPrefsContext(QString context);
    void storeSize();
    void storeVisibility(bool);

    PrefsContext* palettePrefs;
    QString prefsContextName;
    bool bool_visibleOnStartup;


private:
    QWidget * wdg_grip;
    QColor col_grip;
    QVBoxLayout *lyt_innerArea;
   // FlowLayout * lyt_normalArea;
    QBoxLayout *lyt_main;
    QVBoxLayout * lyt_content;
    QScrollArea * wdg_scrollArea;

    QWidget * wdg_widget;

    QPoint pnt_relativeOffset;
    QIcon ico_icon;
    int int_index;
    IndigoDockState m_state;
    IndigoPanelHandle::IndigoExpanderState m_expander;

    Qt::Orientation m_orientation;
    int int_handleWidth;
    bool resizing;
    QPoint oldPos;

    int int_minWidth;
    int int_minHeight;

    int int_dockWidth;
    int int_dockHeight;


signals:
    void mouseReleased();
    void mouseMove();
    void isFloating();
    void panelClosedByButton();
    void panelClosed(int index);
    void panelShown(int index);
    void isAdvanced();
    void isNormal();
    void handleMove();

public slots:
    void show();
    void hide();    
    void toggleExpander();

private slots:
    void clickCloseButton();
};

#endif // INDIGOPANEL_H
