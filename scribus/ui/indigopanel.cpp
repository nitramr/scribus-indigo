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


#include "indigopanel.h"
#include <QPushButton>
#include "qapplication.h"
#include <QDrag>
#include <QStyle>
#include <QStyleOptionDockWidget>


/*#####################
 #
 #
 # IndigoPanelHandle
 #
 #
 #####################*/

IndigoPanelHandle::IndigoPanelHandle(QWidget *parent) :
    QWidget(parent)
{

    setAutoFillBackground( true );
    setFixedHeight(24);

    str_title = "";

    // Objects
    wdg_btnClose = new QToolButton(this);
    wdg_btnClose->setFixedSize(16,16);
    wdg_btnClose->setAutoRaise(true);
    wdg_btnClose->setFocusPolicy(Qt::NoFocus);

    wdg_btnExpander = new QToolButton(this);
    wdg_btnExpander->setFixedSize(16,16);
    wdg_btnExpander->setAutoRaise(true);
    wdg_btnExpander->setFocusPolicy(Qt::NoFocus);

    QStyleOptionDockWidget opt;
    wdg_btnClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton, &opt, this));
    wdg_btnExpander->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton, &opt, this)); // SP_TitleBarUnshadeButton // http://doc.qt.io/qt-4.8/qstyle.html


    // Main LayoutContainer
    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setMargin(2);
    mainLayout->setSpacing(0);
    mainLayout->addStretch(0);
    mainLayout->addWidget(wdg_btnExpander);
    mainLayout->addWidget(wdg_btnClose);

    setLayout(mainLayout);
    setObjectName("IndigoPanelHandle");

    // Actions
    connect(wdg_btnClose, SIGNAL (clicked()), parent, SLOT (hide()));
    connect(wdg_btnExpander, SIGNAL (clicked()), parent, SLOT (toggleExpander()));

}


/**********************
 *
 * Events
 *
 * *******************/


void IndigoPanelHandle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter p(this);


    int h = this->height();
    int iconY = (h - int_iconSize) / 2;

    QFont font = p.font() ;
    font.setPointSize ( int_fontSize );
    font.setWeight(QFont::DemiBold);
    QFontMetrics fm(this->font());
    int lbl_width = fm.width(str_title);


    if (h > 0)
    {
        int labelX = int_iconSize + 4;

        p.drawPixmap(QRect(0, iconY, int_iconSize,int_iconSize),ico_icon.pixmap(int_iconSize*devicePixelRatio(),int_iconSize*devicePixelRatio()));
        p.setFont(font);
        p.drawText( QRect(labelX, 0, lbl_width, h), Qt::AlignVCenter, str_title );
    }
}


/**********************
 *
 * Properties
 *
 * *******************/


void IndigoPanelHandle::setCaption(QString title, int fontSize){

    str_title = title;
    int_fontSize = fontSize;
}



QString IndigoPanelHandle::Caption(){
    return str_title;
}



void IndigoPanelHandle::setIcon(QIcon icon, int iconSize){

    ico_icon = icon;
    int_iconSize = iconSize;
}



void IndigoPanelHandle::setExpanderState(IndigoExpanderState expanderState){


    QStyleOptionDockWidget opt;
    // SP_TitleBarUnshadeButton // http://doc.qt.io/qt-4.8/qstyle.html

    switch(expanderState){

    case IndigoPanelHandle::Advanced:
        wdg_btnExpander->setIcon(style()->standardIcon(QStyle::SP_TitleBarShadeButton, &opt, this));
        break;

    case IndigoPanelHandle::Normal:
    default:
        wdg_btnExpander->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton, &opt, this));
        break;

    }

}


/*#####################
 #
 #
 # IndigoPanel
 #
 #
 #####################*/


