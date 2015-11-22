//
//  SimpleNine.h
//  BuildIndex
//
//  Created by DongYun on 10/6/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef __BuildIndex__SimpleNine__
#define __BuildIndex__SimpleNine__

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
    
private:
    // store nine situations <1,28> <2,14> ....
    unordered_map<int,int> codeInfo;
    //return how many bits a integer needs
    int bitsNum(unsigned int number);
};

#endif /* defined(__BuildIndex__SimpleNine__) */
