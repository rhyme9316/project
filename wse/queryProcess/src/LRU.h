//
//  LRU.h
//  QueryProcess
//
//  Created by DongYun on 10/23/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//

#ifndef QueryProcess_LRU_h
#define QueryProcess_LRU_h
#include <iostream>
#include <unordered_map>
using namespace std;

struct Node {
    //string term;
    unsigned int* buffer;
    Node * pre;
    Node * next;
    Node():buffer(NULL),pre(NULL), next(NULL){}
    Node(unsigned int* b , Node * p = NULL, Node *n = NULL):buffer(b),pre(p),next(n){}
};

class LRUCache{
private:
    int cap;
    unordered_map<string, Node *> cache;
    Node * head;
    Node * tail;
    
public:
    LRUCache(int capacity){
        cap = capacity;
        head = tail = NULL;
    }
    
    void updateNode(Node * node){
        if( node == tail )
            return;
        if( node == head ){
            head = node->next;
        }else{
            node->pre->next = node->next;
            node->next->pre = node->pre;
        }
        tail->next = node;
        node->pre = tail;
        node->next = NULL;
        tail = node;
    }
    
    unsigned int* get(string key){
        if( cache.find(key) != cache.end() ){
            Node * targetNode = cache[key];
            updateNode(targetNode);
            return targetNode->buffer;
        }else
            return NULL;
    }
    
    int set(string key, unsigned int* value){
        if( cache.find(key) != cache.end() ){
            Node * targetNode = cache[key];
            //targetNode->buffer = value;
            updateNode(targetNode);
        }else{
            if( cap != 0 && cache.size() == cap ){
                Node * delNode = head;
                head = head->next;
                if( head == NULL ) tail = NULL;
                delete[] delNode->buffer;
                cache.erase(key);
                delete delNode;
            }
            if( cap > 0 ){
                Node *newNode = new Node(value, NULL, NULL);
                if(tail != NULL){
                    tail->next = newNode;
                    newNode->pre = tail;
                    tail = newNode;
                }else{
                    head = tail = newNode;
                }
                cache[key] = newNode;
            }
        }
        return 1;
    }
};

#endif