IndigoPanel::IndigoPanel(QString name, QWidget *dock) :
    QFrame(dock)
{
    int int_padding = 5;

    resizing = false;
    int_handleWidth = 6;
    int_minHeight = 50;
    int_minWidth = 80;
    int_dockHeight = 50;
    int_dockWidth = 80;
    col_grip = QColor(this->palette().color(QPalette::Background));
    m_orientation = Qt::Vertical;
    QIcon icon = QIcon();


    wdg_widget = new QWidget();
    wdg_widget->setObjectName("IndigoPanelContentSpacer");

    wdg_handle = new IndigoPanelHandle(this);
    wdg_handle->installEventFilter(this);

    wdg_grip = new QWidget();
    wdg_grip->setAutoFillBackground(true);
    wdg_grip->installEventFilter(this);
    wdg_grip->setMouseTracking(true);

    lyt_content = new QVBoxLayout();
    lyt_content->addWidget(wdg_widget);
    lyt_content->setAlignment(Qt::AlignTop);

    //    lyt_content = new FlowLayout(int_padding);
    //    lyt_content->setSizeConstraint( QLayout::SetNoConstraint );

    lyt_innerArea = new QVBoxLayout();
    lyt_innerArea->setMargin(int_padding);
    lyt_innerArea->addWidget(wdg_handle);
    lyt_innerArea->addLayout(lyt_content, 1);
    lyt_innerArea->setAlignment(Qt::AlignTop);

    // Layouts
    lyt_main = new QBoxLayout(QBoxLayout::TopToBottom);
    lyt_main->setMargin(0);
    lyt_main->setSpacing(0);
    lyt_main->addLayout(lyt_innerArea,1);
    lyt_main->addWidget(wdg_grip);
    lyt_main->setAlignment(Qt::AlignTop);
    setLayout(lyt_main);

    // Extended Properties
    int_index = -1;
    m_state = IndigoPanel::None;
    m_expander = IndigoPanelHandle::Normal;

    // General Properties
    setMouseTracking(true);
    setAutoFillBackground( true );
    setBackgroundRole(QPalette::Background);
    // setMinimumWidth(150);
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setCaption(name);
    setAccessibleName(name);
    setObjectName(name);
    setIcon(icon, 0);
    setHandleWidth(int_handleWidth);

}



IndigoPanel::IndigoPanel(QString name, QIcon icon, int iconSize, QWidget *dock) :
    IndigoPanel(name, dock)
{
    setIcon(icon, iconSize);

}



void IndigoPanel::hide(){

//    switch(dockState()){
//    case IndigoPanel::Docked:
//        setDockState(IndigoPanel::HiddenDocked);
//        emit panelClosed(Index()); // used for tab
//        qDebug() << "emit: panelClosed(...);" << endl;
//        break;
//    default:
//        QFrame::hide();
//        break;

//    }

    setVisible(false);

}



void IndigoPanel::show(){

//    switch(dockState()){
//    case IndigoPanel::HiddenDocked:
//        setDockState(IndigoPanel::Docked);
//        emit panelShown(Index()); // used for tab
//        qDebug() << "emit: panelShow(...);" << endl;
//        break;
//    default:
//        QFrame::show();
//        break;
//    }

//    update();

    setVisible(true);
}



void IndigoPanel::toggleExpander(){


    switch(expanderState()){
    case IndigoPanelHandle::Normal:

        m_expander = IndigoPanelHandle::Advanced;
        wdg_handle->setExpanderState(m_expander);
        emit isAdvanced();
        qDebug() << "emit: isAdvanced();" << endl;
        break;


    case IndigoPanelHandle::Advanced:

        m_expander = IndigoPanelHandle::Normal;
        wdg_handle->setExpanderState(m_expander);
        emit isNormal();
        qDebug() << "emit: isNormal();" << endl;
        break;
    }
}



void IndigoPanel::updateSize(){

    switch(m_orientation){

    case Qt::Vertical:

        setFixedHeight(int_dockHeight);
        setMinimumWidth(minimumResizeWidth());
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

        break;
    case Qt::Horizontal:
        setFixedWidth(int_dockWidth);
        setMinimumHeight(minimumResizeHeight());
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        break;
    }

}


/**********************
 *
 * Events
 *
 * *******************/


bool IndigoPanel::mouseInGrip(QPoint mousePos)
{
    QRect size = QRect(0,0, wdg_grip->width(), wdg_grip->height());

    if(size.contains(mousePos)){
        return true;
    }

    return false;

}



