#ifndef MENUITEMS_H
#define MENUITEMS_H

#include <QFrame>
#include <QDebug>
#include <initializer_list>
#include <vector>



class MenuItems
{
private:
    std::vector<QFrame*> frames;

public:


    void initalize(std::initializer_list<QFrame*> frames)
    {
        this->frames = frames;
    }

    void initalizationCheck()
    {
        if (frames.empty()) {
            qDebug() << "Menu Items not initalized. Menu Items class must be initalized after ui setup.";
            throw "uninitialized class";
        }
    }

    void setSelectedFrame(unsigned int frameNumber)
    {
        initalizationCheck();
        for (unsigned int i = 0; i < frames.size(); ++i) {
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
