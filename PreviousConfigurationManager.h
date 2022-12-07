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
    void setSetting(const QString& group, const QString& key, const QString& value)
    {
        settings.beginGroup(group);
        settings.setValue(key, value);
        settings.endGroup();
    }

    QString getSetting(const QString& group, const QString& key)
    {
        settings.beginGroup(group);
        QString value = settings.value(key).toString();
        settings.endGroup();
        return value;
    }
    QStringList getKeys(const QString& group)
    {
        settings.beginGroup(group);
        QStringList value = settings.childKeys();
        settings.endGroup();
        return value;
    }



    QString getPlayerConfig(int playerNumber, const QString& key)
    {
        return getSetting("Player" + QString::number(playerNumber), key);
    }

    void setPlayerConfig(int playerNumber, QString key, QString value)
    {
        setSetting("Player" + QString::number(playerNumber), key, value);
    }






};

#endif // PREVIOUSCONFIGURATIONMANAGER_H
