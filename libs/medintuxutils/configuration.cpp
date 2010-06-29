/***************************************************************************
 *   FreeMedicalForms                                                      *
 *   (C) 2008-2010 by Eric MAEKER, MD                                     **
 *   eric.maeker@free.fr                                                   *
 *   All rights reserved.                                                  *
 *                                                                         *
 *   This program is a free and open source software.                      *
 *   It is released under the terms of the new BSD License.                *
 *                                                                         *
 *   Redistribution and use in source and binary forms, with or without    *
 *   modification, are permitted provided that the following conditions    *
 *   are met:                                                              *
 *   - Redistributions of source code must retain the above copyright      *
 *   notice, this list of conditions and the following disclaimer.         *
 *   - Redistributions in binary form must reproduce the above copyright   *
 *   notice, this list of conditions and the following disclaimer in the   *
 *   documentation and/or other materials provided with the distribution.  *
 *   - Neither the name of the FreeMedForms' organization nor the names of *
 *   its contributors may be used to endorse or promote products derived   *
 *   from this software without specific prior written permission.         *
 *                                                                         *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     *
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        *
 *   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,  *
 *   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  *
 *   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      *
 *   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    *
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     *
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
 *   POSSIBILITY OF SUCH DAMAGE.                                           *
 ***************************************************************************/
