/*
 * Parser.h
 *
 *  Created on: Oct 3, 2015
 *      Author: dongyun
 */

#ifndef PARSER_H_
#define PARSER_H_
#include<iostream>
#include<vector>
#include<string>
using namespace std;

#define PTAG_B	1
#define PTAG_I	2
#define	PTAG_H	3
#define PTAG_TITLE	4
#define PTAG_SCRIPT	5

#define _TITLE_TAG	0x0001
#define _B_TAG		0x0004
#define _H_TAG		0x0008
#define _I_TAG		0x0010

#define xl_isdigit(c) (((c) >= '0') && ((c) <= '9'))
#define xl_islower(c) (((c) >= 'a') && ((c) <= 'z'))
#define xl_isupper(c) (((c) >= 'A') && ((c) <= 'Z'))
#define xl_isindexable(c) (xl_isdigit(c) || xl_islower(c) || xl_isupper(c))
#define xl_tolower(c) ((c) += 'a' - 'A')

#define xlbit_set(__b1, __b2)	((__b1) |= (__b2))
#define xlbit_unset(__b1, __b2)	((__b1) &= ~(__b2))
#define xlbit_check(__b1, __b2) ((__b1)&(__b2))

class Parser{
public:

    Parser(unsigned int len);
    ~Parser();
    
    void resetBuf();
    void resetPos();
    string getBuf();
    vector<int> getPos();
    
    // parse document char by char
    // and do not parse pages that has no 200ok status
    int parser(char* doc, unsigned int len);
private:
    char *buf;
    vector<int> pos;
    
    // parse html tags
    int tag_parser(char* tag, int len, char* back_tag);
    char* parser_init(char* doc);
    
};
#endif /* PARSER_H_ */
