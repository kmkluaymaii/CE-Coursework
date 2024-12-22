//
//  MissingNum.cpp
//  MissingNum
//
//  Copyright Â© Tali Moreshet. All rights reserved.
//

#include "InputArray.h"

InputArray::InputArray()
{
    // this->n = 7;
    // this->Array = new int[n];
    // this->Array[0] = 7;
    // this->Array[1] = 5;
    // this->Array[2] = 2;
    // this->Array[3] = 0;
    // this->Array[4] = 1;
    // this->Array[5] = 3;
    // this->Array[6] = 6;
    this->n = 15;
    this->Array = new int[n];
    this->Array[0] = 12;
    this->Array[1] = 5;
    this->Array[2] = 2;
    this->Array[3] = 0;
    this->Array[4] = 1;
    this->Array[5] = 3;
    this->Array[6] = 6;
    this->Array[7] = 9;
    this->Array[8] = 8;
    this->Array[9] = 11;
    this->Array[10] = 15;
    this->Array[11] = 10;
    this->Array[12] = 13;
    this->Array[13] = 14;
    this->Array[14] = 4;
}

InputArray::~InputArray()
{
    delete [] Array;
}

// return true if 'bit' in 'element' of Array is set to 1
bool InputArray::findBit(int element, int bit)
{
    bitset<32> Ai = Array[element];
    return (Ai[bit] == 1);
}