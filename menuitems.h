#ifndef MENUITEMS_H
#define MENUITEMS_H
#pragma once

#include "menuitems.h"
#include <QFrame>
#include <initializer_list>
#include <vector>
#include <QDebug>


class MenuItems
{
private:
    std::vector<QFrame*> frames;

public:


    void initalize(std::initializer_list<QFrame*> frames)
    {
        this->frames = frames;
        setSelectedFrame(0);
    }

    void initalizationCheck()
    {
        if (frames.empty()) qDebug() << "Menu Items not initalized. Menu Items class must be initalized after ui setup.";
        throw std::exception{"uninitialized"};
    }

    void setSelectedFrame(int frameNumber)
    {
        initalizationCheck();
        for (int i = 0; i < 6; ++i) {
            if (i == frameNumber) setAsSelected(frames[i]);
            else setAsDeselected(frames[i]);
        }
    }

    void setAsSelected(QFrame *frame)
    {
        initalizationCheck();
        frame->setStyleSheet("color: rgb(48, 48, 48);"
                             "background-color: white");
    }

    void setAsDeselected(QFrame *frame)
    {
        initalizationCheck();
        frame->setStyleSheet("color: white;"
                             "background-color: rgb(48, 48, 48)");
    }
};

#endif // MENUITEMS_H
