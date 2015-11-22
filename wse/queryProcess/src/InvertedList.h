//
//  InvertedList.h
//  QueryProcess
//
//  Created by DongYun on 10/19/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef __QueryProcess__InvertedList__
#define __QueryProcess__InvertedList__

#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "Global.h"
#include "SimpleNine.h"
using namespace std;


class InvertedList{
public:
    void openList(string term);
    void closeList();
    unsigned int nextGEQ(unsigned int k);
    unsigned int getFreq();
    double getIDF();
private:
    string term;
    unsigned int* buffer;
    unsigned int curBufferPtr;
    unsigned int curChunkPtr;
    //unsigned int curDocId;
    unsigned int freq;
    double idf;
    unordered_map<string, Index>::iterator it;
};
#endif /* defined(__QueryProcess__InvertedList__) */
