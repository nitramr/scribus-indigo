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

#ifndef __SCPAINTEREX_PS2_H__
#define __SCPAINTEREX_PS2_H__

// libart wrapper

#include <QGlobalStatic>
#include <QColor>
#include <QFont>
#include <QTransform>
#include <QList>
#include <QPixmap>
#include <QStack>
#include <QTextStream>

#include "scribusapi.h"
#include "scconfig.h"
#include "colormgmt/sccolormgmtengine.h"
#include "fpoint.h"
#include "fpointarray.h"
#include "vgradient.h"
#include "scpainterexbase.h"

class ScribusDoc;

class ScPs2OutputParams
{
public:
	ScPs2OutputParams(ScribusDoc* doc = NULL);
	ScPainterExBase::ColorMode colorMode;
	ScribusDoc* document;
	bool reloadImages;
	int resolution;
	bool useProfiles;
	bool toGray;
	bool mirrorH;
	bool mirrorV;
	QString outputProfile;
	ScColorProfile   hProfile;
	ScColorTransform rgbToOutputColorTransform;
	ScColorTransform rgbToOutputImageTransform;
	ScColorTransform cmykToOutputColorTransform;
	ScColorTransform cmykToOutputImageTransform;
};

class SCRIBUS_API ScPainterEx_Ps2 : public ScPainterExBase
{
public:

	ScPainterEx_Ps2( QIODevice* iodev, QRect& rect, ScPs2OutputParams& options );
	virtual ~ScPainterEx_Ps2();

	virtual int supportedColorModes() { return (int) (rgbMode|cmykMode); }
	virtual ColorMode preferredColorMode() { return m_colorMode; }
	virtual void setPreferredColorMode(ScPainterEx_Ps2::ColorMode colorMode) { m_colorMode = colorMode; }
	virtual ImageMode imageMode();

	virtual void begin();
	virtual void end();
	virtual void clear();
	virtual void clear( ScColorShade & );

	// matrix manipulation
	virtual void setWorldMatrix( const QTransform & );
	virtual const QTransform worldMatrix();
	virtual void translate( double, double );
	virtual void translate( const QPointF& offset );
	virtual void rotate( double );
	virtual void scale( double, double );

	// drawing
	virtual void moveTo( const double &, const double & );
	virtual void lineTo( const double &, const double & );
	virtual void curveTo( FPoint p1, FPoint p2, FPoint p3 );
	virtual void newPath();
	virtual void closePath();
	virtual void fillPath();
	virtual void strokePath();
	virtual void setFillRule( bool fillRule );
	virtual bool fillRule() { return m_fillRule; }
	virtual void setFillMode( int fill );
	virtual int  fillMode() { return m_fillMode; }
	virtual void setStrokeMode( int fill );
	virtual int  strokeMode() { return m_strokeMode; }
	virtual void setGradient( VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew);
	virtual void setPattern(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY);

	virtual void setMaskMode( int mask ) {};
	virtual void setGradientMask(VGradientEx::Type mode, FPoint orig, FPoint vec, FPoint foc, double scale, double skew) {};
	virtual void setPatternMask(ScPattern *pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation, double skewX, double skewY, bool mirrorX, bool mirrorY) {};

	virtual void set4ColorGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4) {};
	virtual void set4ColorColors(const ScColorShade& col1, const ScColorShade& col2, const ScColorShade& col3, const ScColorShade& col4) {};
	virtual void setDiamondGeometry(FPoint p1, FPoint p2, FPoint p3, FPoint p4, FPoint c1, FPoint c2, FPoint c3, FPoint c4, FPoint c5) {};
	virtual void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<QList<meshPoint> > meshArray) {};
	virtual void setMeshGradient(FPoint p1, FPoint p2, FPoint p3, FPoint p4, QList<meshGradientPatch> meshPatches) {};

	virtual void setClipPath();

	virtual void drawImage( ScImage *image, ScPainterExBase::ImageMode mode );
	virtual void drawRGBImage_ps2( ScImage *image );
	virtual void drawCMYKImage_ps2( ScImage *image );

	virtual bool hasAlphaChannel( ScImage* image );

	virtual void setupPolygon(const FPointArray *points, bool closed = true);
	virtual void drawPolygon();
	virtual void drawPolyLine();
	virtual void drawLine(FPoint start, FPoint end);
	virtual void drawLine(const QPointF& start, const QPointF& end);
	virtual void drawRect(double, double, double, double);

	// pen + brush
	virtual ScColorShade pen();
	virtual ScColorShade brush();
	virtual void setPen( const ScColorShade & );
	virtual void setPen( const ScColorShade &c, double w, Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo );
	virtual void setPenOpacity( double op );
	virtual void setLineWidth( double w);
	virtual void setDash(const QVector<double>& array, double ofs);
	virtual void setBrush( const ScColorShade & );
	virtual void setBrushOpacity( double op );
	virtual void setOpacity( double op );
	virtual void setFont( const QFont &f );
	virtual QFont font();

	// stack management
	virtual void save();
	virtual void restore();

	virtual void setRasterOp( int op );
	virtual void setBlendModeFill( int blendMode ) {};
	virtual void setBlendModeStroke( int blendMode ) {};

