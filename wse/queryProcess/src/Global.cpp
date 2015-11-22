//
//  Global.cpp
//  QueryProcess
//
//  Created by DongYun on 10/20/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#include "Global.h"

unordered_map<string, Index> wordIndex;
unordered_map<unsigned int,DocInfo> docInfo;
//string indexFilePath = "/Users/dongyun/Documents/third semester/web search engine/homework/hw3/nz2 res/s_data/interFile_data_4_0";
string indexFilePath = "/Users/dongyun/Documents/third semester/web search engine/homework/hw3/nz2 res/toBeMerge4/interFile_data_1_0";
ifstream indexFile;
unsigned int totalDocNums;
double aveDocLength;
LRUCache cache(1000);
clock_t startTime;
clock_t endTime;
double duration;

void loadWordIndex(string path){
    ifstream input;
    input.open(path);
    if(!input){
        cout << "can not open file" << endl;
        exit(-1);
    }
    
    string line;
    while(getline(input, line)){
        vector<string> lineInfo;
        split(line, lineInfo, "\t");
        if(lineInfo.size() < 7){
            cout << "split error or wordIndex error"<<endl;
            exit(-1);
        }
        vector<ChunkInfo> tmpChunkInfo;
        Index index;
        //cout << lineInfo[1]<<endl;
        index.startPos = stoi(lineInfo[1]);
        index.totalDocNum = stoi(lineInfo[2]);
        index.length = stoi(lineInfo[3]);
        for(int i = 4; i < lineInfo.size(); i++){
            //cout << i << endl;
            /*if(i == 409) {
                cout << lineInfo[i-1] << "mmm"<<lineInfo[i]<<"mmm"<<endl;
                for(int k=0; k<lineInfo[i].size(); k++ ){
                    cout << (int)lineInfo[i][k]<<endl;
                }
                system("pause");
            }*/
            ChunkInfo chunk;
            chunk.length = stoi(lineInfo[i++]);
            chunk.startDocId = stoi(lineInfo[i++]);
            chunk.endDocId = stoi(lineInfo[i]);
            tmpChunkInfo.push_back(chunk);
        }
        index.chunkInfo = tmpChunkInfo;
        wordIndex.insert(pair<string, Index> (lineInfo[0],index));
        
    }
    //cout << wordIndex.size()<<endl;
    cout << "finished loading wordInfo "<< endl;
}


void loadDocInfo(string path){
    ifstream input;
    input.open(path);
    if(!input){
        cout << "error in open file" << endl;
        exit(-1);
    }
    string line;
    totalDocNums = 0;
    unsigned int termNum = 0;
    while(getline(input, line)){
        totalDocNums ++;
        vector<string> res;
        splitString(line, res, " ");
        DocInfo info;
        info.termNum = stoi(res[1]);
        termNum += stoi(res[1]);
        info.url = res[2];
        docInfo[stoi(res[0])] = info;
    }
    aveDocLength = double(termNum)/double (totalDocNums);
    //cout << "aveDocLength = " << aveDocLength<<endl;
    //cout << "totalDocNums = " << totalDocNums<<endl;
    cout << "Load docInfo finished...." << endl;
}

void  split(string& str, vector<string>& res, string c) {
    int start = 0;
    //vector<string>res;
    for(int i=0; i<str.length(); i++) {
        if(str[i]== '\t') {
            res.push_back(str.substr(start,i-start));
            start = i + 1;
        }
    }
    /*for(int i = 0; i < res.size(); i++){
        cout << res[i]<<endl;
    }*/
    //return res;
}
void splitString(string& s, vector<string>& v, string c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        string str = s.substr(pos1, pos2-pos1);
        if(str != "")
            v.push_back(str);
        
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length() && s.substr(pos1) != ""&& s.substr(pos1) != "\n")
        v.push_back(s.substr(pos1));
    
    //cout << s << endl;
    //cout << v.size()<<endl;
    /*for(int i = 0; i < v.size(); i++){
        cout << v[i]<<endl;
    }*/
}

void printIndex(){
    unordered_map<string, Index> ::iterator it = wordIndex.begin();
    
    for(; it!= wordIndex.end(); it++){
        cout << it->first << "\t" << it->second.startPos<<"\t"<<it->second.totalDocNum<<"\t"<<it->second.length<<"\t";
        for(int i = 0 ; i < it->second.chunkInfo.size(); i++){
            cout << it->second.chunkInfo[i].length<<"\t"<<it->second.chunkInfo[i].startDocId<<"\t"<<it->second.chunkInfo[i].endDocId <<endl;
        }
    }
}

