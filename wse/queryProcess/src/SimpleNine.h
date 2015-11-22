//
//  SimpleNine.h
//  QueryProcess
//
//  Created by DongYun on 10/20/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef __QueryProcess__SimpleNine__
#define __QueryProcess__SimpleNine__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class SimpleNine{
public:
    SimpleNine();
    ~SimpleNine();
    
    // compress data in vector
    void compress(vector<unsigned int>&needCompress,vector<unsigned int>& resVector);
    void deCompress(vector<unsigned int>&needDecompress, vector<unsigned int>& result);
    
private:
    // store nine situations <1,28> <2,14> ....
    unordered_map<int,int> codeInfo;
    //return how many bits a integer needs
    int bitsNum(unsigned int number);
};
#endif /* defined(__QueryProcess__SimpleNine__) */
