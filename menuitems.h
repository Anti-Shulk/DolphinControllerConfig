#ifndef MENUITEMS_H
#define MENUITEMS_H

#include <QFrame>
#include <initializer_list>
#include <array>

#include "ui_mainwindow.h"

class MenuItems
{
private:
    QFrame* frames[7];
    void setAsSelected(QFrame* frame)
    {
        frame->setStyleSheet("color: rgb(48, 48, 48);"
                             "background-color: white");
    }

    void setAsDeselected(QFrame* frame)
    {
        frame->setStyleSheet("color: white;"
                             "background-color: rgb(48, 48, 48)");
    }

public:
    MenuItems(Ui::MainWindow* ui)
        : frames{ui->DolphinConfigWindowFrame, ui->PlayerFrame, ui->RealControllerFrame, ui->PortFrame, ui->EmulatedControllerFrame, ui->ProfileFrame, ui->LaunchFrame}
    { }

    void setSelectedFrame(unsigned int frameNumber)
    {
        for (unsigned int i = 0; i < std::size(frames); ++i) {
            if (i == frameNumber) setAsSelected(frames[i]);
            else setAsDeselected(frames[i]);
        }
    }


};

#endif // MENUITEMS_H