bool IndigoPanel::eventFilter(QObject *object, QEvent *event)
{

    switch( event->type() )
    {
    case QEvent::MouseButtonPress:
    {

        QMouseEvent *me = static_cast<QMouseEvent *>(event);

        if(object == wdg_grip && me->buttons() == Qt::LeftButton){

            // Check if we hit the grip handle
            if (mouseInGrip(me->pos())) {

                pnt_relativeOffset = me->globalPos();

                resizing = true;
            } else {
                resizing = false;
            }
        }


        if(object == wdg_handle && me->buttons() == Qt::LeftButton){
            QPoint point = me->globalPos();
            QPoint xy = this->mapToGlobal(QPoint(0,0));

            pnt_relativeOffset = point - xy;
        }


        break;
    }

    case QEvent::MouseMove:
    {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);

        if(object == wdg_grip){

            switch(m_orientation){

            case Qt::Vertical:
                this->setCursor(Qt::SizeVerCursor);
                break;
            case Qt::Horizontal:
                this->setCursor(Qt::SizeHorCursor);
                break;
            }

            if (me->buttons() == Qt::LeftButton) {

                QPoint point = me->globalPos();

                QPoint delta = point - pnt_relativeOffset;
                pnt_relativeOffset = me->globalPos();

                if (resizing) {

                    switch(m_orientation){

                    case Qt::Vertical:{


                        //setMinimumSize(minimumResizeWidth(), minimumResizeHeight());

                        int _height = height()+delta.y();
                        if(_height <= minimumResizeHeight()) _height = minimumResizeHeight();

                        int_dockHeight = _height;
                        setFixedHeight(_height);
                        setMinimumWidth(minimumResizeWidth());
                        //setMinimumSize(width(), _height);
                        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);


                        break;
                    }
                    case Qt::Horizontal:
                    {

                        //setMinimumSize(minimumResizeWidth(), minimumResizeHeight());

                        int _width = width()+delta.x();
                        if(_width <= minimumResizeWidth()) _width = minimumResizeWidth();

                        int_dockWidth = _width;
                        setFixedWidth(_width);
                        setMinimumHeight(minimumResizeHeight());
                        //setMinimumSize(_width, height());
                        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

                        break;
                    }
                    }

                    emit handleMove();
                }


            }

        }


        if(object == wdg_handle && me->buttons() == Qt::LeftButton){
            // undock Panel if not already undocked
            if(dockState() == IndigoPanel::Docked){
                emit isFloating();

                setDockState(IndigoPanel::Floating);
            }

            //  setWindowState(Qt::WindowActive);

            QPoint point = me->globalPos();
            move(point - pnt_relativeOffset);


            emit mouseMove(); // activate DropZone hover*

        }



        break;
    }

    case QEvent::MouseButtonRelease:
    {
        if(object == wdg_handle){
            if(dockState() == IndigoPanel::Floating){

                emit mouseReleased(); // activate reparenting in DropZone*
                update();
            }
        }

        break;
    }

    case QEvent::MouseButtonDblClick:
        if(object == wdg_handle){
            toggleExpander();
        }
        break;

    case QEvent::Leave:
        if(object == wdg_grip){
            this->setCursor(Qt::ArrowCursor);
        }
        break;
    case QEvent::Paint:

        if(object == wdg_grip){
            QPainter p(wdg_grip);
            p.fillRect(wdg_grip->rect(),col_grip);
        }
        break;

    default:
        break;
    }






    return QFrame::eventFilter(object, event);
}


/**********************
 *
 * Settings
 *
 * *******************/



void IndigoPanel::setCaption(const QString title, int fontSize){
    wdg_handle->setCaption(title, fontSize);
}



QString IndigoPanel::Caption(){
    return wdg_handle->Caption();
}



void IndigoPanel::setWindowTitle(const QString &title){

    QWidget::setWindowTitle(title);
    setCaption(title);

}



QIcon IndigoPanel::Icon(){
    return ico_icon;
}



void IndigoPanel::setIcon(QIcon icon, int iconSize){


    QPixmap pix(iconSize, iconSize);
    pix.fill(QColor(0,0,0,0));

    if(icon.isNull()){
        icon = QIcon(pix);
    }

    ico_icon = icon;
    wdg_handle->setIcon(icon, iconSize); // iconSize will used for icon in caption bar
}



/**
 * @brief IndigoPanel::Index
 * @return index in dropzone
 */
int IndigoPanel::Index(){
    return int_index;
}



/**
 * @brief IndigoPanel::setIndex
 * @param index
 */
void IndigoPanel::setIndex(int index){
    int_index = index;

}



void IndigoPanel::addWidget(QWidget *content){

    if(wdg_widget->objectName() == "IndigoPanelContentSpacer") wdg_widget->deleteLater();

    wdg_widget = content;
    // wdg_widget->setMinimumSize(this->minimumSize());
    // wdg_widget->setMinimumSize(minimumResizeWidth(), minimumResizeHeight());
    wdg_widget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    lyt_content->addWidget(wdg_widget);

}



void IndigoPanel::addWidget(QLayout *content){

    QWidget *widget = new QWidget();
    widget->setLayout(content);

    addWidget(widget);
}



void IndigoPanel::setWidget(QWidget * widget)
{
    addWidget(widget);

}



void IndigoPanel::addStretch(int stretch){

    lyt_content->addStretch(stretch);

}



