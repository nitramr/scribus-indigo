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


#include "indigodockmanager.h"
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QMainWindow>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QStyle>

#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"

IndigoDockManager::IndigoDockManager(QMainWindow *parent) : QWidget(parent),
	palettePrefs(0),
	prefsContextName(QString::null)
{

	int_minimumPanelHeight = 50;
	int_minimumPanelWidth = 100;
	int_fadeSpeed = 100;

	setFixedSize(0,0);
	setPrefsContext("IndigoDockManager");
}



void IndigoDockManager::setPrefsContext(QString context)
{
	if (prefsContextName.isEmpty())
	{
		prefsContextName=context;
		if (!prefsContextName.isEmpty())
		{
			palettePrefs = PrefsManager::instance()->prefsFile->getContext(prefsContextName);
		}
		else{
			palettePrefs = NULL;
		}
	}
}



void IndigoDockManager::connectPanel(IndigoPanel * panel){

	// reconnect
	this->connect(panel, SIGNAL(panelClosed(int)), this, SLOT(hideTab(int)));
	this->connect(panel, SIGNAL(panelShown(int)), this, SLOT(showTab(int)));
	this->connect(panel, SIGNAL(panelShown(int)), this, SLOT(scrollToPanel()));
	this->connect(panel, SIGNAL(mouseMove()), this, SLOT(hoverDock()));
	this->connect(panel, SIGNAL(mouseReleased()), this, SLOT(dropPanel()));
	this->connect(panel, SIGNAL(isFloating()), this, SLOT(dragoutPanel()));
	this->connect(panel, SIGNAL(panelClosed(int)), this, SLOT(updateDockSize()));
	this->connect(panel, SIGNAL(handleMove()), this, SLOT(updateDockSize()));


}



void IndigoDockManager::addIndigoDock(IndigoDock *dock, Qt::DockWidgetArea area){

	QMainWindow * main = qobject_cast<QMainWindow *>(parent());

	if(main){


		if(!lst_Docks.contains(dock)){
			lst_Docks.append(dock);



			QString count = QString::number(lst_Docks.size());

			dock->setAccessibleName("IndigoDock"+count);
			dock->setObjectName("IndigoDock"+count);
			dock->setMinimumPanelSize(minimumPanelSize()); // set min size

			main->addDockWidget(area,dock);

		}

	}

}



void IndigoDockManager::removeDock(IndigoDock * dock){

	QMainWindow * main = qobject_cast<QMainWindow *>(parent());

	if(main){

		main->removeDockWidget(dock);
		lst_Docks.removeAll(dock);
		dock->deleteLater();

		return;

	}
}



void IndigoDockManager::removeAllDocks(QList<IndigoPanel *> &lst_outPanel, QList<IndigoDock *> &lst_outDock){

	QMainWindow * main = qobject_cast<QMainWindow *>(parent());

	if(main){

		IndigoDock * dock;
		foreach (dock, lst_Docks) {


			// Make copy of dock
			lst_outDock.append(dock);

			// make copy of panels
			IndigoPanel * panel;
			foreach(panel, dock->getPanels()){
				lst_outPanel.append(panel);
			}

			// clear dock
			dock->clear();

			// remove dock
			main->removeDockWidget(dock);


		}

		lst_Docks.clear();
	}

}


QList<IndigoDock*> IndigoDockManager::indigoDocks() const{

	return lst_Docks;

}


void IndigoDockManager::addIndigoPanel(IndigoPanel * panel, IndigoPanel::IndigoDockState dockState, bool isNewPanel, int tabIndex){

	addIndigoPanel(NULL, panel, dockState, isNewPanel, tabIndex);
}



