/***************************************************************************
    copyright            : (C) 2016 by Martin Reininger
    email                : martinreininger@gmx.net
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QCursor>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QDomDocument>
#include <QStyleFactory>

#include "thememanager.h"
#include "prefsmanager.h"
#include "scpaths.h"

ThemeManager* ThemeManager::m_instance = 0;

ThemeManager::ThemeManager(QObject *parent)
 : QObject(parent)
{
}

ThemeManager::~ThemeManager()
{
}

ThemeManager* ThemeManager::instance()
{
    if (m_instance == 0)
        m_instance = new ThemeManager();

    return m_instance;
}

void ThemeManager::deleteInstance()
{
    if (m_instance)
        delete m_instance;
    m_instance = 0;
}

bool ThemeManager::setup()
{
    if (!initThemes())
    {
        qDebug()<<"No themes found :(";
        return false;
    }
    return true;
}


bool ThemeManager::initThemes()
{
    readThemeConfigFiles();
    if (m_themeSets.isEmpty() || m_activeSetBasename.isEmpty())
        return false;
    return true;
}


void ThemeManager::addSystemThemes(){

    QStringList sysThemes = QStyleFactory::keys();

    QString theme;
    foreach(theme, sysThemes){

        ScIconSetData isd;

        isd.path="";
        isd.license="System";
        isd.activeversion="1.5.3";
        isd.nameTranslations.insert("en_US",theme);
        isd.baseName=theme;

        m_themeSets.insert(isd.baseName, isd);
        m_backupSetBasename=m_activeSetBasename;
        m_backupSetVersion=m_backupSetVersion;
        m_activeSetBasename=isd.baseName;
        m_activeSetVersion=isd.activeversion;

    }

}


void ThemeManager::readThemeConfigFiles()
{
    addSystemThemes();

    QString baseThemeDir(ScPaths::instance().libDir());
    QStringList locations;
    locations<<baseThemeDir;
    QStringList configNames;
    for ( QStringList::Iterator it = locations.begin(); it != locations.end(); ++it )
    {
        QFileInfo libDir(*it);
        if (!libDir.exists())
            continue;
        QDir id(*it, "*.xml", QDir::Name, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks);
        if (!id.exists() || (id.count() == 0))
            continue;
        for (uint i = 0; i < id.count(); ++i)
        {
            QFileInfo file(*it + id[i]);
            //qDebug()<<file.absoluteFilePath();
            QFile dataFile(file.absoluteFilePath());
            if (!dataFile.exists())
                continue;
            dataFile.open(QIODevice::ReadOnly);
            QTextStream ts(&dataFile);
            ts.setCodec(QTextCodec::codecForName("UTF-8"));
            QString errorMsg;
            int eline;
            int ecol;
            QDomDocument xmlData( QString(file.baseName()));
            QString data(ts.readAll());
            dataFile.close();
            if ( !xmlData.setContent( data, &errorMsg, &eline, &ecol ))
            {
                qDebug()<<data<<errorMsg<<eline<<ecol;
                if (data.toLower().contains("404 not found"))
                    qDebug()<<"File not found on server";
                else
                    qDebug()<<"Could not open file"<<dataFile.fileName();
                continue;
            }
            QDomElement docElem = xmlData.documentElement();
            QDomNode n = docElem.firstChild();
            ScIconSetData isd;
            while( !n.isNull() )
            {
                QDomElement e = n.toElement();
                if( !e.isNull() )
                {
                    //qDebug()<<e.tagName()<<e.text();
                    if (e.tagName()=="path")
                    {
                        isd.path=e.text();
                    }
                    else
                    if (e.tagName()=="author")
                    {
                        isd.author=e.text();
                    }
                    else
                    if (e.tagName()=="license")
                    {
                        isd.license=e.text();
                    }
                    else
                    if (e.tagName()=="activeversion")
                    {
                        isd.activeversion=e.text();
                    }
                    else
                    if (e.tagName()=="nametext")
                    {
                        if (e.hasAttribute("lang"))
                        {
                            isd.nameTranslations.insert(e.attribute("lang"),e.text());
                            if (e.attribute("lang")=="en_US")
                                isd.baseName=e.text();
                        }
                    }
                }
                n = n.nextSibling();
            }
            //just in case there's no en_US basename
            if (!isd.baseName.isEmpty())
            {

                m_themeSets.insert(isd.baseName, isd);
                if(!isd.activeversion.isEmpty())
                {
                    int av_major, av_minor, av_patch, curr_major, curr_minor, curr_patch, ver_major, ver_minor, ver_patch;
                    av_major=isd.activeversion.section(".",0,0).toInt();
                    av_minor=isd.activeversion.section(".",1,1).toInt();
                    av_patch=isd.activeversion.section(".",2,2).toInt();
                    curr_major=m_activeSetVersion.section(".",0,0).toInt();
                    curr_minor=m_activeSetVersion.section(".",1,1).toInt();
                    curr_patch=m_activeSetVersion.section(".",2,2).toInt();
                    ver_major=QString(VERSION).section(".",0,0).toInt();
                    ver_minor=QString(VERSION).section(".",1,1).toInt();
                    ver_patch=QString(VERSION).section(".",2,2).toInt();
                    //If theme version <= app version, and theme version >= current active theme set version
                    if (av_major<=ver_major &&
                        av_minor<=ver_minor &&
                        av_patch<=ver_patch &&
                        (
                        av_major>=curr_major ||
                        (av_major==curr_major && av_minor>=curr_minor) ||
                        (av_major==curr_major && av_minor==curr_minor && av_patch>=curr_patch)
                        )
                        )
                    {
                        m_backupSetBasename=m_activeSetBasename;
                        m_backupSetVersion=m_backupSetVersion;
                        m_activeSetBasename=isd.baseName;
                        m_activeSetVersion=isd.activeversion;
                        //qDebug()<<"backupSetBasename"<<m_backupSetBasename<<"activeSetBasename"<<m_activeSetBasename;
                    }
                }
            }
        }
    }
}



bool ThemeManager::setActiveFromPrefs(QString prefsSet)
{
    //qDebug()<<"setting active from prefs to"<<prefsSet;
	if (m_themeSets.contains(prefsSet))
    {
        m_activeSetBasename=m_themeSets[prefsSet].baseName;
        m_activeSetVersion=m_themeSets[prefsSet].activeversion;
        return true;
    }

	if(prefsSet.isEmpty()) return true;

    return false;
}

QString ThemeManager::baseNameForTranslation(QString transName)
{
    QString name;

    foreach (ScIconSetData value, m_themeSets)
    {
        QMapIterator<QString, QString> nameValue(value.nameTranslations);
        while (nameValue.hasNext())
        {
            nameValue.next();
            if (nameValue.value()==transName)
                return value.nameTranslations["en_US"];
        }
    }
    return name;
}


QString ThemeManager::activePath(){
    return m_themeSets[m_activeSetBasename].path;

}

QStringList ThemeManager::pathList()
{
    QStringList paths;
    foreach (ScIconSetData value, m_themeSets)
        paths << value.path;
    return paths;
}

QStringList ThemeManager::nameList(QString language)
{
    QStringList names;

    foreach (ScIconSetData value, m_themeSets)
    {
        if (value.nameTranslations.contains(language))
            names << value.nameTranslations.value(language);
        else if (value.nameTranslations.contains("en_US"))
            names << value.nameTranslations.value("en_US");
        else if (value.nameTranslations.contains("en"))
            names << value.nameTranslations.value("en");
        else
            names << value.path;
    }
    return names;
}

void ThemeManager::languageChange()
{

}

