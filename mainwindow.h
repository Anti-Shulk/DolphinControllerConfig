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
#include <QtGamepad>
#include <QSettings>
#include "selector.h"
#include "menuitems.h"
#include "QKeyEvent"
#include <QShortcut>
#include "writer.h"
#include "SettingsManager.h"
#include "PreviousConfigurationManager.h"

// TODO: for every value function, make a defualt value
// TODO: dont modify if it wasnt modified by us
// TODO: how to launch with args
// TODO: clean comments
// TODO: clean code
// TODO: make releases executables
// TODO: make controller profiles
// TODO: make an option for none in controller select
// TODO: fix weird behavior especially whrn switching players
// TODO: seperate some stuff out into multiple files
// TODO: copy your settings.ini
// TODO: check for settings then save them
// TODO: make a thing saying controller disconnected
// TODO: real controller selector should start from 1 maybe and also list selector too maybe
// TODO: change the settings manager to enums instead of strings
// TODO: by default the path says {username}. make it so that the {username} gets auto replaced with actual username
// TODO: to see if the layout is a dolphin controler config one, check and make sure the layout has a {port} thing in it
// TODO: this needs to be done bc program will crash if it doesnt
// TODO: fix case where it cannot find the text it is lookign for bc its not htere when launching which causes infinite loop
// TODO: port number needs to be replaced multiple times sometimes

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Selector playerSelector = SelectorBuilder() // selector for player number
            .setInitalValue(1)
            .setMinValue(1)
            .setMaxValue(4)
            .setIsLooping(true)
            .build();

    Selector listSelector = SelectorBuilder() // This is the selector for the up and down movement
            .setInitalValue(5)
            .setMinValue(0)
            .setMaxValue(5)
            .setIsLooping(false)
            .build();

    Selector realControllerSelector = SelectorBuilder() // This is the selector for real controllers. We do not know how many controllers there are
            .setInitalValue(0)
            .setMinValue(0)
            .setIsLooping(true)
            .build();

    Selector portSelector = SelectorBuilder() // This is the selector for real controllers. We do not know how many controllers there are
            .setInitalValue(1)
            .setMinValue(1)
            .setMaxValue(4)
            .setIsLooping(true)
            .build();

    Selector emulatedControllerSelector = SelectorBuilder() // This for whether its a gc or wiimote
            .setInitalValue(0)
            .setMinValue(0)
            .setMaxValue(1)
            .setIsLooping(true)
            .build();

    Selector profileSelector = SelectorBuilder() // This is the selector for real controllers. We do not know how many controllers there are
            .setInitalValue(0)
            .setMinValue(0)
            .setIsLooping(true)
            .build();

    // QString realController = "SDL/0/Sony Interactive Entertainment Wireless Controller";
    QString emulatedController{};
    QString controllerProfile{};
    bool isWiimote = false;
    Ui::MainWindow *ui;
    MenuItems menuItems;
    QFrame *frames[6];
    SettingsManager settingsManager;
    PreviousConfigurationManager previousConfigManager;

