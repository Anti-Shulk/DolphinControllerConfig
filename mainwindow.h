#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGamepadManager>
#include <QFrame>
#include <QFile>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QtGamepad>
#include <QSettings>
#include <QShortcut>
#include <QThread>
#include <memory>

#include "ui_mainwindow.h"

#include "selector.h"
#include "menuitems.h"
#include "QKeyEvent"
#include "writer.h"
#include "SettingsManager.h"
#include "PreviousConfigurationManager.h"


// TODO: for every value function, make a defualt value
// TODO: dont modify if it wasnt modified by us
// TODO: how to launch with args
// TODO: clean comments
// TODO: clean code
// TODO: make releases executables
//// TODO: make controller profiles
// TODO: make an option for none in controller select
// TODO: fix weird behavior especially whrn switching players
// TODO: seperate some stuff out into multiple files
// TODO: copy your settings.ini
// TODO: check for settings then save them
// TODO: make a thing saying controller disconnected
// TODO: real controller selector should start from 1 maybe and also list selector too maybe
//// TODO: change the settings manager to enums instead of strings
// rename exe file
// TODO: by default the path says {username}. make it so that the {username} gets auto replaced with actual username
// TODO: to see if the layout is a dolphin controler config one, check and make sure the layout has a {port} thing in it
// TODO: this needs to be done bc program will crash if it doesnt
// TODO: fix case where it cannot find the text it is lookign for bc its not htere when launching which causes infinite loop
// TODO: port number needs to be replaced multiple times sometimes
// TODO: make a do not write config
// TODO: FOOOOONNNTTTT

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
            .setInitalValue(6)
            .setMinValue(0)
            .setMaxValue(6)
            .setIsLooping(false)
            .build();

    Selector realControllerSelector = SelectorBuilder() // This is the selector for real controllers. We do not know how many controllers there are
            .setInitalValue(0)
            .setMinValue(0)
            .setIsLooping(true)
            .build();

    Selector portSelector = SelectorBuilder() // This is the selector for real controllers. We do not know how many controllers there are
            .setInitalValue(0)
            .setMinValue(0)
            .setMaxValue(3)
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
    QFrame* frames[6];
    SettingsManager settingsManager;
    PreviousConfigurationManager previousConfigManager;

    std::unique_ptr<QGamepad>gamepad;

    std::unique_ptr<QShortcut> up;
    std::unique_ptr<QShortcut> down;
    std::unique_ptr<QShortcut> left;
    std::unique_ptr<QShortcut> right;
    std::unique_ptr<QShortcut> enter;
    QStringList args;


