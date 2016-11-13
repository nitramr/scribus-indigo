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
#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H


#include <QMap>
#include <QObject>
#include <QString>

#include "prefsstructs.h"
#include "scribusapi.h"
#include "scpixmapcache.h"

class PrefsFile;
class ScribusMainWindow;


/**
  * @author Martin Reininger
  * @brief Manage Scribus themes here, and here alone
  */
class SCRIBUS_API ThemeManager : public QObject
{

    Q_OBJECT

    public:
        ThemeManager(QObject *parent = 0);
        ~ThemeManager();

        /**
        * @brief Returns a pointer to the ThemeManager instance
        * @return A pointer to the ThemeManager instance
        */
        static ThemeManager* instance();
        /**
        * @brief Deletes the ThemeManager Instance
        * Must be called when ThemeManager is no longer needed.
        */
        static void deleteInstance();
        bool setup();
        QString activePath();
        QStringList pathList();
        QStringList nameList(QString language);
        bool setActiveFromPrefs(QString prefsSet);
        QString activeSetBasename() { return m_activeSetBasename; }
        QString baseNameForTranslation(QString transName);

    public slots:
        void languageChange();

    private:
        /**
        * @brief The only instance of ThemeManager available.
        *
        * ThemeManager is singleton and the instance can be queried with the method
        * instance().
        */
        static ThemeManager* m_instance;
        QMap<QString, ScIconSetData> m_themeSets;
        QString m_activeSetBasename;
        QString m_activeSetVersion;
        QString m_backupSetBasename;
        QString m_backupSetVersion;

        bool initThemes();
        void readThemeConfigFiles();


    signals:
        void prefsChanged();
};

#endif
