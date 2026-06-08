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

    // helpers
    void loadFromFile(string filename);
    string getCurrentPath();
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
