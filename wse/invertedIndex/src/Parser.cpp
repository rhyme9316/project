/*
 * Parser.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: dongyun
 */
#include "Parser.h"


Parser::Parser(unsigned int len){
    buf = (char*)malloc(sizeof(char)*(2*len + 1));
}

Parser:: ~Parser(){
    free(buf);
    //cout<<"in !parser()"<<endl;
}


void Parser::resetBuf(){
    if(buf!=NULL)
        free(buf);
    buf = NULL;
}

void Parser::resetPos(){
    this->pos.clear();
   // cout<<"in reset: "<<pos.size()<<endl;
}
string Parser::getBuf(){
    string str = (const char*)buf;
    return str;
}

vector<int> Parser::getPos(){
    return pos;
}

int Parser::tag_parser(char* tag, int len, char* back_tag)
{
    int i = 0;
    
    if (tag[0] == '/')
    {
        *back_tag = 1;
        i++;
        
    } else
        *back_tag = 0;
    
    switch (tag[i])
    {
        case 'b':
        case 'B':
        case 'i':
        case 'I':
            if (!isspace(tag[i+1]))
                return 0;
            if ((tag[i] == 'b') || (tag[i] == 'B'))
                return PTAG_B;
            return PTAG_I;
            
        case 'e':
        case 'E':
            i++;
            if (((tag[i]=='m')||(tag[i]=='M')) && (isspace(tag[i+1])))
                return PTAG_I;
            return 0;
            
        case 'h':
        case 'H':
            i++;
            if (((tag[i]>='1')&&(tag[i]<='6')) && (isspace(tag[i+1])))
                return PTAG_H;
            return 0;
            
        case 't':
        case 'T':
            i++;
            if ((0==strncasecmp(tag+i, "itle", 4)) && (isspace(tag[i+4])))
                return PTAG_TITLE;
            return 0;
            
        case 's':
        case 'S':
            i++;
            if ((0==strncasecmp(tag+i, "trong", 5)) && (isspace(tag[i+5])))
                return PTAG_B;
            if ((0==strncasecmp(tag+i, "cript", 5)) && (isspace(tag[i+5])))
                return PTAG_SCRIPT;
            return 0;
            
        default:
            break;
    }
    
    return 0;
}

char* Parser::parser_init(char* doc)
{
    char *p;
    
    if (strncasecmp(doc, "HTTP/", 5))
        return NULL;
    
    for (p = doc; (*p != ' ')&&(*p); p++);
    if (*p == '\0')
        return NULL;
    
    if (atoi(p) != 200)
        return NULL;
    
    p = strstr(p,  "\r\n\r\n");
    if (p == NULL)
        return NULL;
    
    return p+4;
}



int Parser::parser(char* doc, unsigned int len){
    char *p , *word, *ptag = NULL, *pbuf;
    char ch, back_tag, intag, inscript;
    unsigned tag_flag;
    int ret;
    int blen = 4*len +2;
    p = parser_init(doc);
    if (p == NULL)
        return 0;
    pbuf = buf;
    
    /* parsing URL */
/*    purl = url;
    while (*purl != '\0')
    {
        if (!xl_isindexable(*purl))
        {
            purl++;
            continue;
        }
        
        word = purl;
        while (xl_isindexable(*purl))
        {
            if (xl_isupper(*purl))
                xl_tolower(*purl);
            purl++;
        }
        
        ch = *purl;
        *purl = '\0';
        
        if (pbuf-buf+purl-word+3 > blen-1)
            return -1;
        sprintf(pbuf, "%s U\n", word);
        pbuf += (purl-word)+3;
        
        *purl = ch;
    }
    */
    /* parsing page */
    tag_flag = 0;
    intag = 0;
    inscript = 0;
    
    while (*p != '\0')
    {
       // cout<<"pbuf - buf = "<< (pbuf - buf)<<endl;
        if (!xl_isindexable(*p))
        {
            if (*p != '>')
            {
                if (*p == '<')
                {
                    ptag = p;
                    intag = 1;
                }
                p++;
                continue;
            }
            
            *p = ' ';
            if(ptag != NULL){
                ret = tag_parser(ptag+1, int(p-ptag), &back_tag);
            switch (ret)
            {
                case PTAG_B:
                    
                    if (back_tag == 0)
                        xlbit_set(tag_flag, _B_TAG);
                    else
                        xlbit_unset(tag_flag, _B_TAG);
                    break;
                    
                case PTAG_I:
                    
                    if (back_tag == 0)
                        xlbit_set(tag_flag, _I_TAG);
                    else
                        xlbit_unset(tag_flag, _I_TAG);
                    break;
                    
                case PTAG_H:
                    
                    if (back_tag == 0)
                        xlbit_set(tag_flag, _H_TAG);
                    else
                        xlbit_unset(tag_flag, _H_TAG);
                    break;
                    
                case PTAG_TITLE:
                    
                    if (back_tag == 0)
                        xlbit_set(tag_flag, _TITLE_TAG);
                    else
                        xlbit_unset(tag_flag, _TITLE_TAG);
                    break;
                    
                case PTAG_SCRIPT:
                    
                    if (back_tag == 0)
                        inscript = 1;
                    else
                        inscript = 0;
                    
                default:
                    break;
            }
            
            intag = 0;
            p++;
            continue;
        }
        }
        
        if (inscript || intag)
        {
            p++;
            continue;
        }
        
        word = p;
        int cur_pos = int(p-doc);
        while (xl_isindexable(*p))
        {
            if (xl_isupper(*p))
                xl_tolower(*p);
            p++;
        }
        
        ch = *p;
        *p = '\0';
        
        if (pbuf-buf+p-word+1 > blen-1)
            return -1;
        sprintf(pbuf, "%s ", word);
        pbuf += (p-word)+1;
        pos.push_back(cur_pos);
        //printf("%s*%d\n",word,cur_pos);
        
        
        if (xlbit_check(tag_flag, _B_TAG))
        {
            if (pbuf-buf+1> blen-1)
                return -1;
            *pbuf = 'B';
            pbuf++;
        }
        
        if (xlbit_check(tag_flag, _H_TAG))
        {
            if (pbuf-buf+1> blen-1)
                return -1;
            *pbuf = 'H';
            pbuf++;
        }
        
        if (xlbit_check(tag_flag, _I_TAG))
        {
            if (pbuf-buf+1> blen-1)
                return -1;
            *pbuf = 'I';
            pbuf++;
        }
        
        if (xlbit_check(tag_flag, _TITLE_TAG))
        {
            if (pbuf-buf+1> blen-1)
                return -1;
            *pbuf = 'T';
            pbuf++;
        }
        
        if (tag_flag == 0)
        {
            if (pbuf-buf+1> blen-1)
                return -1;
            *pbuf = 'P';
            pbuf++;
        }
        
        if (pbuf-buf+1> blen-1)
            return -1;
        *pbuf = '\n';
        pbuf++;
        *p = ch;
    }
    
    *pbuf = '\0';
    return int(pbuf-buf);
    
}



