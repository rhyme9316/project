//
//  InvertedList.cpp
//  QueryProcess
//
//  Created by DongYun on 10/19/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#include "InvertedList.h"


void InvertedList::openList(string term){
    this->term = term;
    it = wordIndex.find(term);
    //cout << it->second.length<< endl;
    buffer = cache.get(term);
    if(buffer == NULL){
        buffer = new unsigned int[it->second.length];
        indexFile.seekg(it->second.startPos*4);
        //indexFile.seekg(0);
        //cout<< it->second.startPos << endl;
        indexFile.read((char*)buffer, sizeof(unsigned int)* it->second.length);
       // for(int i = 0; i < it->second.length; i++){
         //   cout << buffer[i]<<endl;
        //}
    }
    curBufferPtr = 0;
    curChunkPtr = 0;
    freq = 0;
    double tmp = (totalDocNums - it->second.totalDocNum + 0.5)/(it->second.totalDocNum + 0.5);
    idf = log(tmp);
}


void InvertedList::closeList(){
    cache.set(term, buffer);
    //delete[] buffer;
}


unsigned int InvertedList::nextGEQ(unsigned int k){
    unsigned int curLen = 0;
    unsigned int curDocId = 0;
    //cout << "need to find k = "<<k<<endl;
    //cout <<"how many chunks it has = " <<it->second.chunkInfo.size() << endl;
    for(int i = curChunkPtr; i < it->second.chunkInfo.size(); i++){
        if(it->second.chunkInfo[i].endDocId < k){
            curLen += it->second.chunkInfo[i].length;
            curDocId = it->second.chunkInfo[i].endDocId;
        } else{
            vector<unsigned int> needDecompress;
            unsigned int bufferStart = curLen + curBufferPtr;
            curBufferPtr = bufferStart;
            //cout << "buffer start = "<<bufferStart << endl;
            curChunkPtr = i;
            //cout << "chunk " << i << "'s length = " << it->second.chunkInfo[i].length<<endl;
            for(int j = bufferStart; j < it->second.chunkInfo[i].length + bufferStart; j++){
                needDecompress.push_back(buffer[j]);
            }
            vector<unsigned int> result;
            SimpleNine sn;
            sn.deCompress(needDecompress, result);
            //cout << "chunk length = " << result.size()<<endl;
            
            for(int j = 0; j < result.size(); j++){
                if(j == 0) {
                    curDocId = it->second.chunkInfo[i].startDocId;
                } else{
                    curDocId += result[j];
                }
                //cout <<"curDocId = "<<curDocId<<endl;
                if(curDocId == k){
                    freq = result[j+1];
                    //cout << "find docId = " << k<<endl;
                    return k;
                } else if(curDocId < k){
                    j += result[j+1] + 1;
                    if(j >= result.size()){
                        //cout << "error" << endl;
                        exit(-1);
                    }
                } else if(curDocId > k){
                    freq = result[j+1];
                    //cout << "find docId = " << curDocId<<endl;
                    return curDocId;
                }
            }
        }
    }
    return 0;
}

unsigned int InvertedList::getFreq(){
    return freq;
}


double InvertedList::getIDF(){
    return idf;
}
