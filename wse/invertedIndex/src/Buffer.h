//
//  Buffer.h
//  BuildIndex
//
//  Created by DongYun on 10/07/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef BuildIndex_Buffer_h
#define BuildIndex_Buffer_h

#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>

using namespace std;


// This index buffer provides a readWord() function that each time it receive a length and returns
// corresponding length's wordInfo. It is realized by using a fixed sized buffer, and each time return
// wordInfo from buffer. if buffer is empty, reload fixed size to buffer.
class inputIndexBuffer {
private:
    ifstream input;
    string buffer;
    int ptr;
public:
    void close() {
        input.clear();
        input.close();
        buffer.clear();
    }
    bool initial(string path) {
        input.open(path);
        ptr=0;
        if(!input)
            return 0;
        return 1;
    }
    bool load() {
        if(input.eof())
            return 0;
        char *charBuffer;
        charBuffer=(char*)malloc(sizeof(char)*MEMORY_LIMIT);
        //buffer = buffer.substr(ptr);
        //ptr = 0;
        input.read(charBuffer, MEMORY_LIMIT);
        int bufferBytes = buffer.size()+input.gcount();
        buffer += charBuffer;
        buffer = buffer.substr(0, bufferBytes);
        free(charBuffer);
        return 1;
    }
    bool empty() {
        if(ptr >= buffer.length() && input.eof())
            return 1;
        return 0;
    }
    void loadIndex() {
        while(!input.eof())
            load();
        ptr=0;
        cout<<"load complete"<<endl;
    }
    bool readWord(string& word,int& len, int& totalDoc, string& posting) {
        int firstGap=ptr, secondGap=ptr,thirdGap=ptr, fourthGap=ptr;
        int count =0;
        int start = ptr;
        for(int i=ptr;i < buffer.size();i++) {
            if(buffer[i] == '\t' && count == 0){
                count++;
                firstGap=i;
            }
            else if(buffer[i] == '\t' && count == 1){
                count++;
                secondGap=i;
            }
            else if(buffer[i] == '\t' && count == 2){
                count++;
                thirdGap=i;
            }
            else if(buffer[i] == '\t' && count == 3){
                count++;
                fourthGap=i;
            }
            else if(buffer[i] == '\n'){
                ptr = i+1;
                word = buffer.substr(start, firstGap-start);
                totalDoc = stoi(buffer.substr(secondGap+1,thirdGap-secondGap-1));
                len = stoi(buffer.substr(thirdGap+1,fourthGap-thirdGap-1));
                posting = buffer.substr(fourthGap+1, i-fourthGap-1);
                return 1;
            }
        }
        return 0;
    }
    
};


// This data buffer provides a read() function that each time it receive a length and returns
// corresponding length's data. It is realized by using a fixed sized buffer, and each time return
// data from buffer. if buffer is empty, reload fixed size to buffer.
class InputDataBuffer {
private:
    ifstream input;
    unsigned int* buffer;
    int bPtr, ePtr;
    
public:
    void close() {
        input.clear();
        input.close();
        delete []buffer;
    }
    InputDataBuffer(){
        bPtr = 0;
        ePtr = 0;
        buffer = NULL;
    }
    bool initial(string path) {
        buffer = NULL;
        input.open(path,ios::binary);
        if(!input)
            return 0;
        return 1;
    }
    bool load() {
        if(input.eof())
            return 0;
        buffer = new unsigned int[MEMORY_LIMIT/4];
        input.read((char*)buffer, sizeof(int)*(MEMORY_LIMIT/4));
        int bufferBytes = input.gcount();
        ePtr = bufferBytes/4 ;
        /*for(int i = 0; i < 100; i ++){
         cout <<buffer[i]<<endl;
         }*/
        
        return 1;
    }
    bool empty() {
        if(ePtr == bPtr && input.eof())
            return 1;
        return 0;
    }
    void read (int length, unsigned int* tmpBuffer) {
        //int tmpBuffer[length];
        int count = 0;
        if(ePtr == bPtr && !input.eof()){
            ePtr = 0;
            bPtr = 0;
            if(buffer){
                delete []buffer;
                buffer = NULL;
            }
            load();
        }
        if(ePtr != bPtr){
            while(bPtr < ePtr && count < length ){
                tmpBuffer[count] = buffer[bPtr];
                count ++;
                bPtr ++;
            }
            
            if(count < length){
                int tmpLength = length - count;
                unsigned int *rest = new unsigned int[tmpLength];
                read(tmpLength,rest);
                //cout << tmpLength<<endl;
                for(int i = 0; i <tmpLength; i++){
                    // cout<<"rest[i] = "<<i << "  "<<rest[i]<<endl;
                    tmpBuffer[count++] = rest[i];
                }
                delete []rest;
            }
        }
        return ;
    }
};
#endif
