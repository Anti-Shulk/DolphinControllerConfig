#ifndef PREVIOUSCONFIGURATIONMANAGER_H
#define PREVIOUSCONFIGURATIONMANAGER_H

#include <QSettings>

class PreviousConfigurationManager {
private:
    QSettings settings{QSettings::IniFormat,
                QSettings::UserScope,
                "DolphinControllerConfig",
                "PreviousConfiguration"};
public:
    void setSetting(QString group, QString key, QString value)
    {
        settings.beginGroup(group);
        settings.setValue(key, value);
        settings.endGroup();
    }

    QString getSetting(QString group, QString key)
    {
        settings.beginGroup(group);
        QString value = settings.value(key).toString();
        settings.endGroup();
        return value;
    }
    QStringList getKeys(QString group)
    {
        settings.beginGroup(group);
        QStringList value = settings.childKeys();
        settings.endGroup();
        return value;
    }



    QString getPlayerConfig(int playerNumber, QString key)
    {
        return getSetting("Player" + QString::number(playerNumber), key);
    }

    void setPlayerConfig(int playerNumber, QString key, QString value)
    {
        setSetting("Player" + QString::number(playerNumber), key, value);
    }






};

#endif // PREVIOUSCONFIGURATIONMANAGER_H
