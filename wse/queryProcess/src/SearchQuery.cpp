//
//  SearchQuery.cpp
//  QueryProcess
//
//  Created by DongYun on 10/20/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#include "SearchQuery.h"


SearchQuery::SearchQuery(string query){
    vector<string> tmpTerms;
    splitString(query, tmpTerms, " ");
    for(int i = 0; i < tmpTerms.size(); i++){
        unordered_map<string, Index>::iterator it;
        it = wordIndex.find(tmpTerms[i]);
        if(it != wordIndex.end()){
            terms.push_back(tmpTerms[i]);
            //cout << "tmpTerms[i]= " << tmpTerms[i]<<endl;
        } else{
            cout << "Does not exist term "<<tmpTerms[i]<<endl;
        }
    }
    if(terms.size() == 0){
        cout << "Invalid query, please try again!"<<endl;
        return;
    }
    indexFile.open(indexFilePath, ios::binary);
    if(!indexFile){
        cout << "can not open inverted list file"<<endl;
        exit(-1);
    }
}

SearchQuery::~SearchQuery(){
    indexFile.close();
}


void SearchQuery::openTermsList(){
    for(int i = 0; i < terms.size(); i++){
        InvertedList list;
        list.openList(terms[i]);
        termList.push_back(list);
    }
}


void SearchQuery::closeTermsList(){
    for(int i = 0; i < termList.size(); i++){
        termList[i].closeList();
    }
}


void SearchQuery::findDocs(){
    openTermsList();
    vector<DocContainQuery> result;
    
    if(termList.size() == 0){
        cout << "Invalid query, please try another query!" <<endl;
        return;
    }
    bool flag = true;
    unsigned int curDocId = 0;
    while(flag){
        for(int i = 0; i < termList.size(); i++){
            unsigned int tmp = termList[i].nextGEQ(curDocId);
            if(tmp < curDocId){
                flag = false;
                break;
            } else if(tmp > curDocId){
                    curDocId = tmp;
                    i = -1;
            } else{
                    if(i == termList.size()-1){
                        DocContainQuery doc;
                        doc.docId = curDocId ++;
                        //cout << "add new doc = " << doc.docId << endl;
                        for(int j = 0; j < termList.size(); j++){
                            doc.freqForTerms.push_back(termList[j].getFreq());
                        }
                        result.push_back(doc);
                    }
            }
        }
    }
    if(result.size() == 0){
        cout << "sorry, no result.... >.<" << endl;
        return;
    } else{
        topK(result);
    }
    closeTermsList();
    return;
}

void SearchQuery::topK(vector<DocContainQuery>& docs){
    priority_queue<topKdoc> q;
    
    for(int i = 0; i < docs.size(); i++){
        double bm25 = bmScore(docs[i]);
        topKdoc topDoc(docs[i].docId,docInfo[docs[i].docId].url, bm25);
        q.push(topDoc);
    }
    //cout << "We got "<<docs.size()<<" docs"<<endl;
    int counter = 10 > docs.size()? docs.size() : 10;
    endTime = clock();
    duration = double(endTime-startTime)/CLOCKS_PER_SEC;
    cout << "Query processing time is: " << duration << " seconds!"<<endl;
    for(int i = 0; i < counter; i++){
        topKdoc tmpDoc = q.top();
        q.pop();
        cout << tmpDoc.docId << "\t" << tmpDoc.BM25 << "\t" << tmpDoc.url<<endl;
    }
    return;
}



double SearchQuery::bmScore(DocContainQuery docQuerInfo){
    double bm25 = 0;
    double k = 1.2;
    double b = 0.75;
    
    for(int i = 0; i < docQuerInfo.freqForTerms.size(); i++){
        double termFreq = docQuerInfo.freqForTerms[i];
        double termIdf = termList[i].getIDF();
        double docTermNum = (double)docInfo[docQuerInfo.docId].termNum;
        bm25 +=  termIdf * termFreq * (k+1)/ (termFreq + k * (1-b + b * (docTermNum / aveDocLength)));
        //cout << "bm25 is " << bm25 << endl;
    }
    
    return bm25;
}





