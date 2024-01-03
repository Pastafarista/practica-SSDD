// Authors: Antonio Cabrera y Alejandro Gómez
// Group: 3º MAIS  2023/2024

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <dirent.h>

using namespace std;

class FileManager
{
    string dirPath;
    map<string, string* > files;
public:
    FileManager(string path);
    vector<string*>* listFiles();
    void readFile(char* fileName, char* &data, unsigned long int & dataLength);
    void writeFile(char* fileName, char* data, unsigned long int dataLength);
    void freeListedFiles(vector<string*>* fileList);
    bool fileExists(char* fileName);
};

#endif // FILEMANAGER_H
