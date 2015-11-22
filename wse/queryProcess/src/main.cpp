//
//  main.cpp
//  QueryProcess
//
//  Created by DongYun on 10/19/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#include <iostream>
#include <vector>
#include "SimpleNine.h"
#include "Global.h"
#include "SearchQuery.h"
#include <time.h>
#include <math.h>
using namespace std;

void printData(string path);
int main(int argc, const char * argv[]) {
    string query;
    string path = "/Users/dongyun/Documents/third semester/web search engine/homework/hw3/nz2 res/toBeMerge4/interFile_index_1_0";
   // string path = "/Users/dongyun/Documents/third semester/web search engine/homework/hw3/nz2 res/s_data/interFile_index_0_1";
    string path2 = "/Users/dongyun/Documents/third semester/web search engine/homework/hw3/nz2 res/toBeMerge4/docInfo_ALL";
    
    //printData(indexFilePath);
    
    loadWordIndex(path);
    loadDocInfo(path2);
    //clock_t start , end;
    cout << "Please input query:"<<endl;
   while (getline(cin, query)){
        if(query.length() >= 80){
            cout << "query is too long"<< endl;
        } else{
            //cout <<query<<endl;
            startTime = clock();
            SearchQuery sq(query);
            sq.findDocs();
        }
        cout << "Please input query:"<<endl;
    }
    
    //sq.test();
    
    
    return 0;
}



void printData(string path){
     ifstream fp;
     string path1 = "/Users/dongyun/Documents/third semester/web search engine/homework/hw3/nz2 res/toBeMerge4/interFile_data_4_0";
     fp.open(path1, ios::binary);
     if(!fp){
         return;
     }
    unsigned int n = 1 <<28;
    //cout << n<<endl;
    unsigned int bytes = 0;
     while(1){
         unsigned int* buffer;
         buffer = new unsigned int [10000];
         fp.read((char*)buffer, sizeof(int)* 10000);
         int have = fp.gcount();
         if(have == 0){
            break;
         }
         bytes += have;
         for(int i = 0; i < have/4; i++){
             if(buffer[i]< 268435456 && have != 0){
                // cout << "have = " << have << endl;
                 cout << "buffer[i] = " << buffer[i]<<endl;
             }
            // cout << buffer[i]<<endl;
         }
         delete[] buffer;
     }
  //  cout << "end"<<endl;
}