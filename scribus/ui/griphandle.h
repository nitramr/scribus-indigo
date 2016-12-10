#ifndef GRIPHANDLE_H
#define GRIPHANDLE_H

#include <QWidget>
#include <QPaintEvent>

class GripHandle : public QWidget
{
	Q_OBJECT
public:
	GripHandle(QWidget* parent);
	void setOrientation(Qt::Orientation orientation);
	void setHandleWidth(int width);
	int handleWidth();

protected:

	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	QWidget * modParent;
	Qt::Orientation m_orientation;
	int int_handleWidth;
	int int_minWidth;
	int int_minHeight;
	bool resizing;
	bool bool_updateParent;
	QPoint pnt_relativeOffset;

	void updateParent(int size);

signals:
	void handleMove();

public slots:
};

#endif // GRIPHANDLE_H
