#ifndef _H_CSV
#define _H_CSV

#include <vector>
#include <string>

/**
 * Load file list of sample's features matrix and list of samples classes from a CSV file.
 * CSV file coloumns:  "<CLASSLABEL>,<FEATURESFILEPATH>"
 *
 * @param csvFile The CSV file to load
 * @param sampleFiles Vector where to save samples feature file paths
 * @param sampleClasses Vector where to save samples classes
 * @return  
 * */
bool csv_load_traindata(const char * csvFile, std::vector<std::string> & sampleFiles, std::vector<int> & sampleClasses); 
/**
 * Returns the number of coloumns in csv file
 *
 * @param csvFile
 * */
int csv_ncoloumns(const char * csvFile);
/**
 * Returns the number of lines in csv file
 * 
 * @param csvFile
 * */
int csv_nlines(const char * csvFile);

#endif // _H_CSV
