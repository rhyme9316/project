//
//  main.cpp
//  BuildIndex
//
//  Created by DongYun on 10/1/15.
//  Copyright (c) 2015 DongYun. All rights reserved.
//


#include "Input.h"
#include "GenWordList.h"
#include "Merge.h"
#include <time.h>

int mergeFiles();

/*int main(){
    
    Input input;
    
    clock_t start, end;
    start = clock();
    
    vector<string> inputPath;
    input.readInputPath(inputPath);

    // process input files , each time processed fixed number of files
    for (int i = 0; i < inputPath.size(); i++){
        int count=0;

        input.inputClear();
        while(count < FILE_NUMBER_LIMIT && i < inputPath.size()){
            string dataPath = inputPath[i++];
            string indexPath = inputPath[i++];
            if(input.setInput(indexPath,dataPath)){
                input.gzLoadIndex();
                input.gzLoadData();
                count++;
            }
            else
                break;
   
        }
        i--;
    
        // generate compressed binary intermediate files
        GenWordList genWordList;
        genWordList.sortAllWords(input.pages);
        //printDocInfo();
        
    }
    printDocInfo();
    // merge all intermediate files through several phases
    mergeFiles();
    end = clock();
    double duration = (double)(end -start)/CLOCKS_PER_SEC;
    cout << "cost "<< duration <<"seconds"<<endl;
    
    return 0;
}*/

int main() {    
    cout << "here"<<endl;
    Input input;
    input.setInput(inputPath + "293.gz");
    vector<Page> pages;
    long long pages_size = 0;
    while(input.gzLoad(pages)){
        if(pages.size() > 5000) {
            cout <<pages.size()<<endl;
            pages_size += pages.size();
            //parse
            GenWordList genWordList;
            genWordList.sortAllWords(pages);
            pages.clear();
            //printDocInfo();
        }
    }
    cout << bytes <<endl;
  //  cout << "pages_size = "<< pages_size<<endl;
    printDocInfo();
    input.close();
    return 0;
    
}

int mergeFiles(){
    clock_t start, end;
    start = clock();
    int rounds=0;
    bool flag=1;
    while(flag) { //keep merging until number of file less than 2
        int fileCount = 0;
        int fileNextCount = 0;
        while(1) { // one round of merge
            string dataPath1 = basePath + "interFile_data_"+int2str(rounds)+"_"+int2str(fileCount);
            string indexPath1 = basePath+"interFile_index_"+int2str(rounds)+"_"+int2str(fileCount++);
            string dataPath2 = basePath +"interFile_data_"+int2str(rounds)+"_"+int2str(fileCount);
            string indexPath2 = basePath + "interFile_index_"+int2str(rounds)+"_"+int2str(fileCount++);
            string outputDataPath = basePath + "interFile_data_"+int2str(rounds+1)+"_"+int2str(fileNextCount);
            string outputIndexPath = basePath + "interFile_index_"+int2str(rounds+1)+"_"+int2str(fileNextCount++);
            
            cout<<dataPath1<<"  "<<dataPath2<<endl;
            Merge M;
            int fileOpened = M.initial(dataPath1,dataPath2,indexPath1,indexPath2);
            if(fileOpened<4 ){
                M.close();
                if( fileCount == 2 ){
                    flag =0;
                    cout <<"All merged"<<endl;
                }
                else {
                    cout <<"One round Complete."<<endl;
                    if(fileOpened == 2){
                        ifstream inputData(dataPath1,ios::binary);
                        ifstream inputIndex(indexPath1);
                        ofstream outputData(outputDataPath);
                        ofstream outputIndex(outputIndexPath);
                        outputData<<inputData.rdbuf();
                        outputIndex<<inputIndex.rdbuf();
                        inputData.close();
                        inputIndex.close();
                        outputData.close();
                        outputIndex.close();
                    }
                    rounds++;
                }
                break;
            }
            M.setOutput(outputDataPath, outputIndexPath);
            M.readIndex();
            M.startMerging();
            M.close();
            
        }
    }
    end = clock();
    double duration = (double) (end - start)/CLOCKS_PER_SEC;
    cout << "time cost is = "<<duration<<endl;
    return 0;
}
