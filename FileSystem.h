#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <string>
#include <fstream>
#include "Folder.h"
#include "File.h"

using namespace std;

class FileSystem {
private:
    Folder* root;
    Folder* current;

public:
    FileSystem(string filename);
    ~FileSystem();
    void run();

    // exception handling class
    class ExceptionHandling{
    private:
        string errorMsg;
    public:
        ExceptionHandling(string msg) : errorMsg(msg){}
        string getErrorMsg()const{return errorMsg;}

    };

    // helpers
    void loadFromFile(string filename);
    bool recursiveSearch(Folder* node, string target, string& foundPath, string currentPath);

    // menu operations
    void createFolder();
    void createFile();
    void displayCurrentFolder();
    void displayFullTree();
    void searchFile();
    void enterFolder();
    void goBack();
    void renameFile();
    void removeFile();
    void renameFolder();
    void removeFolder();
    void showCurrentPath();
};

#endif
