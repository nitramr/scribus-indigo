/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/* This file is part of the KDE project
   Copyright (C) 2001, 2002, 2003 The Karbon Developers

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
/* Adapted for Scribus 22.08.2003 by Franz Schmid */
/* Adapted for Scribus 15.01.2006 by Jean Ghali */

#ifndef __SCPAINTEREXBASE_H__
#define __SCPAINTEREXBASE_H__

#include <QColor>
#include <QFont>
#include <QGlobalStatic>
#include <QList>
#include <QTransform>
#include <QPixmap>
#include <QPointF>
#include <QStack>

#include "scribusapi.h"
#include "scconfig.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "mesh.h"
#include "vgradientex.h"
#include "sccolorshade.h"

class ScImage;
class ScPattern;

class SCRIBUS_API ScPainterExBase
{
public:
	virtual ~ScPainterExBase() {};

	enum FillMode { None, Solid, Gradient, Pattern };
	enum ColorMode { rgbMode = 1, cmykMode = 2 };
	enum ImageMode { cmykImages, rgbImages, rawImages };
	enum Capabilities{ basic = 0, transparencies = 1, patterns = 2 };

	virtual Capabilities capabilities() { return basic; }
	virtual bool hasCapability(Capabilities cap) { return ((m_capabilities & (int) cap) != 0); }

	virtual int supportedColorModes() = 0;
	virtual ColorMode preferredColorMode() = 0;
	virtual ImageMode imageMode() = 0;
	
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void clear() = 0;
	virtual void clear( ScColorShade& ) = 0;

	// matrix manipulation
	virtual void setWorldMatrix( const QTransform & ) = 0;
	virtual const QTransform worldMatrix() = 0;
	virtual void translate( double, double ) = 0;
	virtual void translate( const QPointF& offset ) = 0;
	virtual void rotate( double ) = 0;
	virtual void scale( double, double ) = 0;

	// drawing
	virtual void moveTo( const double &, const double & ) = 0;
	virtual void lineTo( const double &, const double & ) = 0;
	virtual void curveTo( FPoint p1, FPoint p2, FPoint p3 ) = 0;
	virtual void newPath() = 0;
	virtual void fillPath() = 0;
	virtual void strokePath() = 0;
	virtual void setFillRule( bool fillRule ) = 0;
	virtual bool fillRule()  = 0;
	virtual void setFillMode( int fill ) = 0;
	virtual int  fillMode() = 0;
	virtual void setStrokeMode( int stroke ) = 0;
	virtual int  strokeMode() = 0;
	virtual void setGradient( VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew ) = 0;
	virtual void setPattern(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY) = 0;

	virtual void setMaskMode( int mask ) = 0;
	virtual void setGradientMask(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew) = 0;
	virtual void setPatternMask(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY) = 0;

	virtual void set4ColorGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4) = 0;
	virtual void set4ColorColors(const ScColorShade& col1, const ScColorShade& col2, const ScColorShade& col3, const ScColorShade& col4) = 0;
	virtual void setDiamondGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4, FPoint c5) = 0;
	virtual void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<QList<meshPoint> > meshArray) = 0;
	virtual void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<meshGradientPatch> meshPatches) = 0;

	virtual void setClipPath() = 0;

	virtual void drawImage( ScImage *image, ScPainterExBase::ImageMode mode ) = 0;
	virtual void setupPolygon(const FPointArray *points, bool closed = true) = 0;
	virtual void drawPolygon() = 0;
	virtual void drawPolyLine() = 0;
	virtual void drawLine(FPoint start, FPoint end) = 0;
	virtual void drawLine(const QPointF& start, const QPointF& end) = 0;
	virtual void drawRect(double, double, double, double) = 0;

	// pen + brush
	virtual ScColorShade pen() = 0;
	virtual ScColorShade brush() = 0;
	virtual void setPen( const ScColorShade &c ) = 0;
	virtual void setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo ) = 0;
	virtual void setPenOpacity( double op ) = 0;
	virtual void setLineWidth( double w) = 0;
	virtual void setDash(const QVector<double>& array, double ofs) = 0;
	virtual void setBrush( const ScColorShade & ) = 0;
	virtual void setBrushOpacity( double op ) = 0;
	virtual void setOpacity( double op ) = 0;
	virtual void setFont( const QFont &f ) = 0;
	virtual QFont font() = 0;

	// stack management
	virtual void save() = 0;
	virtual void restore() = 0;

	virtual void setRasterOp( int op ) = 0;
	virtual void setBlendModeFill( int blendMode ) = 0;
	virtual void setBlendModeStroke( int blendMode ) = 0;

	VGradientEx  m_fillGradient;
	VGradientEx  m_strokeGradient;
	VGradientEx  m_maskGradient;
	ScPattern*   m_pattern;
	ScPattern*   m_maskPattern;
	QTransform   m_patternTransform;

protected:
	int m_capabilities;
	ScPainterExBase(void);
};

#endif
