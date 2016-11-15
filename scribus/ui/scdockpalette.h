/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2005 by Craig Bradney                                   *
 *   cbradney@scribus.info                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#ifndef SCDOCKPALETTE_H
#define SCDOCKPALETTE_H

#include <QKeySequence>

#include "scribusapi.h"
#include "indigopanel.h"
class PrefsContext;

class SCRIBUS_API ScDockPalette : public IndigoPanel
{
	Q_OBJECT

	public:
        ScDockPalette(QWidget * parent = NULL, const QString& prefsContext = QString::null);
        ~ScDockPalette() {};
        /** @brief Sample way to grab keystrokes, simply calls superclass at this point */
		//virtual void keyPressEvent(QKeyEvent *keyEvent);
		void startup();
		
	public slots:
		virtual void setPaletteShown(bool);
		virtual void setFontSize();

    private slots:
        void panelClose();
		
	signals:
        /** @brief Let the action for this palette know when something changes and it hasn't caused it */
		void paletteShown(bool);

};
#endif