public:


    MainWindow(double scaleFactor, int height, int width, QStringList args, QWidget *parent = nullptr) : QMainWindow(parent) , ui(new Ui::MainWindow), args(args)
    {
        if (settingsManager.getSetting("Setup", "setup") != "true") {
            settingsManager.setSetting("Setup", "setup", "false");
            settingsManager.setSetting("RealControllers", "ex", "Example Controller");
            settingsManager.setSetting("ProfileSuffixes", "examplesuffix", "with example suffix");
            settingsManager.setSetting("Paths", "dolphinconfigpath", "[Enter Dophin Config Path]");
            settingsManager.setSetting("Paths", "dolphinpath", "[Enter Path to launch Dolphin]");
            QMessageBox::warning(this, tr("Warning"), tr("DolphinControllerConfigurator has not been setup. Please read README.md. "
                                                         "For more information see https://github.com/Anti-Shulk/DolphinControllerConfig"));
            QApplication::quit();
            delete this;
            return;
        }



        ui->setupUi(this);

        this->showFullScreen();


        /* Scaling code */
        int labelFontSize = 18;
        int selectionFontSize = 16;
        double widthScale = width/1920.0;
        double heightScale = height/1080.0;

        if (width / height < 1) { // if monitor is taller than wide
            labelFontSize = 18.0 * (100.0 / scaleFactor) * widthScale;
            selectionFontSize = 16.0 * (100.0 / scaleFactor) * widthScale;
        } else {
            labelFontSize = 18.0 * (100.0 / scaleFactor) * heightScale;
            selectionFontSize = 16.0 * (100.0 / scaleFactor) * heightScale;
        }


        QFrame* frames[] = {
            ui->DolphinConfigWindowFrame,
            ui->EmulatedControllerFrame,
            ui->PlayerFrame,
            ui->ProfileFrame,
            ui->LaunchFrame,
            ui->PortFrame,
            ui->RealControllerFrame
        };

        for (QFrame* frame : frames) {
            QRect originalGeometry = frame->geometry();
            frame->setGeometry(
                        originalGeometry.x() * widthScale,
                        originalGeometry.y() * heightScale,
                        originalGeometry.width() * widthScale,
                        originalGeometry.height() * heightScale
                        );
        }

        QWidget* Widgets[] = {
            ui->DolphinConfigWindowWidget,
            ui->PlayerWidget,
            ui->EmulatedControllerWidget,
            ui->RealControllerWidget,
            ui->ProfileWidget,
            ui->PortWidget,
            ui->LaunchWidget
        };

        for (QWidget* widget : Widgets) {
            QRect originalGeometry = widget->geometry();
            widget->setGeometry(
                        originalGeometry.x() * widthScale,
                        originalGeometry.y() * heightScale,
                        originalGeometry.width() * widthScale,
                        originalGeometry.height() * heightScale
                        );
        }

        QLabel* labels[] = {
            ui->DolphinConfigWindowLabel,
            ui->PlayerLabel,
            ui->EmulatedControllerLabel,
            ui->RealControllerLabel,
            ui->ProfileLabel,
            ui->PortLabel,
            ui->LaunchLabel
        };

        for (QLabel* label : labels) {
            label->setFont(QFont("Exo 2 Condensed", labelFontSize));
        }


        QLabel* selections[] = {
            ui->DolphinConfigWIndowSelection,
            ui->PlayerSelection,
            ui->EmulatedControllerSelection,
            ui->RealControllerSelection,
            ui->ProfileSelectoin,
            ui->PortSelection,
        };

        for (QLabel* selection : selections) {
            selection->setFont(QFont("Exo 2 Condensed", selectionFontSize));
        }

        /* Scaling Code end */




        // create gamepad
        gamepad = std::make_unique<QGamepad>(QGamepadManager::instance()->connectedGamepads().at(0), this); // this crashes on debug builds

        connect(gamepad.get(), &QGamepad::buttonAChanged, this, &MainWindow::launchPressedCheck);
        connect(gamepad.get(), &QGamepad::buttonUpChanged, this, &MainWindow::upPressedCheck);
        connect(gamepad.get(), &QGamepad::buttonDownChanged, this, &MainWindow::downPressedCheck);
        connect(gamepad.getIO, &QGamepad::buttonLeftChanged, this, &MainWindow::leftPressedCheck);
        connect(gamepad, &QGamepad::buttonRightChanged, this, &::MainWindow::rightPresssedCheck);


        // create shortcuts for keyboard
        down = std::make_unique<QShortcut>(QKeySequence(Qt::Key_Down), this);
        up = std::make_unique<QShortcut>(QKeySequence(Qt::Key_Up), this);
        left = std::make_unique<QShortcut>(QKeySequence(Qt::Key_Left), this);
        right = std::make_unique<QShortcut>(QKeySequence(Qt::Key_Right), this);
        enter = std::make_unique<QShortcut>(QKeySequence(Qt::Key_Space), this);

        // connect 'activated' signal of shortcuts to MainWindow functions
        QObject::connect(down, &QShortcut::activated, this, &MainWindow::downPressed);
        QObject::connect(up, &QShortcut::activated, this, &MainWindow::upPressed);
        QObject::connect(left, &QShortcut::activated, this, &MainWindow::leftPressed);
        QObject::connect(right, &QShortcut::activated, this, &MainWindow::rightPressed);
        QObject::connect(enter, &QShortcut::activated, this, &MainWindow::launchPressed);





        menuItems.initalize(std::initializer_list<QFrame*>{ui->DolphinConfigWindowFrame, ui->PlayerFrame, ui->RealControllerFrame, ui->PortFrame, ui->EmulatedControllerFrame, ui->ProfileFrame, ui->LaunchFrame});

        menuItems.setSelectedFrame(6);


        // This line basically says how many real contorllers we have so we know how long to make the real contorller list
        realControllerSelector.setMaxValue(settingsManager.getKeys("RealControllers").length() - 1); // we do -1 becuase we count our array starting from 0 meaning that if there are 4 keys, 3 would be the max value

        loadCurrentPlayerConfig();



        bool found = false;
        for (int i = 0; i < args.length(); i++) {
            if (args.at(i) == "--batch" || args.at(i) == "-b") {
                found = true;
                ui->DolphinConfigWIndowSelection->setText(arrowAdder("Disabled"));
            }
        }

        if (!found) {
            ui->DolphinConfigWIndowSelection->setText(arrowAdder("Enabled"));
        }
    }

    ~MainWindow()
    {
        delete up;
        delete down;
        delete left;
        delete right;
        delete enter;
        delete gamepad;
        delete ui;
    }



    void resetControllerProfiles()
    {
        QFile dolphinWiimoteNewSettings(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/" + "WiimoteNew.ini");
        QTextStream dolphinWiimoteNewSettingsStream(&dolphinWiimoteNewSettings);
        QFile dolphinGCPadNewSettings(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/" + "GCPadNew.ini");
        QTextStream dolphinGCPadNewSettingsStream(&dolphinGCPadNewSettings);

        if (!dolphinWiimoteNewSettings.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Warning"), tr("Unable to open Dolphin Emulator's WiimoteNew.ini. Make sure your paths are configured incorrectly. "
                                                         "For more information, please read README.md or see "
                                                         "https://github.com/Anti-Shulk/DolphinControllerConfig"
                                                         "Error: ") + dolphinWiimoteNewSettings.errorString());
            QApplication::quit();
            delete this;
            return;
        }

        if (!dolphinGCPadNewSettings.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Warning"), tr("Unable to open Dolphin Emulator's GCPadNew.ini. Make sure your paths are configured incorrectly. "
                                                         "For more information, please read README.md or see "
                                                         "https://github.com/Anti-Shulk/DolphinControllerConfig"
                                                         "Error: ") + dolphinGCPadNewSettings.errorString());
            QApplication::quit();
            delete this;
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

        if (profile == "realwii" || profile == "realgc") {
            ui->ProfileSelectoin->setText("Real Controller");
            return;
        }

