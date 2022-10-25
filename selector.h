#ifndef SELECTOR_H
#define SELECTOR_H
#pragma once


class Selector
{
public:
    int selection;

    int maxValue;

    int minValue;

public:
    Selector(int initialValue, int minValue, int maxValue);

    int getSelection();

    void setSelection(int value);

    void setMaxValue(int value);

    void setMinValue(int value);

    void increaseSelection();

    void increaseSelection(bool loop);

    void modifySelection(bool increasing, bool loop);

    void decreaseSelection();

    void decreaseSelection(bool loop);
};

#endif // SELECTOR_H