QWidget *IndigoPanel::widget() const
{
    // QLayout *layout = qobject_cast<QLayout*>(lyt_content);

    //  QLayoutItem *item = lyt_normalArea->itemAt(0);
    // return item == 0 ? 0 : item->widget();

    //return lyt_normalArea->layout()->widget();


    return wdg_widget;// == 0 ? 0 : new QWidget();

}



void IndigoPanel::setOrientation(Qt::Orientation orientation){
    m_orientation = orientation;

    switch(m_orientation){

    case Qt::Vertical:
        lyt_main->setDirection(QBoxLayout::TopToBottom);
        break;
    case Qt::Horizontal:
        lyt_main->setDirection(QBoxLayout::LeftToRight);
        break;
    }


    setHandleWidth(int_handleWidth);

}



void IndigoPanel::setHandleWidth(int width){

    int_handleWidth = width;

    switch(m_orientation){

    case Qt::Vertical:
        wdg_grip->setFixedHeight(int_handleWidth);
        wdg_grip->setMaximumWidth(QWIDGETSIZE_MAX);
        break;
    case Qt::Horizontal:
        wdg_grip->setFixedWidth(int_handleWidth);
        wdg_grip->setMaximumHeight(QWIDGETSIZE_MAX);
        break;
    }

}


void IndigoPanel::setGripColor(QColor color){

    col_grip = color;
    wdg_grip->update();

}



IndigoPanelHandle::IndigoExpanderState IndigoPanel::expanderState(){
    return m_expander;
}



void IndigoPanel::setExpanderState(IndigoPanelHandle::IndigoExpanderState expanderState){
    m_expander = expanderState;

    switch(expanderState){
    case IndigoPanelHandle::Normal:
        wdg_handle->setExpanderState(expanderState);
        emit isNormal();
        qDebug() << "emit: isNormal();" << endl;
        break;

    case IndigoPanelHandle::Advanced:

        wdg_handle->setExpanderState(expanderState);
        emit isAdvanced();
        qDebug() << "emit: isAdvanced();" << endl;
        break;
    }
}



void IndigoPanel::setExpanderState(int expanderState){

    switch(expanderState){
    case 0:{
        setExpanderState(IndigoPanelHandle::Normal);
        break;
    }
    case 1:{
        setExpanderState(IndigoPanelHandle::Advanced);
        break;
    }
    default:{
        setExpanderState(IndigoPanelHandle::Normal);
        break;
    }

    }
}



IndigoPanel::IndigoDockState IndigoPanel::dockState(){
    return m_state;
}



void IndigoPanel::setDockState(IndigoPanel::IndigoDockState state){
    m_state = state;

    switch(state){
    case IndigoPanel::HiddenDocked:
        QFrame::hide();
        break;

    case IndigoPanel::Floating:
        setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint);
        this->adjustSize();
        QFrame::show();
        break;

    case IndigoPanel::Docked:
    case IndigoPanel::None:
    default:
        QFrame::show();
        break;
    }

}



void IndigoPanel::setDockState(int state){

    switch(state){
    case 0:{
        setDockState(IndigoPanel::HiddenDocked);
        break;
    }
    case 1:{
        setDockState(IndigoPanel::Floating);
        break;
    }
    case 2:{
        setDockState(IndigoPanel::Docked);
        break;
    }
    default:{
        setDockState(IndigoPanel::None);
        break;
    }

    }
}



void IndigoPanel::setVisible(bool visible){

QFrame::setVisible(visible);

        if(visible){
//            m_state = IndigoPanel::Docked;
//            emit panelShown(Index()); // used for tab

            switch(dockState()){
            case IndigoPanel::HiddenDocked:
                setDockState(IndigoPanel::Docked);
                //m_state = IndigoPanel::Docked;
                emit panelShown(Index()); // used for tab
                break;
            }
        }else{

//            m_state = IndigoPanel::HiddenDocked;
//            emit panelClosed(Index()); // used for tab

            switch(dockState()){
            case IndigoPanel::Docked:
                setDockState(IndigoPanel::HiddenDocked);
                //m_state = IndigoPanel::HiddenDocked;
                emit panelClosed(Index()); // used for tab
                break;
            }
        }



}



void IndigoPanel::setMinimumResizeHeight(int height){

    int_minHeight = height;

}



void IndigoPanel::setMinimumResizeWidth(int width){

    int_minWidth = width;

}



int IndigoPanel::minimumResizeHeight(){
    return int_minHeight;

}



int IndigoPanel::minimumResizeWidth(){

    return int_minWidth;
}



void IndigoPanel::setDockWidth(int width){

    int_dockWidth = width;
}



void IndigoPanel::setDockHeight(int height){

    int_dockHeight = height;
}