void IndigoDockManager::addIndigoPanel(IndigoDock * dock, IndigoPanel * panel, IndigoPanel::IndigoDockState dockState, bool isNewPanel, int tabIndex){

	if(!dock) dock = new IndigoDock(this);

	// connect panel signals
	if(isNewPanel)
		connectPanel(panel);

	// add dock if needed
	addIndigoDock(dock);


	// set floating options
	if(dockState == IndigoPanel::Floating){

		dock->setWindowOpacity(0);
		dock->show();
		dock->adjustSize();

		int titleHeight = dock->style()->pixelMetric(QStyle::PM_TitleBarHeight);
		int titleMargin = dock->style()->pixelMetric(QStyle::PM_DockWidgetTitleMargin);

		QPoint pntOffset(0, titleHeight + titleMargin);

		dock->setFloating(true);
		dock->move(panel->pos() - pntOffset);

		QPropertyAnimation *animation;
		animation = new QPropertyAnimation(dock, "windowOpacity");
		animation->setDuration(int_fadeSpeed);
		animation->setStartValue(0);
		animation->setEndValue(1);
		animation->start();

		//dock->setWindowOpacity(1);

	}

	// add panel
	dock->addIndigoPanel(panel, dockState, tabIndex);


}



void IndigoDockManager::dragoutPanel(){

	IndigoPanel *panel = qobject_cast<IndigoPanel *>(sender());

	if (!panel) return;
	panel->setParent(this);
	panel->setDockState(IndigoPanel::Floating);

	qDebug() << "Panel DragOut" << endl;


	removePanel(panel);

}



void IndigoDockManager::removePanel(IndigoPanel * panel){


	int index = panel->Index();

	IndigoDock * dock;
	foreach(dock, lst_Docks){

		if(dock->getPanels().contains(panel)){

			dock->removePanel(index);
			panel->setParent(this);
			panel->show();

			if(dock->getPanels().size() <= 0){

				/* QPropertyAnimation *animation;
				animation = new QPropertyAnimation(dock, "windowOpacity");
				animation->setDuration(int_fadeSpeed);
				animation->setStartValue(1);
				animation->setEndValue(0);
				animation->start();*/

				dock->hide();
				lst_removeDocks.append(dock); // add remove candidate

			}

			return;
		}

	}

}



void IndigoDockManager::hideTab(int index){

	IndigoPanel *pan = qobject_cast<IndigoPanel *>(sender());
	if (!pan) return;

	IndigoDock * dock;
	foreach(dock, lst_Docks){

		if(dock->getPanels().contains(pan)){

			dock->hideTab(index);

			return;
		}

	}

}



void IndigoDockManager::showTab(int index){

	IndigoPanel *pan = qobject_cast<IndigoPanel *>(sender());
	if (!pan) return;


	IndigoDock * dock;
	foreach(dock, lst_Docks){

		if(dock->getPanels().contains(pan)){

			dock->showTab(index);

			return;
		}

	}
}



void IndigoDockManager::updateDockSize(){

	IndigoPanel *pan = qobject_cast<IndigoPanel *>(sender());
	if (!pan) return;


	IndigoDock * dock;
	foreach(dock, lst_Docks){

		if(dock->getPanels().contains(pan)){

			dock->updateMinHeight();

			return;
		}

	}
}



void IndigoDockManager::scrollToPanel(){

	IndigoPanel *pan = qobject_cast<IndigoPanel *>(sender());
	if (!pan) return;

	IndigoDock * dock;
	foreach(dock, lst_Docks){

		if(dock->getPanels().contains(pan)){

			//dock->scrollToPanel(pan->objectName());
			dock->scrollToPanel(pan->Index());

			return;
		}

	}

}



void IndigoDockManager::scrollToPanel(QString name){


	IndigoDock * dock;
	foreach(dock, lst_Docks){

		dock->scrollToPanel(name);

	}

}



void IndigoDockManager::hoverDock(){

	IndigoPanel *pan = qobject_cast<IndigoPanel *>(sender());
	if (!pan) return;


	IndigoDock * dock;
	foreach(dock, lst_Docks){

		if(dock->hoverDock(pan)) break;
	}


}



void IndigoDockManager::dropPanel(){
	IndigoPanel *pan = qobject_cast<IndigoPanel *>(sender());
	if (!pan) return;

	// try to add panel which hover a dock
	IndigoDock * dock;
	foreach(dock, lst_Docks){

		if(dock->dropPanel(pan)) break;

	}


	// create floating dock if panel is not already docked
	if (pan->dockState() == IndigoPanel::Floating){

		addIndigoPanel(pan, pan->dockState(), false);

	}


	// remove remove-candidate docks

	if (lst_removeDocks.size() <= 0) return;

	IndigoDock * remDock;
	foreach(remDock, lst_removeDocks){

		removeDock(remDock);

	}

	lst_removeDocks.clear();

}



