
#ifndef BuildIndex_Input_h
#define BuildIndex_Input_h


#include "Global.h"
#include <zlib.h>
#include <iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

#define GZ_BUF_SIZE 100000



class Input{
private:
	string inputPath;
	gzFile gzfp;
    //ifstream fp;
	unsigned int docID;
	//vector<Page> pages;
	string buffer;
	int ptr;
   // long long int bytes;
public:
	Input(){
		docID = 550195;
		ptr=0;
	}
	void close() {
		buffer.clear();
        //fp.close();
		gzclose(gzfp);
	}
	bool setInput(string inputPath){
		buffer.clear();
		ptr = 0;
        bytes = 0;
		//pages.clear();
		this->inputPath=inputPath;
		gzfp = gzopen(inputPath.c_str(),"rb");
        //fp.open(inputPath);
		if(!gzfp)
			return false;
		return true;
	}
	
	bool gzLoad(vector<Page>& pages)
	{
		//gzfp = gzopen(inputPath.c_str(),"rb");
		unsigned char buf[GZ_BUF_SIZE];
        int have;
        //fp.seekg(bytes);
        //fp.read((char*)buf, sizeof(char)*GZ_BUF_SIZE);
        
        /*
        long have = fp.gcount();
        bytes += have;
        if(have == 0){
            return false;
        }
        buffer.append((const char*)buf, sizeof(char)*have);
        while(extractPageInfo(pages)) {
        }
        buffer = buffer.substr(ptr);
        ptr = 0;
        return true;
         */
		if( (have = gzread(gzfp,buf,GZ_BUF_SIZE)) > 0){
            //bytes += have;
            //cout << have <<endl;
			buffer.append((const char*)buf,have);
			while(extractPageInfo(pages)) {
			}
			buffer = buffer.substr(ptr);
			ptr = 0;
			return true;
		}
		else {
			return false;
		}
        
    
	}

	bool extractPageInfo(vector<Page>& pages){
		int start = ptr;
		int content_len = 0;
		int i = ptr;
		bool isResponse = 0;
		string URL = "";
		string page;
		for(; i< buffer.size(); i++){
			if(i > 5 && buffer[i-6]=='R'&&buffer[i-5]=='C'&&buffer[i-4]=='-'&&buffer[i-3]=='T'&&buffer[i-2]=='y'&&buffer[i-1]=='p'&&buffer[i]=='e') {
				if(i+10>=buffer.size())
					return 0;
				if(buffer.substr(i+3,8) == "response") {
					isResponse = 1;
					break;
				}
			}
		}
		if(!isResponse)
			return 0;
		
		for(; i< buffer.size(); i++){
			if(i > 5 && buffer[i-6]=='L'&&buffer[i-5]=='e'&&buffer[i-4]=='n'&&buffer[i-3]=='g'&&buffer[i-2]=='t'&&buffer[i-1]=='h'&&buffer[i]==':') {
				int len_start = i + 2;
				while(i< buffer.size()&&buffer[i]!='\r')
					i ++;
				if(i == buffer.size() || buffer[i] != '\r')	
					return 0;
				content_len=atoi(buffer.substr(len_start, i - len_start).c_str());
				break;
			}
		}
		if(content_len == 0)
			return 0;

		for(; i< buffer.size(); i++){
			if(i > 5 && buffer[i-6]=='e'&&buffer[i-5]=='t'&&buffer[i-4]=='-'&&buffer[i-3]=='U'&&buffer[i-2]=='R'&&buffer[i-1]=='I'&&buffer[i]==':') {
				int len_start = i + 2;
				while(i< buffer.size()&&buffer[i]!='\r')
					i ++;
				if(i == buffer.size() || buffer[i] != '\r')	
					return 0;
				URL = buffer.substr(len_start, i - len_start);
				//cout<<URL<<endl;
				break;
			}
		}

		for(; i< buffer.size(); i++){
			if(i >2 && buffer[i-3] == '\r'&& buffer[i-2] == '\n' && buffer[i-1] == '\r' && buffer[i] == '\n' ){
				if(i+ content_len >= buffer.size())
					return 0;
				page = buffer.substr(i+1, content_len);
				//cout<< page;
				ptr = i+1+content_len;
				Page p(URL,docID++,page,content_len);
				pages.push_back(p);
				return 1;
			}
		}
		
		return 0;
	}

};

#endif