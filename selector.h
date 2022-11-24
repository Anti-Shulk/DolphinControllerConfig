#ifndef SELECTOR_H
#define SELECTOR_H
#pragma once


class Selector
{
private:
    const bool isLooping;
    int minValue;
    int maxValue;
    int increasingResetValue;
    int decreasingResetValue;
    int selection;

public:
    Selector(int initialValue, int minValue, int maxValue, bool isLooping) :
        isLooping(isLooping),
        minValue(minValue),
        maxValue(maxValue),
        increasingResetValue(isLooping ? minValue : maxValue),
        decreasingResetValue(isLooping ? maxValue : minValue),
        selection(initialValue)
    {}

    enum Direction {
        Increase, Decrease
    };

    void setMaxValue(int maxValue)
    {
        this->maxValue = maxValue;
        increasingResetValue = isLooping ? minValue : maxValue;
        decreasingResetValue = isLooping ? maxValue : minValue;
    }


    int getSelection()
    {
        return selection;
    }

    void setSelection(int value)
    {
        selection = value;
    }

    void increaseSelection()
    {
        selection = selection >= maxValue ? increasingResetValue: selection + 1;
    }

    void decreaseSelection()
    {
        selection = selection <= minValue ? decreasingResetValue : selection - 1;
    }

    void modifySelection(Direction direction)
    {
        switch (direction)
        {
        case Increase:
            increaseSelection();
            break;
        case Decrease:
            decreaseSelection();
            break;
        default:
            increaseSelection();
            break;
        }
    }



};

class SelectorBuilder
{
private:
    int initalValue = 0;
    int minValue = 0;
    int maxValue = 0;
    bool isLooping = false;
public:
    SelectorBuilder& setInitalValue(int initalValue)
    {
        this->initalValue = initalValue;
        return *this;
    }
    SelectorBuilder& setMinValue(int minValue)
    {
        this->minValue = minValue;
        return *this;
    }
    SelectorBuilder& setMaxValue(int maxValue)
    {
        this->maxValue = maxValue;
        return *this;
    }
    SelectorBuilder& setIsLooping(bool isLooping)
    {
        this->isLooping = isLooping;
        return *this;
    }
    Selector build()
    {
        return Selector(initalValue, minValue, maxValue, isLooping);
    }
};

#endif // SELECTOR_H