/***************************************************************************
 *   Main Developper : Eric MAEKER, <eric.maeker@free.fr>                  *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#include "configuration.h"

#include <utils/log.h>
#include <utils/global.h>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QVariant>

namespace MedinTux {
namespace Constants {
    const char* const  MEDINTUX_SEARCH    = "*edin*ux*";
    const char* const  MEDINTUX_ROOT_PATH = "MedinTux";
    const char* const  MANAGER_PATH       = "Programmes/Manager/bin";
    const char* const  DRTUX_PATH         = "Programmes/drtux/bin";
    const char* const  MANAGER_INI        = "Manager.ini";
    const char* const  DRTUX_INI          = "drtux.ini";

    const char* const  MANAGER_PATH_2_10       = "Manager/bin";
    const char* const  DRTUX_PATH_2_10         = "drtux/bin";
    const char* const  INI_PATH_LINUX2_10      = ".MedinTux";

    const char* const  DRTUX_USERMENU_PATH      = "Ressources/Menus";  // from drtux/bin/
    const char* const  DRTUX_RESOURCES_PATH     = "Ressources";        // from drtux/bin/

    const char* const  MACOS_APPLICATIONS_PATH  = "/Applications";
    const char* const  WIN_APPLICATIONS_PATH    = "/Program Files";
    const char* const  LINUX_APPLICATIONS_PATH  = "/usr/share";

    const char* const  GLOSSARY_RUB             = "Glossaire";
    const char* const  GLOSSARY_KEY             = "Path";
    const char* const  GLOSSARY_OBS             = "Observation";
    const char* const  GLOSSARY_PRE             = "Prescription";
    const char* const  GLOSSARY_TER             = "Terrain";
    const char* const  GLOSSARY_DOC             = "Documents";
    const char* const  GLOSSARY_INS             = "Champs d'insertions";

    const char* const  DEFAULTLIST_RUB          = "MenuContextuel";
    const char* const  DEFAULTLIST_OBS          = "ListObserv";
    const char* const  DEFAULTLIST_PRE          = "ListPrescr";
    const char* const  DEFAULTLIST_DOC          = "ListDocuments";
    const char* const  DEFAULTLIST_IMA          = "ListImages";

    const char* const  GLOSSARY_CACHELIST_PATH  = "Champs d'insertions/cache";

    const char * const MEDINTUX_MENU_FILENAME     = "definition.txt";
    const char * const MEDINTUX_MENUFILE_CONTENT
            = "<!DOCTYPE CMenu>\n<CMenu version=\"0.0\" stdsetdef=\"1\">\n"
              "<actions>\n"
              " <action>\n"
              "  <property name=\"name\">\n"
              "   <cstring>%1</cstring>\n"                // Title         %1
              "  </property>\n"
              "  <property name=\"iconSet\">\n"
              "   <iconset>%2</iconset>\n"               // Icon           %2
              "  </property>\n"
              "  <property name=\"text\">\n"
              "   <string>%1</string>\n"                 // Title
              "  </property>\n"
              "  <property name=\"menuText\">\n"
              "   <string>%1</string>\n"                 // Title
              "  </property>\n"
              "  <property name=\"accel\">\n"
              "   <string>%3</string>\n"                 // Keysequence     %3
              "  </property>\n"
              "  <property name=\"script\">\n"
              "   <string>%4</string>\n"                 // Script          %4
              "  </property>\n"
              " </action>\n"
              "</actions>\n"
              "</CMenu>\n";

}  // End Constants
}  // End MedinTux


using namespace MedinTux;
using namespace MedinTux::Constants;

namespace MedinTux {
namespace Internal {
class ConfigurationPrivate {
public:
    ConfigurationPrivate() :
            m_IsMedintuxPluginsTested(false),
            m_IsMedintuxPlugins(false)
    {}

    QString screenDirForMedinTuxBinary( const QString &absPath, const QString &binarySubPath )
    {
        const QStringList &list = QDir(absPath).entryList(QStringList() << MEDINTUX_SEARCH, QDir::Dirs | QDir::NoDotAndDotDot );
        foreach( const QString &path, list) {
            if (path.contains(MEDINTUX_ROOT_PATH, Qt::CaseInsensitive)) {
                QString toTest = absPath + QDir::separator() + path + QDir::separator() + binarySubPath;
                if (QFileInfo(toTest).exists())
                    return toTest;
            }
        }
        return QString();
    }

    QString screenHomePathForMedinTuxBinary( const QString &binarySubPath )
    {
        QString toReturn;
            toReturn = screenDirForMedinTuxBinary( QDir::homePath(), binarySubPath );
        if (toReturn.isEmpty()) {
            // scan all dirs from home()
            foreach( const QString &path, QDir::home().entryList(QStringList() << "*", QDir::Dirs | QDir::NoDotAndDotDot ) ) {
                toReturn = screenDirForMedinTuxBinary( path, binarySubPath );
                if (!toReturn.isEmpty()) {
                    return toReturn;
                } else {
                    foreach( const QString &path2, QDir(QDir::homePath() + QDir::separator() + path).entryList(QStringList() << "*", QDir::Dirs | QDir::NoDotAndDotDot ) ) {
                        toReturn = screenDirForMedinTuxBinary( path2, binarySubPath );
                        if (!toReturn.isEmpty()) {
                            return toReturn;
                        }
                    }
                }
            }
        }
        return QString();
    }

    QString findManagerBinaryPath_MacOS()
    {
        QString toReturn;
        toReturn = screenDirForMedinTuxBinary( MACOS_APPLICATIONS_PATH, MANAGER_PATH );
        if (toReturn.isEmpty())
            toReturn = screenHomePathForMedinTuxBinary(MANAGER_PATH);
        if (toReturn.isEmpty()) {
            toReturn = screenDirForMedinTuxBinary( MACOS_APPLICATIONS_PATH, MANAGER_PATH_2_10);
            if (toReturn.isEmpty())
                toReturn = screenHomePathForMedinTuxBinary(MANAGER_PATH_2_10);
        }
        return toReturn;
    }

    QString findManagerBinaryPath_Linux()
    {
        QString toReturn;
        toReturn = screenDirForMedinTuxBinary( LINUX_APPLICATIONS_PATH, MANAGER_PATH );
        if (toReturn.isEmpty())
            toReturn = screenHomePathForMedinTuxBinary(MANAGER_PATH);
        if (toReturn.isEmpty()) {
            toReturn = screenDirForMedinTuxBinary( LINUX_APPLICATIONS_PATH, MANAGER_PATH_2_10);
            if (toReturn.isEmpty())
                toReturn = screenHomePathForMedinTuxBinary(MANAGER_PATH_2_10);
        }
        return toReturn;
    }

    QString findManagerBinaryPath_Win()
    {
        QString toReturn;
        toReturn = screenDirForMedinTuxBinary( WIN_APPLICATIONS_PATH, MANAGER_PATH );
        if (toReturn.isEmpty())
            toReturn = screenHomePathForMedinTuxBinary(MANAGER_PATH);
        if (toReturn.isEmpty()) {
            toReturn = screenDirForMedinTuxBinary( WIN_APPLICATIONS_PATH, MANAGER_PATH_2_10);
            if (toReturn.isEmpty())
                toReturn = screenHomePathForMedinTuxBinary(MANAGER_PATH_2_10);
        }
        return toReturn;
    }

public:
    QString m_ManagerPath;
    bool    m_IsMedintuxPluginsTested;
    bool    m_IsMedintuxPlugins;
    QString m_MedintuxPluginIniFileName;
}; // end class ConfigurationPrivate

}  // End Internal
}  // End MedinTux

Configuration *Configuration::m_Instance = 0;

Configuration *Configuration::instance()
{
    if (!m_Instance)
        m_Instance = new MedinTux::Configuration(qApp);
    return m_Instance;
}

Configuration::Configuration( QObject *parent ) :
        QObject(parent), d(0)
{
    d = new Internal::ConfigurationPrivate();
}

Configuration::~Configuration()
{
    if (d) delete d;
    d=0;
}

bool Configuration::applicationIsAMedintuxPlugins() const
{
    // retreive command line arguments
    if (d->m_IsMedintuxPluginsTested)
        return d->m_IsMedintuxPlugins;

    // test number of parameters (must be at least )
    QStringList args = QCoreApplication::arguments();
    if (args.count() < PlugIn_NumberOfDrTuxArgs ) {
        d->m_IsMedintuxPluginsTested = true;
        d->m_IsMedintuxPlugins = false;
        return false;
    }
    foreach(QString arg, args) {
        if (arg.contains(MANAGER_PATH_2_10)) {
            d->m_IsMedintuxPlugins = true;
            break;
        }
        else if (arg.contains(DRTUX_PATH_2_10)) {
            d->m_IsMedintuxPlugins = true;
            break;
        }
    }
    if (d->m_IsMedintuxPlugins) {
        Utils::Log::addMessage( "tkMedintuxConfiguration", QCoreApplication::translate( "diMedinTux", "MedinTux Detected : Starting as a MedinTux Plugin" ) );
        d->m_MedintuxPluginIniFileName = args[ PlugIn_IniFile ];
        Utils::Log::addMessage( "tkMedintuxConfiguration", QCoreApplication::translate( "diMedinTux", "MedinTux Detected : Using ini file %1" ).arg(d->m_MedintuxPluginIniFileName) );
    }
    d->m_IsMedintuxPluginsTested = true;
    return d->m_IsMedintuxPlugins;
}

QString Configuration::medintuxPluginIniFileName() const
{
    return d->m_MedintuxPluginIniFileName;
}

QString Configuration::medintuxPluginIni(const QString &rubrik, const QString &key ) const
{
    if (!applicationIsAMedintuxPlugins())
        return QString();
    // return key using QSettings structure
    QSettings s( medintuxPluginIniFileName(), QSettings::IniFormat );
    return s.value( rubrik + "/" + key, QVariant() ).toString();
}

QString Configuration::medintuxPluginInformation( PluginsParameters info ) const
{
    if (!applicationIsAMedintuxPlugins())
        return QString();
    if (info==PlugIn_IniFile)
        return medintuxPluginIniFileName();
    return QApplication::arguments().at(info);
}


QString Configuration::findManagerBinaryPath() const
{
    if (Utils::isRunningOnMac())
        d->m_ManagerPath = d->findManagerBinaryPath_MacOS();
    else if (Utils::isRunningOnLinux())
        d->m_ManagerPath = d->findManagerBinaryPath_Linux();
    else if (Utils::isRunningOnWin())
        d->m_ManagerPath = d->findManagerBinaryPath_Win();
    return d->m_ManagerPath;
}

QString Configuration::askUserForManagerBinary() const
{
    if (d->m_ManagerPath.isEmpty())
        findManagerBinaryPath();
    // msgbox --> automatically founded, keep it ?
    // msgbox if not --> qfiledialog
    return QString();
}

bool Configuration::setManagerBinaryPath(const QString &absPath)
{
    if (!QDir(absPath).exists())
        return false;
    d->m_ManagerPath = QDir::cleanPath(absPath);
    return true;
}

QString Configuration::managerIniFileName() const
{
    if (QFile(QDir::homePath() + QDir::separator() + INI_PATH_LINUX2_10 + QDir::separator() + MANAGER_INI).exists())
        return QDir::homePath() + QDir::separator() + INI_PATH_LINUX2_10 + QDir::separator() + MANAGER_INI;
    if (d->m_ManagerPath.isEmpty())
        findManagerBinaryPath();
    return Utils::isFileExists(d->m_ManagerPath + QDir::separator() + MANAGER_INI);
}

QVariant Configuration::managerIni( const QString &rubrik, const QString &key ) const
{
    if (d->m_ManagerPath.isEmpty()) {
        findManagerBinaryPath();
        if (d->m_ManagerPath.isEmpty())
            return QString();
    }
    // return key using QSettings structure
    QSettings s( managerIniFileName(), QSettings::IniFormat );
    return s.value( rubrik + "/" + key, QVariant() );
}

QString Configuration::drtuxIniFileName() const
{
    if (QFile(QDir::homePath() + QDir::separator() + INI_PATH_LINUX2_10 + QDir::separator() + DRTUX_INI).exists())
        return QDir::homePath() + QDir::separator() + INI_PATH_LINUX2_10 + QDir::separator() + DRTUX_INI;
    if (d->m_ManagerPath.isEmpty())
        findManagerBinaryPath();
    if (d->m_ManagerPath.isEmpty())
        return QString();
    return Utils::isFileExists(drtuxBinaryPath() + QDir::separator() + DRTUX_INI);
}

QVariant Configuration::drtuxIni( const QString &rubrik, const QString &key ) const
{
    if (d->m_ManagerPath.isEmpty()) {
        findManagerBinaryPath();
        if (d->m_ManagerPath.isEmpty())
            return QString();
    }
    // return key using QSettings structure
    QSettings s( drtuxIniFileName(), QSettings::IniFormat );
    return s.value( rubrik + "/" + key, QVariant() );
}

QString Configuration::glossaryPath() const
{
    if (d->m_ManagerPath.isEmpty()) {
        findManagerBinaryPath();
        if (d->m_ManagerPath.isEmpty())
            return QString();
    }
    return Utils::isDirExists( d->m_ManagerPath + QDir::separator() + managerIni(GLOSSARY_RUB, GLOSSARY_KEY).toString() );
}

QString Configuration::glossaryPath(GlossaryFor rubrik) const
{
    QString gp = glossaryPath();
    if (gp.isEmpty())
        return QString();
    switch (rubrik) {
        case ObservationGlossary  : return Utils::isDirExists(gp + QDir::separator() + GLOSSARY_OBS);
        case PrescriptionGlossary : return Utils::isDirExists(gp + QDir::separator() + GLOSSARY_PRE);
        case DocumentGlossary     : return Utils::isDirExists(gp + QDir::separator() + GLOSSARY_DOC);
        case TerrainGlossary      : return Utils::isDirExists(gp + QDir::separator() + GLOSSARY_TER);
        case ChampsInsertionGlossary : return Utils::isDirExists(gp + QDir::separator() + GLOSSARY_INS);
    }
    return QString();
}

QString Configuration::defaultListsPath(DefaultList rubrik) const
{
    QString tmp;
    switch (rubrik) {
        case ObservationDefaultList  : tmp = managerIni(DEFAULTLIST_RUB, DEFAULTLIST_OBS).toString(); break;
        case PrescriptionDefaultList : tmp = managerIni(DEFAULTLIST_RUB, DEFAULTLIST_PRE).toString(); break;
        case DocumentDefaultList     : tmp = managerIni(DEFAULTLIST_RUB, DEFAULTLIST_DOC).toString(); break;
        case ImageDefaultList        : tmp = managerIni(DEFAULTLIST_RUB, DEFAULTLIST_IMA).toString(); break;
    }
    if (!tmp.isEmpty())
        tmp.prepend(glossaryPath(ChampsInsertionGlossary) + QDir::separator());
    return Utils::isDirExists(tmp);
}

bool Configuration::deleteListCache() const
{
    // 4. delete MedinTux cache
    QDir cacheDir( QString("%1/%2").arg( glossaryPath(), GLOSSARY_CACHELIST_PATH) );
    if (cacheDir.exists() ) {
        QStringList list = cacheDir.entryList( QStringList() << "*" , QDir::Files);
        int n = 0;
        for (int i = 0; i < list.size(); ++i) {
            if (QFile(cacheDir.path() + QDir::separator() + list.at(i)).remove())
                ++n;
        }
        if (n == list.count())
            return true;
    }
    return false;
}

QString Configuration::drtuxBinaryPath() const
{
    if (d->m_ManagerPath.isEmpty())
        findManagerBinaryPath();
    if (d->m_ManagerPath.isEmpty())
        return QString();
    QString toReturn = d->m_ManagerPath;
    toReturn.replace(MANAGER_PATH_2_10, DRTUX_PATH_2_10);
    return Utils::isDirExists(toReturn);
}

QString Configuration::drtuxUserMenuPath() const
{
    QString tmp = drtuxBinaryPath() + QDir::separator() + DRTUX_USERMENU_PATH;
    return Utils::isDirExists(tmp);
}

QString Configuration::drtuxResourcesPath() const
{
    QString tmp = drtuxBinaryPath() + QDir::separator() + DRTUX_RESOURCES_PATH;
    return Utils::isDirExists(tmp);
}

bool Configuration::addUserMenu( const QString &shortTitle, const QString &title,
                                           const QString &script, const QString &iconAbsPath, const QString &keySequence )
{
    bool toReturn = true;
    QDir binDir(drtuxBinaryPath());

    // test drtux' users' menu path
    if (!binDir.exists(DRTUX_USERMENU_PATH)) {
        if (!binDir.mkdir(DRTUX_USERMENU_PATH) ) {
            Utils::Log::addError( this, tr( "Unable to configure MedinTux : Menus path does not exists and can not be created. Path to DrTux binary : %1").arg(binDir.path() ) );
            toReturn = false;
        } else {
            Utils::Log::addMessage( this, tr( "MedinTux Configuration : resources user menus path correctly created  %1.")
                             .arg(binDir.path() + QDir::separator() + DRTUX_USERMENU_PATH ));
        }
    }
    binDir.cd(DRTUX_USERMENU_PATH);

    // test users' menu path
    if (!binDir.cd(shortTitle)) {
        if (!binDir.mkdir(shortTitle)) {
            Utils::Log::addError( this, tr( "Unable to configure MedinTux : %1 menu does not exists and can not be created. Path to DrTux binary : %2")
                             .arg(shortTitle).arg(binDir.path() ) );
            toReturn = false;
        } else
            Utils::Log::addMessage( this, tr( "MedinTux Configuration : users' menu path correctly created %1.")
                             .arg(binDir.path() + QDir::separator() + shortTitle  ) );
    }
    binDir.cd(shortTitle);

    // create the menu item
    QString tmp = QString(MEDINTUX_MENUFILE_CONTENT)
                  .arg(title)
                  .arg(QFileInfo(iconAbsPath).fileName())
                  .arg(keySequence)
                  .arg(script);
    QString to = QString("%1/%2").arg(binDir.absolutePath(), MEDINTUX_MENU_FILENAME );
    Utils::saveStringToFile(tmp, to, Utils::Overwrite, Utils::DontWarnUser);

    // copy the icon to the dir
    QFileInfo iconFile(iconAbsPath);
    to = QString("%1/%2").arg(binDir.path(), iconFile.fileName() );
    if ( iconFile.exists() ) {
        if (QFile::copy( iconAbsPath , to) || QFile(to).exists())
            Utils::Log::addMessage( this, tr( "MedinTux Configuration : user's menu icon correctly installed %1.")
                               .arg(iconAbsPath) );
        else
            Utils::Log::addError( this, tr( "Unable to configure MedinTux : icon %1 can not be copied to %2.")
                             .arg(iconAbsPath,to) );
    } else {
        Utils::Log::addError( this, tr( "Unable to configure MedinTux : icon %1 does not exists.")
                         .arg(iconAbsPath) );
        toReturn = false;
    }

    return toReturn;
}