void IndigoDockManager::loadWorkspace(){

	if (palettePrefs){

		QString value = palettePrefs->get("IndigoDocks");

		if(value == "") return;


		QList<IndigoDock*> lst_tmpDocks;
		QList<IndigoPanel*> lst_tmpPanels;

		// remove all dock and save copies to restore
		removeAllDocks(lst_tmpPanels, lst_tmpDocks);

		IndigoDock * newDock;
		int d = 0;
		//int p = 0;


		//# Create panels from settings file

		// Value Format: Shap;AlignDistributePalette;PagePalette;Layers;Tree;Inline;Books;Symb;Sclib;undoPalette|PropertiesPalette
		// | = dock divider
		// ; = panel divider


		QStringList valueList = value.split('|');		
		QString s_docks;
		foreach(s_docks, valueList){


			if(d < lst_tmpDocks.size()){
				newDock = lst_tmpDocks.at(d);
			}else newDock = new IndigoDock(this);

			newDock->setRestoreMode(true);

			// add dock property
			addIndigoDock(newDock, Qt::RightDockWidgetArea);

			// get panel names
			QStringList panels = s_docks.split(';');
			QString panelNames;

			foreach(panelNames, panels){

				IndigoPanel * sortPanel;

				foreach(sortPanel, lst_tmpPanels){

					if (sortPanel->objectName() == panelNames){
						//p++;
						newDock->addIndigoPanel(sortPanel);
						//sortPanel->setCaption("Index-Sort"+QString::number(p));
					}
				}
			}


			newDock->setRestoreMode(false);
			newDock->updateMinHeight();

			d++;
		}


		// FIX IT: if an existing panel is missing in prefs it will add in an own floating window frame instead of additional to any dock.

//		//# Create panels which exists but not in prefs file
//		if(0<lst_tmpPanels.size()){

//			if(1 <= lst_tmpDocks.size()){
//				newDock = lst_tmpDocks.at(0);
//			}else newDock = new IndigoDock(this);

//			newDock->setRestoreMode(true);
//			addIndigoDock(newDock, Qt::RightDockWidgetArea);

//			for(int u=0;u<=lst_tmpPanels.size();u++)
//			{
//				IndigoPanel * unsortPanel = lst_tmpPanels.at(u);
//				newDock->addIndigoPanel(unsortPanel);
//				unsortPanel->setCaption("Index" + QString::number(u));
//			}

//			newDock->setRestoreMode(false);
//			newDock->updateMinHeight();
//		}


		lst_tmpDocks.clear();
		lst_tmpPanels.clear();

	}

}



void IndigoDockManager::saveWorkspace(){

	if (palettePrefs){

		QString value = "";

		int d = 0;
		IndigoDock * dock;
		foreach(dock, lst_Docks){

			//            value += dock->wdg_scrollArea_dz->verticalScrollBar()->value() + ";";
			//            value += dock->wdg_scrollArea_dz->horizontalScrollBar()->value() + ";";

			//            value += "{";

			QString dockEnd = "";

			int p = 0;
			IndigoPanel *panel;
			foreach(panel, dock->getPanels()){

				QString panEnd = "";
				if(dock->getPanels().count() > p+1){
					panEnd = ";";
				}

				value += panel->objectName() + panEnd;
				p++;
			}

			//            value += "}";

			if(lst_Docks.count() > d+1){
				dockEnd = "|";
			}

			value += dockEnd;

			d++;

		}

		palettePrefs->set("IndigoDocks", value );



	}

}


/**********************
 *
 * Properties
 *
 * *******************/


void IndigoDockManager::setMinimumPanelHeight(int height){

	int_minimumPanelHeight = height;

}



void IndigoDockManager::setMinimumPanelWidth(int width){

	int_minimumPanelWidth = width;

}



void IndigoDockManager::setMinimumPanelSize(QSize size){

	setMinimumPanelHeight(size.height());
	setMinimumPanelWidth(size.width());

}



QSize IndigoDockManager::minimumPanelSize(){

	return QSize(int_minimumPanelWidth, int_minimumPanelHeight);

}
