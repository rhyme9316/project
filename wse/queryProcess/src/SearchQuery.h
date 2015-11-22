//
//  SearchQuery.h
//  QueryProcess
//
//  Created by DongYun on 10/20/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef __QueryProcess__SearchQuery__
#define __QueryProcess__SearchQuery__

#include <stdio.h>
#include <queue>
#include "Global.h"
#include "InvertedList.h"
#include "SimpleNine.h"

struct DocContainQuery{
    unsigned int docId;
    //this vector stores each terms' freq in this doc
    vector<unsigned int> freqForTerms;
};

struct topKdoc{
    friend bool operator < (topKdoc doc1, topKdoc doc2){
        return doc1.BM25 < doc2.BM25;
    }
    unsigned int docId;
    string url;
    double BM25;
    topKdoc(unsigned int docId, string url, double BM25): docId(docId), url(url), BM25(BM25){}
};
class SearchQuery{
    
public:
    SearchQuery(string query);
    ~SearchQuery();
    void findDocs();
    
private:
    // by the order of query
    vector<InvertedList> termList;
    vector<string> terms;
    string query;
    void openTermsList();
    void closeTermsList();
    void topK(vector<DocContainQuery> & docs);
    double bmScore(DocContainQuery docQueryInfo);

};

#endif /* defined(__QueryProcess__SearchQuery__) */
