#include "TempClass.hpp"

#include <iostream>

int TempClass::getData()
{
    return memberData_;
}

void TempClass::setData(int newData)
{
    memberData_ = newData;
}
