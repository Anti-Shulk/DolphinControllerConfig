#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QGamepadManager>
#include <QFrame>
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QString>
//#include <QGamepad>
//#include <QtGamepad>
#include <QtGamepad>
#include <QSettings>
#include "selector.h"
#include "menuitems.h"



//TODO: for every value function, make a defualt value
//TODO: dont modify if it wasnt modified by us
//TODO: how to launch with args
//TODO: clean comments
//TODO: clean code
//TODO: make releases executables
//TODO: make controller profiles
//TODO: make an option for none in controller select
//TODO: fix weird behavior especially whrn switching players
//TODO: seperate some stuff out into multiple files
//TODO: move includes into header file
//TODO: copy your settings.ini
//TODO: check for settings then save them


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Selector playerSelector{1, 1, 4};
    Selector listSelector{5, 0, 5};
    Selector realControllerSelector{0, 0, 0};
    Selector portSelector{1, 1, 4};
    Selector emulatedControllerSelector{0, 0, 1};
    Selector profileSelector{0, 0, 0};
    QSettings settings{QSettings::IniFormat, QSettings::UserScope, "DolphinControllerConfig", "Settings"};
    QSettings previousConfiguration{QSettings::IniFormat, QSettings::UserScope, "DolphinControllerConfig", "PreviousConfiguration"};

    //QString realController = "SDL/0/Sony Interactive Entertainment Wireless Controller";
    QString emulatedController{};
    QString controllerProfile{};
    bool isWiimote = false;
    Ui::MainWindow *ui;
    MenuItems menuItems;
    QFrame *frames[6];

