#include "selector.h"

Selector::Selector(int initialValue, int minValue, int maxValue)
{
    selection = initialValue;
    this->minValue = minValue;
    this->maxValue = maxValue;
}

int Selector::getSelection()
{
    return selection;
}

void Selector::setSelection(int value)
{
    selection = value;
}

void Selector::setMaxValue(int value)
{
    maxValue = value;
}

void Selector::setMinValue(int value)
{
    minValue = value;
}

void Selector::increaseSelection()
{
    selection = selection >= maxValue ? maxValue: selection + 1;
}

void Selector::increaseSelection(bool loop)
{
    int resetValue = loop ? minValue : maxValue;
    selection = selection >= maxValue ? resetValue : selection + 1;
}

void Selector::modifySelection(bool increasing, bool loop)
{
    if (increasing)increaseSelection(loop);
    else decreaseSelection(loop);
//    int resetValue = loop ? minValue : maxValue;
//    selection = selection >= maxValue ? resetValue : selection + amount;
}

void Selector::decreaseSelection()
{
    selection = selection <= minValue ? minValue : selection - 1;
}

void Selector::decreaseSelection(bool loop)
{
    int resetValue = loop ? maxValue : minValue;
    selection = selection <= minValue ? resetValue : selection - 1;
}