//        QString prefix = settingsManager.getSetting("RealControllers", previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController"));
        QString suffix = settingsManager.getSetting("ProfileSuffixes", profile.remove(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController")));

        ui->ProfileSelectoin->setText(arrowAdder(suffix));
    }


    bool isRealController()
    {
        return (previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController") == "gc" || previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController") == "wii");
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
            return;
        }
        if (previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController") == "wii") {
            setEmulatedControllerWithoutArrows(1);
            setProfile("realwii");
            return;
        }
        setEmulatedController(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "EmulatedController"));

        if ((previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "Profile") == "none")) {
            ui->ProfileSelectoin->setText("No Profiles Match Criteria");
            return;
        }

        setProfile(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "Profile"));


    }


    void selectionAction(Selector::Direction direction, int listLevel)
    {
        switch (listLevel) { // Which up/down level are we on?
        default:
        break;
        case 0:
            {
                bool found = false;
                for (int i = 0; i < args.length(); i++) {
                    if (args.at(i) == "--batch" || args.at(i) == "-b") {
                        found = true;
                        args.removeAt(i);
                        ui->DolphinConfigWIndowSelection->setText(arrowAdder("Enabled"));
                    }
                }
                if (!found) {
                    args.append("--batch");
                    ui->DolphinConfigWIndowSelection->setText(arrowAdder("Disabled"));
                }
            }

            break;
        case 1: /* Players */
            playerSelector.modifySelection(direction); // update player selector
            setPlayerNumber(); // update ui
            loadCurrentPlayerConfig(); // update all other ui elements
        break;
        case 2: /* Real Controllers*/
            realControllerSelector.modifySelection(direction);
            setRealController(settingsManager.getKeys("RealControllers").at(realControllerSelector.getSelection())); // update real controller

            if (settingsManager.getKeys("RealControllers").value(realControllerSelector.getSelection()) == "gc") {
                setEmulatedControllerWithoutArrows(0);
                setProfile("realgc");
                break;
            } else if (settingsManager.getKeys("RealControllers").value(realControllerSelector.getSelection()) == "wii") {
                setEmulatedControllerWithoutArrows(1);
                setProfile("realwii");
                break;
            }
            ui->EmulatedControllerSelection->setText(arrowAdder(" "));
            ui->ProfileSelectoin->setText(arrowAdder(" "));
            selectionAction(Selector::Increase, 4);
        break;
        case 3: /* Ports */
            portSelector.modifySelection(direction); // loops
            setCurrentPort();
        break;
        case 4: /* Emulated Controllers */
            if (isRealController()) break;
            emulatedControllerSelector.modifySelection(direction); //loops
            setCurrentEmulatedController();
            selectionAction(Selector::Increase, 5);
        break;
        case 5:
            QDir dir;
            QFileInfoList usableProfiles;
            if (previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "EmulatedController") == "gc") dir = QDir(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/GCPad");
            else dir = QDir(settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/Wiimote");

            const QFileInfoList possibleFiles = dir.entryInfoList();
            const QStringList suffixes = settingsManager.getKeys("ProfileSuffixes");

            for (const QString &suffix: suffixes) {
                for (const QFileInfo &fileInfo: possibleFiles) {
                    if (fileInfo.baseName().startsWith(previousConfigManager.getPlayerConfig(playerSelector.getSelection(), "RealController")) && fileInfo.baseName().endsWith(suffix)) {
                        QFile file(fileInfo.filePath());

                        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                                QMessageBox::warning(this, tr("Warning"), tr("Unable to open file : ") + file.errorString() +
                                                     "The file path is: " + fileInfo.filePath() +
                                                     "Make sure your paths are configured incorrectly. "
                                                     "For more information, please read README.md or see "
                                                     "https://github.com/Anti-Shulk/DolphinControllerConfig");
                                QApplication::quit();
                                delete this;
                                return;
                        }

                        QTextStream stream(&file);
                        stream.seek(0);

                        QString line;
                        bool isValid = false;
                        // go through the original stream and replace the text with the text that is being requested to be replaced
                        while (!stream.atEnd()) {
                            line = stream.readLine();
                            if (line.contains("{PortNumber")) { // If it contains the text we want to replace
                                isValid = true;
                            }
                        }
                        if (isValid) {
                            usableProfiles.append(fileInfo);
                        }
                        file.close();
                    }
                }
            }
            if (usableProfiles.empty()) {
                if (isRealController()) break;
                setProfile("none");
                ui->ProfileSelectoin->setText("No Profiles Match Criteria");
                break;
            }

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
            if (previousConfigManager.getPlayerConfig(playerNumber, "EmulatedController") == "gc") {

                // Edit Wiimote to remove Wiimote for this port
                wiiMote.replaceFileTextAndSkipLines(
                            "[Wiimote" + QString::number(playerNumber) + "]", // to replace start
                            playerNumber == 4 ? "[BalanceBoard]" : "[Wiimote" + QString::number(playerNumber+1) + "]", // to replace end
                            "Source = 0\n" // to replace with
                            );

                if (previousConfigManager.getPlayerConfig(playerNumber, "Profile") == "none")
                {
                    // Edit SIDevice in dolphin config file to 0 bc this controller doesnt have a profile
                    dolphinConf.replaceFileText(
                                "SIDevice" + QString::number(playerNumber-1), // replaace text containing SIDevice of our port number
                                "SIDevice" + QString::number(playerNumber-1) + " = 0" // set the device to 12 if its a realgc or 6 if its emulated
                                );
                    continue;
                }


                 // First replace the section of GC pad with our profile instead
                gcPad.replaceFileTextAndSkipLines(
                            "[GCPad" + QString::number(playerNumber) + "]", // skip start
                            playerNumber == 4 ? "\n\n\n": "[GCPad" + QString::number(playerNumber+1) + "]", // skip end
                            FileReader::fileToString(this, settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/GCPad" + "/" + previousConfigManager.getPlayerConfig(playerNumber, "Profile") + ".ini") // Replace with whatever profile name our previous configuration has selected
                                .remove("[Profile]\n") // once we converted the file to a string, we remove [Profile] and replae the lines
                            );



                // Replace {PortNumber} with the port number in our new config file
                gcPad.replaceFileText(
                            "{PortNumber}", // to replace
                            QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Port").toInt()) // to replpace with
                            );

                // Edit SIDevice in dolphin config file
                dolphinConf.replaceFileText(
                            "SIDevice" + QString::number(playerNumber-1), // replaace text containing SIDevice of our port number
                            "SIDevice" + QString::number(playerNumber-1) + " = " + QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Profile") == "realgc" ? 12: 6) // set the device to 12 if its a realgc or 6 if its emulated
                            );



            } else {
                // Edit SIDevice to remove gc controller
                dolphinConf.replaceFileText(
                            "SIDevice" + QString::number(playerNumber-1),
                            "SIDevice" + QString::number(playerNumber-1) + " = 0"
                            );

                if (previousConfigManager.getPlayerConfig(playerNumber, "Profile") == "none")
                {
                    // Edit wiimote to 0 bc this controller doesnt have a profile
                    wiiMote.replaceFileTextAndSkipLines(
                                "[Wiimote" + QString::number(playerNumber) + "]", // to replace start
                                playerNumber == 4 ? "[BalanceBoard]" : "[Wiimote" + QString::number(playerNumber+1) + "]", // to replace end
                                "Source = 0\n" // to replace with
                                );
                    continue;
                }


                // First replace the section of wiimote  with our profile instead
                wiiMote.replaceFileTextAndSkipLines(
                            "[Wiimote" + QString::number(playerNumber) + "]", // skip start
                            playerNumber == 4 ? "[BalanceBoard]" : "[Wiimote" + QString::number(playerNumber+1) + "]", // skip end
                            FileReader::fileToString(this, settingsManager.getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/Wiimote" + "/" +
                                                     previousConfigManager.getPlayerConfig(playerNumber, "Profile") + ".ini").remove("[Profile]\n") // Replace with whatever profile name our previous configuration has selected
                            );

                // Replace {PortNumber} with the port number in our new config file
                wiiMote.replaceFileText(
                            "{PortNumber}", // to replace
                            QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Port").toInt()) // to replpace with
                            );

                // Edit Wiimote to use real wiimote if its real or not

                wiiMote.replaceFileText("[Wiimote" + QString::number(playerNumber) + "]",
                                        "[Wiimote" + QString::number(playerNumber) + "]" + "\n" + "Source = " +
                                                    QString::number(previousConfigManager.getPlayerConfig(playerNumber, "Profile") == "realwii" ? 2: 1)
                                        );


            }
        }

//            QProcess::startDetached(settingsManager.getSetting("Paths", "dolphinpath"), QStringList("--exec=C:/Users/Justi/Documents/Games/Console/Nintendo GameCube/Super Smash Bros. Melee (USA).iso"));
        if (!QProcess::startDetached(settingsManager.getSetting("Paths", "dolphinpath"), args)) {
            QMessageBox::warning(this, tr("Warning"), tr("Dolphin.exe not found. Make sure your paths are configured incorrectly. "
                                                         "For more information, please read README.md or see "
                                                         "https://github.com/Anti-Shulk/DolphinControllerConfig"));
            QApplication::quit();
            delete this;
            return;
        }
        QThread::sleep(5);
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
        if (listSelector.getSelection() != 6) return;
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
        selectionAction(Selector::Decrease, listSelector.getSelection());
    }

    void rightPresssedCheck(bool value)
    {
        if (!value) return;
        rightPressed();
    }
    void rightPressed()
    {
        selectionAction(Selector::Increase, listSelector.getSelection());
    }

};
#endif // MAINWINDOW_H
