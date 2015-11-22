//
//  Global.h
//  BuildIndex
//
//  Created by DongYun on 10/5/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef BuildIndex_Global_h
#define BuildIndex_Global_h

#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>

#define BUFFER_SIZE 50000000 // 50MB used for output binary files
#define CHUNK_SIZE 50  //Unit: document
#define MEMORY_LIMIT 100000000 // 100MB used in merge
#define FILE_NUMBER_LIMIT 10 // used for process input
#define GZ_INDEX_BUF_SIZE 1000 // used when load gz-compressed index
#define GZ_DATA_BUF_SIZE 100000 // used when load gz-compressed data

using namespace std;

struct Page{
    string url;
    unsigned int docId;
    string page;
    unsigned int len;
    Page(string Url, unsigned int Id, string page_, unsigned int Len): url(Url), docId(Id), page(page_), len(Len){}
    
};


// stores docId, url and # of terms
struct DocInfo{
    unsigned int docId;
    string url;
    unsigned int size;
    
    DocInfo(unsigned int docId, string url, unsigned int size):docId(docId),url(url),size(size){};
};

extern vector<DocInfo> docInfo;

//inverted list path
extern string basePath;
//number of output intermediate files
extern unsigned int pathNum;

//input data path
extern string inputPath; 

//print docInfo
extern void printDocInfo();

//change int to string
extern string int2str(unsigned int integer);

//output buffer
extern unsigned int outputBuffer[BUFFER_SIZE];

extern unsigned int docIdCounter;

extern long long int bytes;
#endif