public:


    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        this->showFullScreen();

        // create gamepad

        if (QGamepadManager::instance()->isGamepadConnected(0)) // this doesnt work
        {
            QGamepad *gamepad = new QGamepad(QGamepadManager::instance()->connectedGamepads().at(0), this); // this crashes on debug builds

            connect(gamepad, &QGamepad::buttonAChanged, this, &MainWindow::launchPressedCheck);
            connect(gamepad, &QGamepad::buttonUpChanged, this, &MainWindow::upPressedCheck);
            connect(gamepad, &QGamepad::buttonDownChanged, this, &MainWindow::downPressedCheck);
            connect(gamepad, &QGamepad::buttonLeftChanged, this, &MainWindow::leftPressedCheck);
            connect(gamepad, &QGamepad::buttonRightChanged, this, &::MainWindow::rightPresssedCheck);

            ui->ControllerStatusSelection->setText("Connected");
        }
        else
        {
            // say no gamepad is connected
        }


        // create shortcuts for keyboard
        QShortcut *down = new QShortcut(QKeySequence(Qt::Key_Down), this);
        QShortcut *up = new QShortcut(QKeySequence(Qt::Key_Up), this);
        QShortcut *left = new QShortcut(QKeySequence(Qt::Key_Left), this);
        QShortcut *right = new QShortcut(QKeySequence(Qt::Key_Right), this);
        QShortcut *enter = new QShortcut(QKeySequence(Qt::Key_Space), this);

        // connect 'activated' signal of shortcuts to MainWindow functions
        QObject::connect(down, &QShortcut::activated, this, &MainWindow::downPressed);
        QObject::connect(up, &QShortcut::activated, this, &MainWindow::upPressed);
        QObject::connect(left, &QShortcut::activated, this, &MainWindow::leftPressed);
        QObject::connect(right, &QShortcut::activated, this, &MainWindow::rightPressed);
        QObject::connect(enter, &QShortcut::activated, this, &MainWindow::launchPressed);





        menuItems.initalize(std::initializer_list<QFrame*>{ui->PlayerFrame, ui->RealControllerFrame, ui->PortFrame, ui->EmulatedControllerFrame, ui->ProfileFrame, ui->LaunchFrame});

        menuItems.setSelectedFrame(5);


        // This line basically says how many real contorllers we have so we know how long to make the real contorller list
        realControllerSelector.setMaxValue(settingsManager.getKeys("RealControllers").length() - 1); // we do -1 becuase we count our array starting from 0 meaning that if there are 4 keys, 3 would be the max value

        loadCurrentPlayerConfig();
    }

    ~MainWindow()
    {
        delete ui;
    }


    void resetControllerProfiles()
    {
        QFile dolphinWiimoteNewSettings(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/" + "WiimoteNew.ini");
        QTextStream dolphinWiimoteNewSettingsStream(&dolphinWiimoteNewSettings);
        QFile dolphinGCPadNewSettings(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/" + "GCPadNew.ini");
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

        dolphinWiimoteNewSettings.close();
        dolphinGCPadNewSettings.close();


    }




    QString arrowAdder(QString arrowlessString)
    {
        return "⯇ " + arrowlessString + " ⯈";
    }


    QString getPlayerNumber()
    {
        return QString::number(playerSelector.getSelection());
    }

    void setRealController(QString key)
    {
        ui->RealControllerSelection->setText(arrowAdder(settingsManager.getSetting("RealControllers", key)));
        previousConfigManager.setPlayerConfig(playerSelector.getSelection(), "RealController", key);
    }

    void setEmulatedControllerWithoutArrows(int selection)
    {
        if (selection == 1) {
            previousConfigManager.setPlayerConfig(playerSelector.getSelection(), "EmulatedController", "wii");
            ui->EmulatedControllerSelection->setText("Wii Remote");
        }
        else {
            previousConfigManager.setPlayerConfig(playerSelector.getSelection(), "EmulatedController", "gc");
            ui->EmulatedControllerSelection->setText("GameCube Controller");
        }
    }

    void setEmulatedController(int selection)
    {
        if (selection == 1) {
            previousConfigManager.setPlayerConfig(playerSelector.getSelection(), "EmulatedController", "wii");
            ui->EmulatedControllerSelection->setText(arrowAdder("Wii Remote"));
        }
        else {
            previousConfigManager.setPlayerConfig(playerSelector.getSelection(), "EmulatedController", "gc");
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

    void setPort(int portNumber)
    {
        ui->PortSelection->setText(arrowAdder(QString::number(portNumber)));
        previousConfigManager.setPlayerConfig(playerSelector.getSelection(), "Port", QString::number(portNumber));
    }

    void setCurrentPort()
    {
        setPort(portSelector.getSelection());
    }



    void setProfile(QString profile)
    {
        previousConfigManager.setPlayerConfig(playerSelector.getSelection(), "Profile", profile);
        ui->ProfileSelectoin->setText(arrowAdder(profile));
    }


    bool isRealController()
    {
        return (settingsManager.getKeys("RealControllers").value(realControllerSelector.getSelection()) == "gc") || (settingsManager.getKeys("RealControllers").value(realControllerSelector.getSelection()) == "wii");
    }


    void setPlayerNumber()
    {
        ui->PlayerSelection->setText(arrowAdder("Player " + getPlayerNumber()));
    }

    void loadCurrentPlayerConfig()
    {
        setRealController(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController")); // update real controller

        setPort(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "Port").toInt()); // update port

        if (previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController") == "gc") { // if the real controllers are wii and gamecube, update emulated controllers and profile
            setEmulatedControllerWithoutArrows(0);
            setProfile("realgc");
            ui->ProfileSelectoin->setText("Real Controller");
            return;
        } else if (previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController") == "wii") {
            setEmulatedControllerWithoutArrows(1);
            setProfile("realwii");
            ui->ProfileSelectoin->setText("Real Controller");
            return;
        } else {
            setEmulatedController(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "EmulatedController"));
            setProfile(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "Profile"));
        }


    }


    void selectionAction(Selector::Direction direction)
    {
        switch (listSelector.getSelection()) { // Which up/down level are we on?
        default:
        break;
        case 0: /* Players */
            playerSelector.modifySelection(direction); // update player selector
            setPlayerNumber(); // update ui
            loadCurrentPlayerConfig(); // update all other ui elements
        break;
        case 1: /* Real Controllers*/
            realControllerSelector.modifySelection(direction);
            setRealController(settingsManager.getKeys("RealControllers").at(realControllerSelector.getSelection())); // update real controller

            if (settingsManager.getKeys("RealControllers").value(realControllerSelector.getSelection()) == "gc") {
                setEmulatedControllerWithoutArrows(0);
                setProfile("realgc");
                ui->ProfileSelectoin->setText("Real Controller");
                break;
            } else if (settingsManager.getKeys("RealControllers").value(realControllerSelector.getSelection()) == "wii") {
                setEmulatedControllerWithoutArrows(1);
                setProfile("realwii");
                ui->ProfileSelectoin->setText("Real Controller");
                break;
            }
            ui->EmulatedControllerSelection->setText(arrowAdder(" "));
            ui->ProfileSelectoin->setText(arrowAdder(" "));
        break;
        case 2: /* Ports */
            portSelector.modifySelection(direction); // loops
            setCurrentPort();
        break;
        case 3: /* Emulated Controllers */
            if (isRealController()) break;
            emulatedControllerSelector.modifySelection(direction); //loops
            setCurrentEmulatedController();
        break;
        case 4:
            QDir dir;
            QFileInfoList usableProfiles;
            if (previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "EmulatedController") == "gc") dir = QDir(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/GCPad");
            else dir = QDir(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/Wiimote");

            const QFileInfoList possibleFiles = dir.entryInfoList();
            const QStringList suffixes = settingsManager.getKeys("ProfileSuffixes");

            for (const QString &suffix: suffixes) {
                for (const QFileInfo &file: possibleFiles) {
                    if (file.baseName().startsWith(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController")) && file.baseName().endsWith(suffix)) {
                        usableProfiles.append(file);
                    }
                }
            }
            if (usableProfiles.empty()) break;

            profileSelector.setMaxValue(usableProfiles.count() - 1);
            profileSelector.modifySelection(direction);
            setProfile(usableProfiles.at(profileSelector.getSelection()).baseName());
        break;
        }
    }


    void launch()
    {

        // In these two functions, we cannot use the standard QSettings to interface with the dolphin controller profiles
        // becuase they do not allow spaces or forward slashes in file names easily. instead, I am using file text streams to put the text into the files.
        resetControllerProfiles();

        Writer gcPad(this, Writer::REPLACE_TEXT_ONLY, settingsManager.getSetting("Paths", "dolphinconfigpath") + "/" + "GCPadNew.ini");
        Writer wiiMote(this, Writer::REPLACE_TEXT_ONLY, settingsManager.getSetting("Paths", "dolphinconfigpath") + "/" + "WiimoteNew.ini");
        Writer dolphinConf(this, Writer::REPLACE_FULL_LINE, settingsManager.getSetting("Paths", "dolphinconfigpath") + "/" + "Dolphin.ini");

        for (int playerNumber = 1; playerNumber <= 4; playerNumber++) { // for each player
            if (previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "EmulatedController") == "gc") {

                // First replace the section of GC pad with our profile instead
                gcPad.replaceFileTextAndSkipLines(
                            "[GCPad" + QString::number(playerNumber) + "]", // skip start
                            playerNumber == 4 ? "\n\n\n": "[GCPad" + QString::number(playerNumber+1) + "]", // skip end
                            FileReader::fileToString(this, settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/GCPad" + "/" + previousConfigManager.getPlayerConfig(playerNumber, "Profile") + ".ini") // Replace with whatever profile name our previous configuration has selected
                                .remove("[Profile]\n") // once we converted the file to a string, we remove [Profile] and replae the lines
                            );



                // Replace {portNumber} with the port number in our new config file
                gcPad.replaceFileText(
                            "{portNumber}", // to replace
                            QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Port").toInt() - 1) // to replpace with
                            );

                // Edit SIDevice in dolphin config file
                dolphinConf.replaceFileText(
                            "SIDevice" + QString::number(playerNumber-1), // replaace text containing SIDevice of our port number
                            "SIDevice" + QString::number(playerNumber-1) + " = " + QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Profile") == "realgc" ? 12: 6) // set the device to 12 if its a realgc or 6 if its emulated
                            );
//                qDebug() << "hey";

                // Edit Wiimote to remove Wiimote for this port
                wiiMote.replaceFileTextAndSkipLines(
                            "[Wiimote" + QString::number(playerNumber) + "]", // to replace start
                            playerNumber == 4 ? "[BalanceBoard]" : "[Wiimote" + QString::number(playerNumber+1) + "]", // to replace end
                            "Source = 0\n" // to replace with
                            );

            } else {
                // First replace the section of wiimote  with our profile instead
                wiiMote.replaceFileTextAndSkipLines(
                            "[Wiimote" + QString::number(playerNumber) + "]", // skip start
                            playerNumber == 4 ? "[BalanceBoard]" : "[Wiimote" + QString::number(playerNumber+1) + "]", // skip end
                            FileReader::fileToString(this, settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/Wiimote" + "/" +
                                                     previousConfigManager.getPlayerConfig(playerNumber, "Profile") + ".ini").remove("[Profile]\n") // Replace with whatever profile name our previous configuration has selected
                            );

                // Replace {portNumber} with the port number in our new config file
                wiiMote.replaceFileText(
                            "{portNumber}", // to replace
                            QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Port").toInt() - 1) // to replpace with
                            );

                // Edit Wiimote to use real wiimote if its real or not

                wiiMote.replaceFileText("[Wiimote" + QString::number(playerNumber) + "]",
                                        "[Wiimote" + QString::number(playerNumber) + "]" + "\n" + "Source = " +
                                                    QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Profile") == "realwii" ? 2: 1)
                                        );

                // Edit SIDevice to remove gc controller
                dolphinConf.replaceFileText(
                            "SIDevice" + QString::number(playerNumber-1),
                            "SIDevice" + QString::number(playerNumber-1) + " = 0\n"
                            );
            }
        }
            QProcess::startDetached(settingsManager.getSetting("Paths", "dolphinpath"), QStringList());
            QApplication::quit();
    }



private slots:
    void launchPressedCheck(bool value)
    {
        if (!value) return;
        launchPressed();
    }
    void launchPressed()
    {
        if (listSelector.getSelection() != 5) return;
        launch();
    }

    void upPressedCheck(bool value)
    {
        if (!value) return;
        upPressed();
    }
    void upPressed()
    {
        listSelector.decreaseSelection(); // decrease the selector
        menuItems.setSelectedFrame(listSelector.getSelection()); // update the visuals to match
    }

    void downPressedCheck(bool value)
    {
        if (!value) return;
        downPressed();
    }
    void downPressed()
    {
        listSelector.increaseSelection(); // increase the selector
        menuItems.setSelectedFrame(listSelector.getSelection()); // update the visuals to match
    }

    void leftPressedCheck(bool value)
    {
        if (!value) return;
        leftPressed();
    }
    void leftPressed()
    {
        selectionAction(Selector::Decrease);
    }

    void rightPresssedCheck(bool value)
    {
        if (!value) return;
        rightPressed();
    }
    void rightPressed()
    {
        selectionAction(Selector::Increase);
    }

};
#endif // MAINWINDOW_H
