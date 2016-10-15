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

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QHideEvent>
#include <QPoint>

#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scdockpalette.h"
#include "util.h"

ScDockPalette::ScDockPalette( QWidget * parent, const QString& prefsContext)

    : IndigoPanel(prefsContext )
{

    if (PrefsManager::instance()->appPrefs.uiPrefs.useSmallWidgets)
	{
		setStyleSheet("	QToolButton { margin: 1px; padding: 0px; font-size: 10px; } \
						QToolButton:pressed { padding-top: 2px; padding-left: 2px } \
						QPushButton { margin: 1px; padding: 0px; font-size: 10px; } \
						QPushButton:pressed { padding-top: 2px; padding-left: 2px } \
						QRadioButton, QComboBox, QLineEdit \
							QListView, QLabel { margin:1px; padding: 0px; font-size: 10px; } \
						QCheckBox, QSpinBox, QDoubleSpinBox \
							{ margin:1px; padding: 0px; font-size: 10px; } \
						QTabWidget, QTabBar, QTableView, QGroupBox, QTreeView \
							{ font-size: 10px ; } \
						QToolBox::tab { font-size: 10px; padding: 0px; margin: 0px; } \
			  		");
    }

    connect(PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(setFontSize()));
    connect(this, SIGNAL(panelClosedByButton()), this, SLOT(panelClose()));

}


void ScDockPalette::startup()
{    
	setFontSize();
    if (visibleOnStartup())
	{
        show();
	}
	else
        hide();

    emit paletteShown(visibleOnStartup());
}



void ScDockPalette::setPaletteShown(bool visible)
{
    storeVisibility(visible);

	if (!visible)
        hide();
	else if (!isVisible())
	{
		show();
		activateWindow();
	}
}



void ScDockPalette::setFontSize()
{
	QFont newfont(font());
	newfont.setPointSize(PrefsManager::instance()->appPrefs.uiPrefs.paletteFontSize);
	setFont(newfont);
}



void ScDockPalette::panelClose(){

    emit paletteShown(false);

}

