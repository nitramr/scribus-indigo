Scribus - Open Source Desktop Publishing 
==============================
[![Build Status](https://travis-ci.org/scribusproject/scribus.svg?branch=master)](https://travis-ci.org/scribusproject/scribus) [![Coverity Scan Build Status](https://scan.coverity.com/projects/216/badge.svg)](https://scan.coverity.com/projects/scribus)

## Status
This README.md file is geared toward the Scribus github repository which serves as a mirror for the Scribus Subversion repository `svn://scribus.net/trunk/Scribus`. The Scribus development team prefer svn over git. This mirror is updated manually.

## Contributing
For Bugs and patches: please send them to the [Scribus Bugtracker](http://bugs.scribus.net) to be evaluated. If you do send a PR here, someone will either ask you to do the same or submit the PR as a patch to the bugtracker for you.

## Links
|Resource | Address |  
|:---------|:---------|  
|Website |http://www.scribus.net |  
|Wiki | http://wiki.scribus.net |  
|List | http://lists.scribus.net |  
|Forums | http://forums.scribus.net | 
|WebSVN | http://scribus.net/websvn |
|SVN | svn://scribus.net/trunk/Scribus |  
|IRC | irc://scribus@chat.freenode.net |  


## IndigoDock integration

Following files has been updated to integrate the IndigoDock

ui/scdockpalette.h  
ui/scdockpalette.cpp  
- change type to IndigoPanel  

```C++
ScDockPalette::ScDockPalette( QWidget * parent, const QString& prefsContext, Qt::WindowFlags f)

    : IndigoPanel(prefsContext ),

	palettePrefs(0),
	prefsContextName(QString::null),
	visibleOnStartup(false)
{
...
```

scribus.h  
scribus.cpp  
- add function initIndigoDock()  
- add Panels to Indigodock in initPalettes()  

```C++
void ScribusMainWindow::initIndigoDock()
{

    // install IndigoDockManager
    wdg_indigoDockManager = new IndigoDockManager(this);
    wdg_indigoDockManager->setMinimumPanelSize(QSize(180,100));

    wdg_indigoDock = new IndigoDock();


    // Add IndigoDock
    wdg_indigoDockManager->addIndigoDock(wdg_indigoDock, Qt::RightDockWidgetArea );

}
```
```C++
void ScribusMainWindow::initPalettes()
{


	//CB TODO hide the publicly available members of some palettes
	// these must be filtered too as they take control of the palettes events
	outlinePalette = new OutlinePalette(this);
    outlinePalette->setMainWindow(this);
    wdg_indigoDockManager->addIndigoPanel(wdg_indigoDock, outlinePalette);
...
```
