//
//  Global.cpp
//  BuildIndex
//
//  Created by DongYun on 10/5/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#include "Global.h"

vector<DocInfo> docInfo;
unsigned int pathNum = 0;
unsigned int outputBuffer[BUFFER_SIZE];
long long int bytes = 0;
//unsigned int docIdCounter = 0;

//change here if want to change the output path
string basePath = "/Users/dongyun/Documents/third semester/web search engine/homework/hw2/code/BuildIndex/toBeMerge293/";
string inputPath = "/Users/dongyun/Documents/third semester/web search engine/homework/hw2/code/";

void printDocInfo(){
    ofstream outDocInfo;
    outDocInfo.open(basePath + "docInfo");
    for(int i = 0; i < docInfo.size(); i ++){
        outDocInfo <<docInfo[i].docId<<" "<<docInfo[i].size<<" "<<docInfo[i].url<<endl;
    }
    outDocInfo.close();
}


string int2str(unsigned int integer){
    string res;
    stringstream stream;
    stream<<integer;
    res=stream.str();
    return res;
}