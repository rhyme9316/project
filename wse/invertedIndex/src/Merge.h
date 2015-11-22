//
//  Merge.h
//  BuildIndex
//
//  Created by DongYun on 10/07/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef BuildIndex_Merge_h
#define BuildIndex_Merge_h
#include "Buffer.h"


class Merge{
private:
    // two input wordInfo path and two input data path
    InputDataBuffer  data1;
    InputDataBuffer  data2;
    inputIndexBuffer index1;
    inputIndexBuffer index2;
    
    // two merge output
    ofstream outputData;
    ofstream outputIndex;
    
public:
    void close() {
        data1.close();
        data2.close();
        index1.close();
        index2.close();
        outputData.close();
        outputIndex.close();
    }
    
    // initial input path
    int initial(string dataPath1, string dataPath2,string indexPath1, string indexPath2) {
        bool d1=data1.initial(dataPath1);
        bool d2=data2.initial(dataPath2);
        bool i1=index1.initial(indexPath1);
        bool i2=index2.initial(indexPath2);
        //outputData.open(outputDataPath,ios::binary);
        //outputIndex.open(outputIndexPath2);
        return d1+d2+i1+i2;
    }
    
    void setOutput(string outputDataPath, string outputIndexPath) {
        outputData.open(outputDataPath,ios::binary);
        outputIndex.open(outputIndexPath);
    }
    
    // load index into main memory
    void readIndex() {
        index1.loadIndex();
        index2.loadIndex();
        cout<<"Load Index Complete"<<endl;
    }
    
    // merging process-- compare two word's value and if two words are equal, combine them,
    // otherwise, output the smaller one.
    void startMerging() {
        int writePtr=0;
        string word1="", word2="";
        int len1, len2;
        int totalDoc1, totalDoc2;
        //int startIndex1, startIndex2;
        string posting1, posting2;
        while(!index1.empty() && !index2.empty()) {
            if(word1 == "") {
                if(index1.readWord(word1,len1,totalDoc1,posting1) == 0)
                    cout <<"Read Word Error"<<endl;
            }
            if(word2 == "") {
                if( index2.readWord(word2,len2,totalDoc2,posting2) ==0 )
                    cout<<"Read Word Error"<<endl;
            }
            if(word1 < word2 ){
                string newIndex = word1+"\t"+int2str(writePtr)+"\t"+int2str(totalDoc1)+"\t"+int2str(len1)+"\t"+posting1;
                outputIndex<<newIndex<<endl;
                writePtr += len1;
                
                unsigned int *dataBuffer = new unsigned int[len1];
                data1.read(len1, dataBuffer);
                outputData.write((char*)dataBuffer, len1*sizeof(int));
                delete []dataBuffer;
                word1 = "";
            }
            else if(word1 > word2 ){
                string newIndex = word2+"\t"+int2str(writePtr)+"\t"+int2str(totalDoc2)+"\t"+int2str(len2)+"\t"+posting2;
                outputIndex<<newIndex<<endl;
                writePtr += len2;
                
                unsigned int *dataBuffer = new unsigned int[len2];
                data2.read(len2, dataBuffer);
                outputData.write((char*)dataBuffer, len2*sizeof(int));
                delete []dataBuffer;
                word2 = "";
            } else if (word1 == word2 ) {
                string newIndex = word1+"\t"+int2str(writePtr)+"\t"+int2str(totalDoc1+totalDoc2)+"\t"+int2str(len1+len2)+"\t"+posting1+posting2;
                outputIndex<<newIndex<<endl;
                writePtr = writePtr+len1+len2;
                
                
                unsigned int *dataBuffer = new unsigned int[len1];
                data1.read(len1, dataBuffer);
                outputData.write((char*)dataBuffer, len1*sizeof(int));
                delete []dataBuffer;
                
                unsigned int *dataBuffer2 = new unsigned int[len2];
                data2.read(len2, dataBuffer2);
                outputData.write((char*)dataBuffer2, len2*sizeof(int));
                delete []dataBuffer2;
                
                word1 = "";word2 = "";
            }
        }
        if(word1 != "") {
            string newIndex = word1+"\t"+int2str(writePtr)+"\t"+int2str(totalDoc1)+"\t"+int2str(len1)+"\t"+posting1;
            outputIndex<<newIndex<<endl;
            writePtr += len1;
            
            unsigned int *dataBuffer = new unsigned int[len1];
            data1.read(len1, dataBuffer);
            outputData.write((char*)dataBuffer, len1*sizeof(int));
            delete []dataBuffer;
            word1 = "";
        }
        if(word2 != "") {
            string newIndex = word2+"\t"+int2str(writePtr)+"\t"+int2str(totalDoc2)+"\t"+int2str(len2)+"\t"+posting2;
            outputIndex<<newIndex<<endl;
            writePtr += len2;
            
            unsigned int *dataBuffer = new unsigned int[len2];
            data2.read(len2, dataBuffer);
            outputData.write((char*)dataBuffer, len2*sizeof(int));
            delete []dataBuffer;
            word2="";
        }
        
        while(index1.readWord(word1,len1,totalDoc1,posting1)) {
            string newIndex = word1+"\t"+int2str(writePtr)+"\t"+int2str(totalDoc1)+"\t"+int2str(len1)+"\t"+posting1;
            outputIndex<<newIndex<<endl;
            writePtr += len1;
            
            unsigned int *dataBuffer = new unsigned int[len1];
            data1.read(len1, dataBuffer);
            outputData.write((char*)dataBuffer, len1*sizeof(int));	
            delete []dataBuffer;
        }
        
        while(index2.readWord(word2,len2,totalDoc2,posting2)) {
            string newIndex = word2+"\t"+int2str(writePtr)+"\t"+int2str(totalDoc2)+"\t"+int2str(len2)+"\t"+posting2;
            outputIndex<<newIndex<<endl;
            writePtr += len2;
            
            unsigned int *dataBuffer = new unsigned int[len2];
            data2.read(len2, dataBuffer);
            outputData.write((char*)dataBuffer, len2*sizeof(int));
            delete []dataBuffer;
        }
        cout<<"Merge Complete!"<<endl;
    }
};

#endif
