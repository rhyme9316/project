//
//  SimpleNine.cpp
//  BuildIndex
//
//  Created by DongYun on 10/6/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#include "SimpleNine.h"

SimpleNine::SimpleNine(){
    codeInfo.insert(pair<int, int>(1,28));
    codeInfo.insert(pair<int, int>(2,14));
    codeInfo.insert(pair<int, int>(3,9));
    codeInfo.insert(pair<int, int>(4,7));
    codeInfo.insert(pair<int, int>(5,5));
    codeInfo.insert(pair<int, int>(7,4));
    codeInfo.insert(pair<int, int>(9,3));
    codeInfo.insert(pair<int, int>(14,2));
    codeInfo.insert(pair<int, int>(28,1));
}

SimpleNine::~SimpleNine(){
    
}

/*void SimpleNine::compress(vector<unsigned int>& needCompress, vector<unsigned int>& resVector){
    int i = 0;
    
    while(i< needCompress.size()){
        
        int previousMaxBits = 0;
        int maxBits = 0;
        int count = 0;
        
        for(int j = i; j < needCompress.size(); j ++){
            if(maxBits * count > 28){
                count --;
                break;
            } else{
                int num = bitsNum(needCompress[j]);
                count ++;
                
                if(num > maxBits){
                    previousMaxBits = maxBits;
                    maxBits = num;
                    //cout << "previousMaxBits = " << previousMaxBits << "  maxBits = "<<maxBits<<endl;
                }
            }
        }
        if(previousMaxBits == 0) previousMaxBits = maxBits;
        //cout << "count = "<<count <<"   previousmaxBits = "<<previousMaxBits<<endl;
        int bitsAssigned = 0;
        int compressedCount = 0;
   
        unordered_map<int,int>::iterator it;
        while(true){
            it = codeInfo.find(count);
            if(it == codeInfo.end() || it->second < previousMaxBits){
                count --;
            } else{
                compressedCount = count;
                bitsAssigned = codeInfo[compressedCount];
                break;
            }
        }
        
        //cout <<"compressed from " << i <<" to "<< i+ compressedCount-1<<endl;
        //cout << "bitsAssigned = " <<bitsAssigned << "   compressedCount = " << compressedCount << endl;
       
        
        unsigned int res = 0;
        res <<= 4;
        res += compressedCount;
        for(int k = 0; k < compressedCount ; k ++){
            res <<= bitsAssigned;
            res += needCompress[i+k];
        }

        res <<= (28-compressedCount * bitsAssigned);
       // cout <<"final Result = " << res << endl<<endl;
        i += compressedCount;
        if(res == 170930480 || res == 1713387568){
            for(int l = 0; l < needCompress.size(); l ++){
                cout << needCompress[l]<<endl;
            }
            exit(-1);
        }
        resVector.push_back(res);
    }
    
}
*/
void SimpleNine::compress(vector<unsigned int>& needCompress, vector<unsigned int>& resVector){
    int i = 0;
    
    while(i< needCompress.size()){
        
        int previousMaxBits = 0;
        int maxBits = 0;
        int count = 0;
        
        for(int j = i; j < needCompress.size(); j ++){
            int num = bitsNum(needCompress[j]);
            count ++;
            if(num > maxBits){
                maxBits = num;
            }
            if(maxBits * count > 28){
                count --;
                break;
            }
            
        }
        //if(previousMaxBits == 0) previousMaxBits = maxBits;
        //cout << "count = "<<count <<"   previousmaxBits = "<<previousMaxBits<<endl;
        int bitsAssigned = 0;
        int compressedCount = 0;
        
        unordered_map<int,int>::iterator it;
        while(true){
            it = codeInfo.find(count);
            if(it == codeInfo.end() || it->second < maxBits){
                count --;
            } else{
                compressedCount = count;
                bitsAssigned = codeInfo[compressedCount];
                break;
            }
        }
        
        //cout <<"compressed from " << i <<" to "<< i+ compressedCount-1<<endl;
        //cout << "bitsAssigned = " <<bitsAssigned << "   compressedCount = " << compressedCount << endl;
        if(count == 0) {
            cout << "count == 0" << "error"<<endl;
        }
        
        unsigned int res = 0;
        res <<= 4;
        res += compressedCount;
        for(int k = 0; k < compressedCount ; k ++){
            res <<= bitsAssigned;
            res += needCompress[i+k];
        }
        
        res <<= (28-compressedCount * bitsAssigned);
        // cout <<"final Result = " << res << endl<<endl;
        i += compressedCount;
        resVector.push_back(res);
    }
    
}



int SimpleNine::bitsNum(unsigned int number){
    if(number == 0){
        //cout <<"number = "<<number <<"  bitsNum = 0"<<endl;
        return 0;
    }
    //unsigned int ori = number;
    for(int i = 1 ; i <= 32; i++){
        number >>= 1;
        if(number == 0){
            //cout <<"number = "<<ori <<"  bitsNum = "<<i<<endl;
            return i;
        }
    }
    return -1;
}
