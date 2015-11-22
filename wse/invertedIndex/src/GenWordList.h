//
//  GenWordList.h
//  BuildIndex
//
//  Created by DongYun on 10/5/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef __BuildIndex__GenWordList__
#define __BuildIndex__GenWordList__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include "Parser.h"
#include "Global.h"
#include "SimpleNine.h"
using namespace std;

struct Post{
    unsigned int docId;
    unsigned int freq;
    vector<unsigned int> pos;
    unsigned int previousPos;
    unsigned int previousDocId;
    Post(unsigned long docId, unsigned int freq, unsigned int previousPos, unsigned int previousDocId):docId(docId),freq(freq),previousPos(previousPos), previousDocId(previousDocId){};
};




class GenWordList{
public:
    // this function receive an array of pages and sent them one by one  to parser,
    // and sort all postings by words.
    void sortAllWords(vector<Page>& pages);

private:
    map<string, list<Post>> word_by_alphabet;
    
    // this function calls compress to compress sorted inverted list by chunk,
    // and then print compressed result to binary files.
    // Remeber I use outputBuffer for compressed data. which decrease the times to access disk
    void printToFile();
    void printText();
    // this function stores the docID, url, # of terms for each page
    void storeDocInfo(unsigned int docId,string url,unsigned int size);
    
};

#endif /* defined(__BuildIndex__GenWordList__) */
