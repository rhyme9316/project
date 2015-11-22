//
//  GenWordList.cpp
//  BuildIndex
//
//  Created by DongYun on 10/5/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#include "GenWordList.h"



void GenWordList::sortAllWords(vector<Page>& pages){
    cout<<"pages.size = "<< pages.size()<<endl;
    
    for(int i = 0; i < pages.size(); i++){
        Parser *ps = new Parser(pages[i].page.length());
        //buffer = NULL;
        //cout << i << endl;
        //cout <<pages[i].page<<endl;
        //buffer = (char*)malloc(sizeof(char)*pages[i].page.length());
        char buffer[pages[i].page.length()];
        
        strcpy(buffer,pages[i].page.c_str());
        unsigned int res = ps->parser(buffer, pages[i].len);

        //  cout<<"i="<<i<<"	page_docId:  "<<pages[i].docId << "   res= "<<res<<endl;
        
        if (res > 0){
            string resBuffer = ps->getBuf();
            vector<int> pos = ps->getPos();
            
            //cout<<"buffer length = "<<buffer.length()<<endl;
            //cout<<"pos size = "<< pos.size()<<endl;
            
            storeDocInfo(pages[i].docId, pages[i].url,pos.size());
            string word = "";
            int k = 0;
            for(int j = 0; j < resBuffer.length(); j++){
                //cout<<"k="<<k<<endl;
                if(resBuffer[j] == ' '){
                    if(word == "") continue;
                    map<string,list<Post>>::iterator it = word_by_alphabet.find(word);
                    if(it == word_by_alphabet.end() || it->second.back().previousDocId != pages[i].docId){
                        //docId for new post, freqency, previous pos,previous docId;
                        Post post(pages[i].docId,1,pos[k],pages[i].docId);
                        post.pos.push_back(pos[k++]);
                        if(it == word_by_alphabet.end()){
                            list<Post> tmpList;
                            tmpList.push_back(post);
                            
                            word_by_alphabet.insert(pair<string,list<Post>>(word, tmpList));
                        } else{
                            post.docId -= it->second.back().previousDocId;
                            it->second.back().previousDocId = pages[i].docId;
                            it->second.push_back(post);
                        }
                    } else{
                        it->second.back().freq ++;
                        it->second.back().pos.push_back(pos[k] - it->second.back().previousPos);
                        it->second.back().previousPos = pos[k++];
                    }
                    //cout<<word<<endl;
                    word = "";
                } else if(resBuffer[j] == '\n'){
                    word = "";
                } else{
                    word += resBuffer[j];
                }
            }
            
        }
        //ps->resetBuf();
        //ps->resetPos();
        delete ps;
    }
    cout<<"here   "<<word_by_alphabet.size()<<endl;
    printToFile();
    //delete ps;
    return;
}

void GenWordList::printToFile(){
   // printText();
    map<string,list<Post>>::iterator it;
    
    ofstream outfileInvertedList;
    ofstream outfileListIndex;
    
    string tmpPathNum = int2str(pathNum++);
    unsigned int beginPosInFile = 0;
    string indexBuffer = "";
    unsigned int bufferPtr = -1;
    //compressed binary
    outfileInvertedList.open(basePath + "interFile_data_0_" + tmpPathNum, ios::binary);
    
    //format: word, total length in inverted list(unit :4bytes), <length of chunk, start docId, endDocId>
    outfileListIndex.open(basePath + "interFile_index_0_" + tmpPathNum);
    

    if(!outfileInvertedList || !outfileListIndex){
        cout<<"error in open file"<<endl;
        return;
    }
    
    for(it = word_by_alphabet.begin(); it!=word_by_alphabet.end(); it++){
        // generate wordInfo output
        indexBuffer += it->first + "\t" + int2str(beginPosInFile) + "\t" + int2str(it->second.size())+"\t";
        vector<unsigned int> resVector;
        
        unsigned int chunkNum = 0;
        
        unsigned int startDocId = 0;
        unsigned int currentDocId = 0;
        
        list<Post>::iterator listIt = it->second.begin();
        
        while((chunkNum * CHUNK_SIZE) < it->second.size()){
            vector<unsigned int> needCompressed;
            unsigned int currentChunkSize = 0;
            
            startDocId = currentDocId;
            //for each word, compress by chunk (docs numbers)
            while(currentChunkSize < CHUNK_SIZE && listIt != it->second.end()){
                if(startDocId == currentDocId && needCompressed.size() == 0){
                    startDocId += listIt->docId;
                }
                currentDocId += listIt->docId;
                needCompressed.push_back(listIt->docId);
                needCompressed.push_back(listIt->freq);
                
                
                for(int i = 0; i < listIt->pos.size(); i++){
                    needCompressed.push_back(listIt->pos[i]);
                }
                currentChunkSize ++;
                listIt++;
            }
            
            if(currentChunkSize == CHUNK_SIZE || listIt == it->second.end()){
                SimpleNine sn;
                sn.compress(needCompressed,resVector);
                resVector.push_back(-1);
                resVector.push_back(startDocId);
                resVector.push_back(currentDocId);
                chunkNum ++;
                
            }

        }
        beginPosInFile += resVector.size() - 3* chunkNum;
        indexBuffer += int2str((resVector.size() - 3* chunkNum))+"\t";
       
        unsigned int length = 0;
        for(int i = 0; i < resVector.size(); i ++){
            //cout<<resVector[i]<<endl;
            if(resVector[i] != -1){
                if(bufferPtr != BUFFER_SIZE){
                    outputBuffer[++bufferPtr] = resVector[i];
                    //cout<<outputBuffer[bufferPtr]<<endl;
                    if(outputBuffer[bufferPtr] == 170930480){
                        cout << "error"<<endl;
                        exit(-1);
                    }

                    
                } else{
                    outfileInvertedList.write((char*)outputBuffer, sizeof(int) * BUFFER_SIZE);
                    outfileListIndex << indexBuffer;
                    indexBuffer = "";
                    bufferPtr = -1;
                    i--;
                }
                length ++;
            } else if(resVector[i] == -1){
                indexBuffer += int2str(length) + "\t" + int2str(resVector[i+1])+"\t"+int2str(resVector[i+2])+"\t";
                i += 2;
                length = 0;
            }
        }
        indexBuffer += "\n";
    }
    
    if(bufferPtr != -1){
        outfileInvertedList.write((char*)outputBuffer, sizeof(int) * (bufferPtr+1));
        outfileListIndex << indexBuffer;
        bufferPtr = -1;
        indexBuffer = "";
    }

    outfileInvertedList.close();
    outfileListIndex.close();
}

void GenWordList::storeDocInfo(unsigned int docId,string url, unsigned int size){
    //cout << url <<endl;
    DocInfo info(docId,url,size);
    docInfo.push_back(info);
}

//map<string, list<Post>> word_by_alphabet;
void GenWordList::printText(){
    ofstream text;
    text.open(basePath + "text");
    if(!text){
        cout << "error in open text"<<endl;
        exit(-1);
    }
    map<string,list<Post>>::iterator it;
    for(it = word_by_alphabet.begin(); it!=word_by_alphabet.end(); it++){
        text << it->first <<"\t";
        list <Post> ::iterator lit;
        for(lit = it->second.begin(); lit!= it->second.end(); lit++){
            text << lit->docId << "\t"<<lit->freq<<"\t";
            for(int i = 0; i < lit->pos.size(); i++){
                text << lit->pos[i]<<"\t";
            }
        }
        text << "\n";
    }
}

