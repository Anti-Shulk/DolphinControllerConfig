#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

enum Groups {
    RealControllers, EmulatedControllers, Paths
};

enum Keys {
    DolphinConfigPath // this used to be lowercase
};

// make singleton
class SettingsManager {
private:
    QSettings settings{QSettings::IniFormat,
                QSettings::UserScope,
                "DolphinControllerConfig",
                "Settings"};
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

    void setDefaultValues()
    {

        settings.beginGroup("RealControllers"); // group name cant have spaces
        settings.setValue("xone", "Xbox One Controller");
        settings.setValue("ds4", "Dualshock 4 Controller");
        settings.setValue("nspro", "Nintendo Switch Pro Controller");
        settings.setValue("wii", "Wii Remote");
        settings.setValue("gc", "GameCube Controller");
        settings.endGroup();

        settings.beginGroup("ProfileSuffixes");
        settings.setValue("east", "with layout correct");
        settings.setValue("west", "with button label correct");
        settings.setValue("numchuck", "and numchuck");
        settings.setValue("sideways", "sideways");
        settings.endGroup();

    //    settings.beginGroup("Paths");
    //    settings.setValue("dolphinconfigpath", "C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config");
    //    settings.endGroup();
    }
//    void loadRealControllers()
//    {
//        settings.chi
//        settings.beginGroup("RealControllers");

//        settings.endGroup();
//    }

    QString getRealControllerKey(int keyNumber)
    {
        return getKeys("RealControllers").value(keyNumber);
    }



};

#endif // SETTINGSMANAGER_H
