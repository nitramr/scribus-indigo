#include "griphandle.h"
#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>


GripHandle::GripHandle(QWidget *parent) :
	QWidget(parent)
{

	modParent = parent;

	resizing = false;
	int_handleWidth = 6;
	int_minWidth = modParent->minimumWidth();
	int_minHeight = modParent->minimumHeight();
	m_orientation = Qt::Vertical;

	this->setObjectName("GripHandle");
	this->installEventFilter(this);
	this->setMouseTracking(true);
	this->setAutoFillBackground(true);
}

/**********************
 *
 * Functions
 *
 * *******************/

void GripHandle::updateParent(int size){

	switch(m_orientation){

	case Qt::Vertical:

		modParent->setFixedHeight(size);
		modParent->setMinimumWidth(int_minWidth);
		modParent->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

		break;
	case Qt::Horizontal:
		modParent->setFixedWidth(size);
		modParent->setMinimumHeight(int_minHeight);
		modParent->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
		break;
	}

	modParent->resize(modParent->sizeHint());
	modParent->adjustSize();

}


/**********************
 *
 * Events
 *
 * *******************/

void GripHandle::mouseMoveEvent(QMouseEvent *event){

	if (event->buttons() == Qt::LeftButton) {

		QPoint point = event->globalPos();

		QPoint delta = point - pnt_relativeOffset;
		pnt_relativeOffset = event->globalPos();

		if (resizing) {

			switch(m_orientation){

			case Qt::Vertical:{

				int _height = modParent->height()+delta.y();
				if(_height <= int_minHeight) _height = int_minHeight;

				updateParent(_height);

				break;
			}
			case Qt::Horizontal:
			{

				int _width = modParent->width()+delta.x();
				if(_width <= int_minWidth) _width = int_minWidth;

				updateParent(_width);

				break;
			}
			}

			emit handleMove();
		}

	}
}


void GripHandle::mousePressEvent(QMouseEvent *event){

	if(event->buttons() == Qt::LeftButton){

		pnt_relativeOffset = event->globalPos();

		resizing = true;
	} else
		resizing = false;
}



void GripHandle::paintEvent(QPaintEvent*){

	QStylePainter painter(this);
	QStyleOption opt(0);
	opt.rect = rect();
	opt.palette = palette();
	if (m_orientation == Qt::Horizontal)
		opt.state = QStyle::State_Horizontal;
	else
		opt.state = QStyle::State_None;
	if (isEnabled())
	   opt.state |= QStyle::State_Enabled;

	painter.drawControl(QStyle::CE_Splitter, opt);

}


/**********************
 *
 * Members
 *
 * *******************/


void GripHandle::setOrientation(Qt::Orientation orientation){
	m_orientation = orientation;

	setCursor(orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);

	setHandleWidth(int_handleWidth);

}



void GripHandle::setHandleWidth(int width){

	int_handleWidth = width;

	switch(m_orientation){

	case Qt::Vertical:
		this->setFixedHeight(int_handleWidth);
		this->setMaximumWidth(QWIDGETSIZE_MAX);
		break;
	case Qt::Horizontal:
		this->setFixedWidth(int_handleWidth);
		this->setMaximumHeight(QWIDGETSIZE_MAX);
		break;
	}

}

int GripHandle::handleWidth(){
	return int_handleWidth;
}