public:


    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) , ui(new Ui::MainWindow)
    {

        QGamepad *gamepad = new QGamepad(QGamepadManager::instance()->connectedGamepads().at(0), this);

        ui->setupUi(this);

        this->showFullScreen();

        connect(gamepad, &QGamepad::buttonAChanged, this, &MainWindow::aPressed);
        connect(gamepad, &QGamepad::buttonUpChanged, this, &MainWindow::upPressed);
        connect(gamepad, &QGamepad::buttonDownChanged, this, &MainWindow::downPressed);
        connect(gamepad, &QGamepad::buttonLeftChanged, this, &MainWindow::leftPressed);
        connect(gamepad, &QGamepad::buttonRightChanged, this, &MainWindow::rightPressed);

        menuItems.initalize(std::initializer_list<QFrame*>{ui->PlayerFrame, ui->RealControllerFrame, ui->PortFrame, ui->EmulatedControllerFrame, ui->ProfileFrame, ui->LaunchFrame});

//        menuItems.setSelectedFrame(0);


        loadRealControllers();
        loadCurrentPlayerConfig();
        settings.beginGroup("Paths");
        settings.endGroup();
    }

    ~MainWindow()
    {
        delete ui;
    }
    //    void on_Quit_clicked();

    //    QString getControllerProfile();

        void replaceFileText(QString filePath, QString toBeReplaced, QString toReplaceWith)
        {
            QFile file(filePath);
            QFile tempFile(filePath.append(".tmp"));

            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::warning(this, tr("Warning"), tr("Unable to open file: ") + file.errorString());
                    return;
            }

            if (!tempFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::warning(this, tr("Warning"), tr("Unable to create temporary file: ") + tempFile.errorString());
                    return;
            }

            QTextStream stream(&file);
            QTextStream tempStream(&tempFile);
            QString line;

            while (!stream.atEnd()) {
                line = stream.readLine();
                if (line.contains(toBeReplaced)) {
                    tempStream << line.replace(toBeReplaced, toReplaceWith) + "\n";
                    continue;
                }
                tempStream << line + "\n";
            }

            tempStream.seek(0);
            stream.seek(0);
            while (!tempStream.atEnd()) {
                line = tempStream.readLine();
                stream << line + "\n";
            }

            tempFile.remove();

        }

        void replaceFileLine(QString filePath, QString toBeReplaced, QString toReplaceWith)
        {
            QFile file(filePath);
            QFile tempFile(filePath.append(".tmp"));

            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::warning(this, tr("Warning"), tr("Unable to open file: ") + file.errorString());
                    return;
            }

            if (!tempFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::warning(this, tr("Warning"), tr("Unable to create temporary file: ") + tempFile.errorString());
                    return;
            }

            QTextStream stream(&file);
            QTextStream tempStream(&tempFile);
            QString line;

            while (!stream.atEnd()) {
                line = stream.readLine();
                if (line.contains(toBeReplaced)) {
                    tempStream << toReplaceWith + "\n";
                    continue;
                }
                tempStream << line + "\n";
            }

            tempStream.seek(0);
            stream.seek(0);
            while (!tempStream.atEnd()) {
                line = tempStream.readLine();
                stream << line + "\n";
            }
            file.close();
            tempFile.remove();

        }

        void replaceFileLines(QString filePath, QString toBeReplacedStart, QString toBeReplacedEnd, QString toReplaceWith)
        {
            QFile file(filePath);
            QFile tempFile(filePath.append(".tmp"));

            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::warning(this, tr("Warning"), tr("Unable to open file: ") + file.errorString());
                    return;
            }

            if (!tempFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::warning(this, tr("Warning"), tr("Unable to create temporary file: ") + tempFile.errorString());
                    return;
            }

            QTextStream stream(&file);
            QTextStream tempStream(&tempFile);
            QString line;

            while (!stream.atEnd()) {
                line = stream.readLine();
                if (line.contains(toBeReplacedStart)) {
                    tempStream << line + "\n";
                    break;
                }
                else tempStream << line + "\n";
            }

            tempStream << toReplaceWith + "\n";

            while (!stream.atEnd()) {
                line = stream.readLine();
                if (line.contains(toBeReplacedEnd)) {
                    tempStream << line + "\n";
                    break;
                }


            }

            while (!stream.atEnd())
                tempStream << stream.readLine() + "\n";

            tempStream.seek(0);
            stream.seek(0);
            while (!tempStream.atEnd())
                stream << tempStream.readLine() + "\n";

            file.close();

            tempFile.remove();

        }

        QString fileToString(QString filePath)
        {
            QString contents;
            QFile file(filePath);

            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::warning(this, tr("Warning"), tr("Unable to open file: ") + file.errorString());
                    return nullptr;
            }
            QTextStream stream(&file);
            while (!stream.atEnd()) {
                contents.append(stream.readLine() + "\n");
            }
            file.close();
            return contents;
        }


        void resetControllerProfiles()
        {
            QFile dolphinWiimoteNewSettings(getSetting("Paths", "dolphinconfigpath") + "WiimoteNew.ini");
            QTextStream dolphinWiimoteNewSettingsStream(&dolphinWiimoteNewSettings);
            QFile dolphinGCPadNewSettings(getSetting("Paths", "dolphinconfigpath") + "GCPadNew.ini");
            QTextStream dolphinGCPadNewSettingsStream(&dolphinGCPadNewSettings);

            if (!dolphinWiimoteNewSettings.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::warning(this, tr("Warning"), tr("Unable to open or write to dolphin configuration files: ") + dolphinWiimoteNewSettings.errorString());
                return;
            }

            if (!dolphinGCPadNewSettings.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::warning(this, tr("Warning"), tr("Unable to open or write to dolphin configuration files: ") + dolphinGCPadNewSettings.errorString());
                return;
            }

            dolphinWiimoteNewSettingsStream << "[Wiimote1]\n\n" << "[Wiimote2]\n\n" << "[Wiimote3]\n\n" << "[Wiimote4]\n\n" << "[BalanceBoard]\n" << "Source = 0";
            dolphinGCPadNewSettingsStream << "[GCPad1]\n\n" << "[GCPad2]\n\n" << "[GCPad3]\n\n" << "[GCPad4]\n\n";

        }



    //    void writeControllerProfileToTempFile(int controllerNumber, QString controllerType);

    //    void writeControllerProfile(int controllerNumber, QString controllerType);

        void setAsSelected(QFrame *frame)
        {
            frame->setStyleSheet("color: rgb(48, 48, 48);"
                                 "background-color: white");
        }

        void setAsDeselected(QFrame *frame)
        {
            frame->setStyleSheet("color: white;"
                                 "background-color: rgb(48, 48, 48)");
        }

        void setSelectedFrame(int frameNumber)
        {
            for (int i = 0; i < 6; ++i) {
                if (i == frameNumber) setAsSelected(frames[i]);
                else setAsDeselected(frames[i]);
            }
        }





        void saveSettings()
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


        void loadRealControllers()
        {
            settings.beginGroup("RealControllers");
            realControllerSelector.setMaxValue(settings.childKeys().length() - 1); // we do -1 becuase we count our array starting from 0 meaning that if there are 4 keys, 3 would be the max value
            settings.endGroup();
        }

        QString getSetting(QString group, QString key)
        {
            QString value;
            settings.beginGroup(group);
            value = settings.value(key).toString();
            settings.endGroup();
            return value;
        }

        QString arrowAdder(QString arrowlessString)
        {
            return "⯇ " + arrowlessString + " ⯈";
        }


        QString getPlayerNumber()
        {
            return QString::number(playerSelector.getSelection());
        }


        QString getPlayerConfig(int playerNumber, QString key)
        {
            QString value;
            previousConfiguration.beginGroup("Player" + QString::number(playerNumber));
            value = previousConfiguration.value(key).toString();
            previousConfiguration.endGroup();
            return value;
        }


        QString getPlayerConfig(QString key)
        {
            return getPlayerConfig(playerSelector.getSelection(), key);
        }


        void setPlayerConfig(QString key, QString value)
        {
            previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
            previousConfiguration.setValue(key, value);
            previousConfiguration.endGroup();
        }

        QString getRealControllerKey(int keyNumber)
        {
            QString key;
            settings.beginGroup("RealControllers");
            key = settings.childKeys().value(keyNumber);
            settings.endGroup();
            return key;
        }

        QString getCurrentRealControllerKey()
        {
            return getRealControllerKey(realControllerSelector.getSelection());
        }

        QString getRealControllerValue(QString key)
        {
            QString value;
            settings.beginGroup("RealControllers");
            value = settings.value(key).toString();
            settings.endGroup();
            return value;
        }

        QString getPath()
        {
            QString key;
            settings.beginGroup("Paths");
            key = settings.value("dolphinconfigpath").toString();
            settings.endGroup();
            return key;
        }


        QString getCurrentRealControllerValue()
        {
            return getRealControllerValue(getCurrentRealControllerKey());
        }


        void setCurrentRealController()
        {
            ui->RealControllerSelection->setText(arrowAdder(getCurrentRealControllerValue()));
            setPlayerConfig("RealController", getCurrentRealControllerKey());

        }

        void setRealController(QString key)
        {
            ui->RealControllerSelection->setText(arrowAdder(getRealControllerValue(key)));
            setPlayerConfig("RealController", key);
        }

        QString getEmulatedController()
        {
            if (emulatedControllerSelector.getSelection() == 0) return "gc";
            else return "wii";
        }

        void setEmulatedControllerWithoutArrows(int selection)
        {
            if (selection == 1) {
                setPlayerConfig("EmulatedController", "wii");
                ui->EmulatedControllerSelection->setText("Wii Remote");
            }
            else {
                setPlayerConfig("EmulatedController", "gc");
                ui->EmulatedControllerSelection->setText("GameCube Controller");
            }
        }

        void setEmulatedController(int selection)
        {
            if (selection == 1) {
                setPlayerConfig("EmulatedController", "wii");
                ui->EmulatedControllerSelection->setText(arrowAdder("Wii Remote"));
            }
            else {
                setPlayerConfig("EmulatedController", "gc");
                ui->EmulatedControllerSelection->setText(arrowAdder("GameCube Controller"));
            }
        }

        void setEmulatedController(QString selection)
        {
            if (selection == "wii") setEmulatedController(1);
            else setEmulatedController(0);
        }


        void setCurrentEmulatedController()
        {
            setEmulatedController(emulatedControllerSelector.getSelection());
        }

        QStringList getProfileSuffixes()
        {
            QStringList suffixes;
            settings.beginGroup("ProfileSuffixes");
            suffixes = settings.childKeys();
            settings.endGroup();
            return suffixes;
        }

        void setPort(int portNumber)
        {
            ui->PortSelection->setText(arrowAdder(QString::number(portNumber)));
            setPlayerConfig("Port", QString::number(portNumber));
        }

        void setCurrentPort()
        {
            setPort(portSelector.getSelection());
        }



        void setProfile(QString profile)
        {
            setPlayerConfig("Profile", profile);
            ui->ProfileSelectoin->setText(arrowAdder(profile));
        }


        bool isRealController()
        {
            return (getCurrentRealControllerKey() == "gc") || (getCurrentRealControllerKey() == "wii");
        }


        void setPlayerNumber()
        {
            ui->PlayerSelection->setText(arrowAdder("Player " + getPlayerNumber()));
        }

        void loadCurrentPlayerConfig()
        {
            setRealController(getPlayerConfig("RealController"));
            setPort(getPlayerConfig("Port").toInt());
            if (getPlayerConfig("RealController") == "gc") {
                setEmulatedControllerWithoutArrows(0);
                setProfile("realgc");
                ui->ProfileSelectoin->setText("Real Controller");
                return;
            } else if (getPlayerConfig("RealController") == "wii") {
                setEmulatedControllerWithoutArrows(1);
                setProfile("realwii");
                ui->ProfileSelectoin->setText("Real Controller");
                return;
            }
            setEmulatedController(getPlayerConfig("EmulatedController"));
            setProfile(getPlayerConfig("Profile"));
        }


        void selectionAction(bool isIncreasing)
        {
            switch (listSelector.getSelection()) {
            default:
            break;
            case 0: /* Players */
                playerSelector.modifySelection(isIncreasing, true);
                setPlayerNumber();
                loadCurrentPlayerConfig();
            break;
            case 1: /* Real Controllers*/
                realControllerSelector.modifySelection(isIncreasing, true);
                setCurrentRealController();

                if (getCurrentRealControllerKey() == "gc") {
                    setEmulatedControllerWithoutArrows(0);
                    setProfile("realgc");
                    ui->ProfileSelectoin->setText("Real Controller");
                    break;
                } else if (getCurrentRealControllerKey() == "wii") {
                    setEmulatedControllerWithoutArrows(1);
                    setProfile("realwii");
                    ui->ProfileSelectoin->setText("Real Controller");
                    break;
                }
                ui->EmulatedControllerSelection->setText(arrowAdder(" "));
                ui->ProfileSelectoin->setText(arrowAdder(" "));
            break;
            case 2: /* Ports */
                portSelector.modifySelection(isIncreasing, true);
                setCurrentPort();
            break;
            case 3: /* Emulated Controllers */
                if (isRealController()) break;
                emulatedControllerSelector.modifySelection(isIncreasing, true);
                setCurrentEmulatedController();
            break;
            case 4:
                QDir dir;
                QFileInfoList usableProfiles;
                if (getEmulatedController() == "gc") dir = QDir(getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/GCPad");
                else dir = QDir(getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/Wiimote");

                const QFileInfoList possibleFiles = dir.entryInfoList();
                const QStringList suffixes = getProfileSuffixes();

                for (const QString &suffix: suffixes) {
                    for (const QFileInfo &file: possibleFiles) {
                        if (file.baseName().startsWith(getPlayerConfig("RealController")) && file.baseName().endsWith(suffix)) {
                            usableProfiles.append(file);
                        }
                    }
                }
                if (usableProfiles.empty()) break;

                profileSelector.setMaxValue(usableProfiles.count() - 1);
                profileSelector.modifySelection(isIncreasing, true);
                setProfile(usableProfiles.at(profileSelector.getSelection()).baseName());
            break;
            }
        }


        void launch()
        {
            settings.beginGroup("Path");
        //    QSettings dolphinSettings(settings.value("dolphinconfigpath").toString() + "\\Dolphin.ini", QSettings::IniFormat);
        //    QSettings dolphinGCPadNewSettings(settings.value("dolphinconfigpath").toString() + "\\GCPadNew.ini", QSettings::IniFormat);
        //    QSettings dolphinWiimoteNewSettings(settings.value("dolphinconfigpath").toString() + "\\WiimoteNew.ini", QSettings::IniFormat);
        //    QSettings dolphinSettings("C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config\\Dolphin.ini", QSettings::IniFormat);
        //    QSettings dolphinGCPadNewSettings("C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config\\GCPadNew.ini", QSettings::IniFormat);
        //    QSettings dolphinWiimoteNewSettings("C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config\\WiimoteNew.ini", QSettings::IniFormat);
            settings.endGroup();


        //    for (QString key: settings.childKeys()) { // how to delete keys
        //        settings.remove(key);

        //    }

            // In these two functions, we cannot use the standard QSettings to interface with the dolphin controller profiles
            // becuase they do not allow spaces or forward slashes in file names easily. instead, I am using file text streams to put the text into the files.
            resetControllerProfiles();
            for (int i = 1; i <= 4; i++) { // for each player

                previousConfiguration.beginGroup("Player" + QString::number(i));
                if (previousConfiguration.value("EmulatedController") == "gc") {
                    replaceFileLines(getSetting("Paths", "dolphinconfigpath") + "/GCPadNew.ini",
                                    "[GCPad" + QString::number(i) + "]",
                                    i == 4 ? "\n\n\n": "[GCPad" + QString::number(i+1) + "]",
                                    fileToString(getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/GCPad" + "/" + previousConfiguration.value("Profile").toString() + ".ini").remove("[Profile]\n"));
                    replaceFileText(getSetting("Paths", "dolphinconfigpath") + "GCPadNew.ini",
                                    "{portNumber}",
                                    QString::number(previousConfiguration.value("Port").toInt() - 1));
                    replaceFileLine(getSetting("Paths", "dolphinconfigpath") + "Dolphin.ini",
                                    "SIDevice" + QString::number(i-1),
                                    "SIDevice" + QString::number(i-1) + " = " + QString::number(previousConfiguration.value("Profile").toString() == "realgc" ? 12: 6));

                    replaceFileLines(getSetting("Paths", "dolphinconfigpath") + "WiimoteNew.ini",
                                    "[Wiimote" + QString::number(i) + "]",
                                    i == 4 ? "[BalanceBoard]" : "[Wiimote" + QString::number(i+1) + "]",
                                    "Source = 0\n");
                } else {
                    replaceFileLines(getSetting("Paths", "dolphinconfigpath") + "WiimoteNew.ini",
                                    "[Wiimote" + QString::number(i) + "]",
                                    i == 4 ? "[BalanceBoard]" : "[Wiimote" + QString::number(i+1) + "]",
                                    fileToString(getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/Wiimote" + "/" + previousConfiguration.value("Profile").toString() + ".ini").remove("[Profile]\n"));
                    replaceFileText(getSetting("Paths", "dolphinconfigpath") + "WiimoteNew.ini",
                                    "{portNumber}",
                                    QString::number(previousConfiguration.value("Port").toInt() - 1));
                    replaceFileLine(getSetting("Paths", "dolphinconfigpath") + "WiimoteNew.ini",
                                    "[Wiimote" + QString::number(i) + "]",
                                    "[Wiimote" + QString::number(i) + "]" + "\n" +
                                    "Source = " + QString::number(previousConfiguration.value("Profile").toString() == "realwii" ? 2: 1));

                    replaceFileLine(getSetting("Paths", "dolphinconfigpath") + "Dolphin.ini",
                                    "SIDevice" + QString::number(i-1),
                                    "SIDevice" + QString::number(i-1) + " = 0\n");
                }

                previousConfiguration.endGroup();
            }


            //    QProcess process;
            //    QStringList args("");
            //    QString file = "cmd.exe";
            //    process.start(file, args);
            //    QProcess::startDetached(getSetting("Paths", "dolphinpath"));
            //    QProcess::startDetached("C:\\Users\\Me\\Downloads\\dolphin-master-5.0-16380-x64\\Dolphin-x64\\Dolphin.exe");




                QProcess::startDetached(getSetting("Paths", "dolphinpath"), QStringList());
                QApplication::quit();



        }



private slots:
        void aPressed(bool value)
        {
            if (!value) return;
            if (listSelector.getSelection() != 5) return;
            launch();
        }

        void upPressed(bool value)
        {
            if (!value) return;
            listSelector.decreaseSelection();
            setSelectedFrame(listSelector.getSelection());
        }


        void downPressed(bool value)
        {
            if (!value) return;
            listSelector.increaseSelection();
            setSelectedFrame(listSelector.getSelection());
        }

        void leftPressed(bool value)
        {
            if (!value) return;
        //    switch (listSelector.getSelection()) {
        //    case 0:
        //        playerSelector.decreaseSelection(true);
        //        ui->PlayerSelection->setText(arrowAdder("Player " + QString::number(playerSelector.getSelection())));
        //        break;
        //    case 1:
        //        settings.beginGroup("RealControllers");
        //        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //        realControllerSelector.decreaseSelection(true);
        //        ui->RealControllerSelection->setText(arrowAdder(settings.value(settings.childKeys().value(realControllerSelector.getSelection())).toString()));
        //        previousConfiguration.setValue("RealController", settings.childKeys().value(realControllerSelector.getSelection()));
        //        previousConfiguration.endGroup();
        //        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //        if (settings.childKeys().value(realControllerSelector.getSelection()) == "gc" || settings.childKeys().value(realControllerSelector.getSelection()) == "wii") {
        //            isRealController = true;
        //            ui->EmulatedControllerSelection->setText("Real Controller");
        //            ui->ProfileSelectoin->setText("Real Controller");

        //            previousConfiguration.setValue("Profile", "real");
        //            previousConfiguration.endGroup();
        //        }
        //        else {
        //            ui->EmulatedControllerSelection->setText(arrowAdder(" "));
        //            previousConfiguration.setValue("Profile", "");
        //            isRealController = false;
        //        }
        //        previousConfiguration.endGroup();
        //        settings.endGroup();
        //        break;
        //    case 2:
        //        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //        if (isRealController) {
        //            previousConfiguration.endGroup();
        //            break;
        //        }
        //        isWiimote = !isWiimote;
        //        if (isWiimote) {
        //            ui->EmulatedControllerSelection->setText(arrowAdder("Wii Remote"));
        //            previousConfiguration.setValue("EmulatedController", "wii");
        //        } else {
        //            ui->EmulatedControllerSelection->setText(arrowAdder("Gamecube Controller"));
        //            previousConfiguration.setValue("EmulatedController", "gc");
        //        }
        //        previousConfiguration.endGroup();
        //        break;
        //    case 3:
        ////        settings.beginGroup("ProfileSuffixes");
        ////        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        ////        if (isRealController) {
        ////            previousConfiguration.endGroup();
        ////            settings.endGroup();
        ////            break;
        ////        }
        ////        profileSelector.decreaseSelection(true);
        ////        ui->ProfileSelectoin->setText(arrowAdder(settings.value(settings.childKeys().value(profileSelector.getSelection()), "fake profile").toString()));
        ////        previousConfiguration.setValue("Profile", settings.childKeys().value(profileSelector.getSelection()));
        ////        previousConfiguration.endGroup();
        ////        settings.endGroup();
        //        QFileInfoList newlist;
        //        if (isWiimote) {
        //            previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //            QDir dir("C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config\\Profiles\\Wiimote");
        //            const QFileInfoList list = dir.entryInfoList();
        //            const QStringList list2 = settings.childKeys();
        //            for (const QString &key: list2) {
        //                for (const QFileInfo &fileinfo: list) {
        //                    if (fileinfo.baseName().endsWith(key) && fileinfo.baseName().startsWith(previousConfiguration.value("RealController").toString())) {
        //                        newlist.append(fileinfo);
        //                        qDebug() << fileinfo.baseName();
        //                    }
        //                }
        //            }
        //            profileSelector.setMaxValue(newlist.count() - 1);
        //            settings.endGroup();
        //            previousConfiguration.endGroup();
        //            profileSelector.decreaseSelection(true);
        //            ui->ProfileSelectoin->setText(arrowAdder(newlist.at(profileSelector.getSelection()).baseName()));
        //        } else {
        //            settings.beginGroup("ProfileSuffixes");

        //            previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //            QDir dir("C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config\\Profiles\\GCPad");
        //            const QFileInfoList list = dir.entryInfoList();
        //            const QStringList list2 = settings.childKeys();
        //            for (const QString &key: list2) {
        //                for (const QFileInfo &fileinfo: list) {
        //                    if (fileinfo.baseName().endsWith(key) && fileinfo.baseName().startsWith(previousConfiguration.value("RealController").toString())) {
        //                        newlist.append(fileinfo);
        //                        qDebug() << fileinfo.baseName();
        //                    }
        //                }
        //            }
        //            profileSelector.setMaxValue(newlist.count() - 1);
        //            settings.endGroup();
        //            previousConfiguration.endGroup();
        //            profileSelector.decreaseSelection(true);
        //            ui->ProfileSelectoin->setText(arrowAdder(newlist.at(profileSelector.getSelection()).baseName()));
        //        }
        //    }
            selectionAction(false);
        }

        void rightPressed(bool value)
        {
            if (!value) return;
            selectionAction(true);
        //    switch (itemSelector.getSelection()) {
        //    case 0:
        //        playerSelector.increaseSelection(true);
        //        ui->PlayerSelection->setText(arrowAdder("Player " + QString::number(playerSelector.getSelection())));
        //        break;
        //    case 1:
        //        settings.beginGroup("RealControllers");
        //        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //        realControllerSelector.increaseSelection(true);
        //        ui->RealControllerSelection->setText(arrowAdder(settings.value(settings.childKeys().value(realControllerSelector.getSelection()), "fake controller").toString()));
        //        previousConfiguration.setValue("RealController", settings.childKeys().value(realControllerSelector.getSelection()));
        //        previousConfiguration.endGroup();
        //        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //        if (settings.childKeys().value(realControllerSelector.getSelection()) == "gc" || settings.childKeys().value(realControllerSelector.getSelection()) == "wii") {
        //            isRealController = true;
        //            ui->EmulatedControllerSelection->setText("Real Controller");
        //            ui->ProfileSelectoin->setText("Real Controller");

        //            previousConfiguration.setValue("Profile", "real");
        //            previousConfiguration.endGroup();
        //        }
        //        else {
        //            ui->EmulatedControllerSelection->setText(arrowAdder(" "));
        //            ui->ProfileSelectoin->setText(arrowAdder(" "));
        //            previousConfiguration.setValue("Profile", "");
        //            isRealController = false;
        //        }
        //        previousConfiguration.endGroup();
        //        settings.endGroup();
        //        break;
        //    case 2:
        //        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //        if (isRealController) {
        //            previousConfiguration.endGroup();
        //            break;
        //        }
        //        isWiimote = !isWiimote;
        //        if (isWiimote) {
        //            ui->EmulatedControllerSelection->setText(arrowAdder("Wii Remote"));
        //            previousConfiguration.setValue("EmulatedController", "wii");
        //        } else {
        //            ui->EmulatedControllerSelection->setText(arrowAdder("Gamecube Controller"));
        //            previousConfiguration.setValue("EmulatedController", "gc");
        //        }
        //        previousConfiguration.endGroup();
        //        break;
        //    case 3:

        //        QFileInfoList newlist;
        //        if (isWiimote) {
        //            previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //            QDir dir("C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config\\Profiles\\Wiimote");
        //            const QFileInfoList list = dir.entryInfoList();
        //            const QStringList list2 = settings.childKeys();
        //            for (const QString &key: list2) {
        //                for (const QFileInfo &fileinfo: list) {
        //                    if (fileinfo.baseName().endsWith(key) && fileinfo.baseName().startsWith(previousConfiguration.value("RealController").toString())) {
        //                        newlist.append(fileinfo);
        //                        qDebug() << fileinfo.baseName();
        //                    }
        //                }
        //            }
        //            profileSelector.setMaxValue(newlist.count() - 1);
        //            settings.endGroup();
        //            previousConfiguration.endGroup();
        //            profileSelector.increaseSelection(true);
        //            ui->ProfileSelectoin->setText(arrowAdder(newlist.at(profileSelector.getSelection()).baseName()));
        //        } else {
        //            settings.beginGroup("ProfileSuffixes");

        //            previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //            QDir dir("C:\\Users\\Me\\Documents\\Dolphin Emulator\\Config\\Profiles\\GCPad");
        //            const QFileInfoList list = dir.entryInfoList();
        //            const QStringList list2 = settings.childKeys();
        //            for (const QString &key: list2) {
        //                for (const QFileInfo &fileinfo: list) {
        //                    if (fileinfo.baseName().endsWith(key) && fileinfo.baseName().startsWith(previousConfiguration.value("RealController").toString())) {
        //                        newlist.append(fileinfo);
        //                        qDebug() << fileinfo.baseName();
        //                    }
        //                }
        //            }
        //            profileSelector.setMaxValue(newlist.count() - 1);
        //            settings.endGroup();
        //            previousConfiguration.endGroup();
        //            profileSelector.increaseSelection(true);
        //            ui->ProfileSelectoin->setText(arrowAdder(newlist.at(profileSelector.getSelection()).baseName()));
        //        }
        //    }





        //        settings.beginGroup("ProfileSuffixes");
        //        previousConfiguration.beginGroup("Player" + QString::number(playerSelector.getSelection()));
        //        if (isRealController) {
        //            previousConfiguration.endGroup();
        //            settings.endGroup();
        //            break;
        //        }
        //        profileSelector.increaseSelection(true);
        //        ui->ProfileSelectoin->setText(arrowAdder(settings.value(settings.childKeys().value(profileSelector.getSelection()), "fake profile").toString()));
        //        previousConfiguration.setValue("Profile", settings.childKeys().value(profileSelector.getSelection()));
        //        previousConfiguration.endGroup();
        //        settings.endGroup();
        //    }
        }



};
#endif // MAINWINDOW_H
