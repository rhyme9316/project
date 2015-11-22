//
//  Global.h
//  QueryProcess
//  Created by DongYun on 10/20/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef __QueryProcess__Global__
#define __QueryProcess__Global__

#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <time.h>
#include "LRU.h"

using namespace std;

struct ChunkInfo{
    unsigned int length;
    unsigned int startDocId;
    unsigned int endDocId;
};

struct Index{
    unsigned int startPos;
    unsigned int length;
    unsigned int totalDocNum;
    vector<ChunkInfo> chunkInfo;
};



struct DocInfo{
    string url;
    int termNum;
    //DocInfo(string url, int termNum): url(url), termNum (termNum){}
};

extern unordered_map<string, Index> wordIndex;

extern unordered_map<unsigned int,DocInfo> docInfo;

extern void loadWordIndex(string path);
extern void loadDocInfo(string path);
extern void splitString(string& s, vector<string>& v, string c);
extern void printIndex();
extern void split(string& s, vector<string>& v, string c);
extern string indexFilePath;
extern ifstream indexFile;
extern unsigned int totalDocNums;
extern double aveDocLength;
extern LRUCache cache;
extern clock_t startTime;
extern clock_t endTime;
extern double duration;
#endif /* defined(__QueryProcess__Global__) */
