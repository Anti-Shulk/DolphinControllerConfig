#ifndef CONFIGWRITER_H
#define CONFIGWRITER_H
#pragma once
#include "writer.h"

class ConfigWriter {
private:

public:
    void writeConfigFiles()
    {
        Writer gcPad(this, Writer::REPLACE_TEXT_ONLY, getSetting("Paths", "dolphinconfigpath") + "GCPadNew.ini");
        Writer wiiMote(this, Writer::REPLACE_TEXT_ONLY, getSetting("Paths", "dolphinconfigpath") + "WiimoteNew.ini");
        Writer dolphinConf(this, Writer::REPLACE_FULL_LINE, getSetting("Paths", "dolphinconfigpath") + "Dolphin.ini");

        for (int playerNumber = 1; playerNumber <= 4; playerNumber++) { // for each player

            previousConfiguration.beginGroup("Player" + QString::number(playerNumber));
            if (getPlayerConfig(playerNumber, "EmulatedController") == "gc") {

                // First replace the section of GC pad with our profile instead
                gcPad.replaceFileTextAndSkipLines(
                            "[GCPad" + QString::number(playerNumber) + "]", // skip start
                            playerNumber == 4 ? "\n\n\n": "[GCPad" + QString::number(playerNumber+1) + "]", // skip end
                            FileReader::fileToString(this, getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/GCPad" + "/" + previousConfiguration.value("Profile").toString() + ".ini") // Replace with whatever profile name our previous configuration has selected
                                .remove("[Profile]\n")
                            );


                // Replace {portNumber} with the port number in our new config file
                gcPad.replaceFileText(
                            "{portNumber}", // to replace
                            QString::number(getPlayerConfig(playerNumber, "Port").toInt() - 1) // to replpace with
                            );

                // Edit SIDevice in dolphin config file
                dolphinConf.replaceFileText(
                            "SIDevice" + QString::number(playerNumber-1), // replaace text containing SIDevice of our port number
                            "SIDevice" + QString::number(playerNumber-1) + " = " + QString::number(previousConfiguration.value("Profile").toString() == "realgc" ? 12: 6) // set the device to 12 if its a realgc or 6 if its emulated
                            );

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
                            FileReader::fileToString(this, getSetting("Paths", "dolphinconfigpath") + "/Profiles" + "/Wiimote" + "/" +
                                                     previousConfiguration.value("Profile").toString() + ".ini").remove("[Profile]\n") // Replace with whatever profile name our previous configuration has selected
                            );

                // Replace {portNumber} with the port number in our new config file
                wiiMote.replaceFileText(
                            "{portNumber}", // to replace
                            QString::number(previousConfiguration.value("Port").toInt() - 1) // to replpace with
                            );

                // Edit Wiimote to use real wiimote if its real or not

                wiiMote.replaceFileText("[Wiimote" + QString::number(playerNumber) + "]",
                                        "[Wiimote" + QString::number(playerNumber) + "]" + "\n" + "Source = " +
                                                    QString::number(previousConfiguration.value("Profile").toString() == "realwii" ? 2: 1)
                                        );

                // Edit SIDevice to remove gc controller
                dolphinConf.replaceFileText(
                            "SIDevice" + QString::number(playerNumber-1),
                            "SIDevice" + QString::number(playerNumber-1) + " = 0\n"
                            );
            }

            previousConfiguration.endGroup();
        }
    }
};

#endif // CONFIGWRITER_H
