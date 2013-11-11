/**
 *
 * */
#include "csv.h"
#include <fstream>
#include <iostream>     

using namespace std;

bool csv_load_traindata(const char * csvFile, std::vector<std::string> & sampleFiles, std::vector<int> & sampleClasses) { 
  try {
    ifstream csvf(csvFile);
    string sampleF;
    string sampleC;
    int sclass;
    while( getline(csvf, sampleC, ',') ) {
      getline(csvf, sampleF, '\n');
      if (sampleC==string("N")) {
        sclass=0;
      } else {
        sclass=1;
      }
      #ifdef DEBUG
      cout<<"DEBUG: found sample ("<<sampleC<<":"<<sclass<<") "<<sampleF<<endl;
      #endif 
      sampleFiles.push_back(sampleF);
      sampleClasses.push_back(sclass);
    }
    return true; 
	} catch (int e) {
		cerr<<"ERR: Something wrong during csv loading #"<<e<<endl;
    return false;
	}
}
int csv_ncoloumns(const char * csvFile){
  try {
    int n=0;
    ifstream csvf(csvFile);
    string dummy;
    while( getline(csvf, dummy, ',') ){n++;}
    return n; 
	} catch (int e) {
		cerr<<"ERR: Something wrong during csv loading #"<<e<<endl;
    return -1;
	}
}
int csv_nlines(const char * csvFile){
	cerr<<"ERR: Not yet implemented!"<<endl;
  return 0;
}
