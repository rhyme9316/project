#include "Input.h"

int main(){
	Input input;
	input.setInput("D:\\data.warc");
	vector<Page> pages;
	while(input.gzLoad(pages)){
		if(pages.size() > 8000) {
			cout <<pages.size()<<endl;
			//parse
			pages.clear();
		}
	}
	input.close();
	return 0;
}