protected:

	virtual void writeRGBImageToStream_Ascii85 ( ScImage* image );
	virtual void writeRGBImageToStream_AsciiHex ( ScImage* image );
	virtual void writeCMYKImageToStream_Ascii85( ScImage* image );
	virtual void writeCMYKImageToStream_AsciiHex( ScImage* image );
	virtual void writeMaskToStream ( QImage* image ); 

private:

	typedef enum {
		Ascii85Encoding,
		AsciiHexEncoding
	} AsciiEncoding;

	/* Output stream */
	QTextStream   m_stream;
	ColorMode     m_colorMode;
	AsciiEncoding m_encoding; 

	/* Path bounding boxes*/
	QRect m_pathBBox;
	QRect m_clipBBox;

	/* General output options */
	ScPs2OutputParams m_options;

	/* Basic drawing functions*/
	void drawVPath( int mode );
	void drawGradient( VGradientEx& gradient );
	void drawLinearGradient( VGradientEx& gradient, const QRect& rect );
	void drawLinearGradient_RGB( VGradientEx& gradient, const QRect& rect );
	void drawLinearGradient_CMYK( VGradientEx& gradient, const QRect& rect );
	void drawCircularGradient( VGradientEx& gradient, const QRect& rect );
	void drawCircularGradient_RGB( VGradientEx& gradient, const QRect& rect );
	void drawCircularGradient_CMYK( VGradientEx& gradient, const QRect& rect );
	void getPathBoundingBox(const FPointArray* points, QRect& r );
	void transformPoint( const FPoint& in, FPoint& out );
	void transformPoints( const FPoint* in, FPoint* out, uint length );

	void putColor( ScColorShade& colorShade, bool doFill );

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_x;
	unsigned int m_y;
	QTransform m_pageTrans;
	QTransform m_matrix;
	QFont m_font;
/* Filling */
	ScColorShade m_fillColor;
	double m_fillTrans;
	bool   m_fillRule;
	int    m_fillMode;			// 0 = none, 1 = solid, 2 = gradient

	double m_patternScaleX;
	double m_patternScaleY;
	double m_patternOffsetX;
	double m_patternOffsetY;
	double m_patternRotation;
	double m_patternSkewX;
	double m_patternSkewY;
	bool   m_patternMirrorX;
	bool   m_patternMirrorY;

/* Stroking */
	ScColorShade m_strokeColor;
	double m_strokeTrans;
	double m_lineWidth;
	int    m_strokeMode;				// 0 = none, 1 = solid, 2 = gradient 3 = pattern
/* Line End Style */
	Qt::PenCapStyle m_lineEnd;
/* Line Join Style */
	Qt::PenJoinStyle m_lineJoin;
/* The Dash Array */
	QVector<double> m_array;
	double m_offset;
/* Transformation Stack */
	QStack<QTransform> m_stack;

/* Some data to describe state of drawing */	
	bool m_pathIsClosed;
	bool m_drawingClosedPath;
/* Drawing surface dimensions */
	double m_deviceDimX;
	double m_deviceDimY;
/* Device resolutions */
	double m_deviceResX;
	double m_deviceResY;

/* Color conversion function */
	void   transformImage( QImage* image, uchar* data, int scan);
};

#endif
