#ifndef FOLDER_H
#define FOLDER_H

#include <iostream>
#include <string>
#include <vector>
#include "File.h"

using namespace std;

class Folder{
private:
    string name;
    Folder* parent;
    vector<File> files;
    vector<Folder*> subfolders;

public:
    Folder(string n, Folder* f_ptr = nullptr): name(n), parent(f_ptr){}

    ~Folder(){
        for(Folder* sub : subfolders){
            delete sub;
        }
    }
    
    void setFolderName(string n){name = n;}
    string getFolderName()const{return name;}

    //manage file
    void addFile(string fileName, string ext){
        files.push_back(File(fileName, ext));
    }

    //manage subfolder
    void addSubfolder(string folderName){
        subfolders.push_back(new Folder(folderName, this));
    }
    
    //recursive printing
    void folderPrintTree(string = "", bool = true)const;

    //recursive searching
    File* fileSearch(const string&, bool = false);
    Folder* folderSearch(const string&);

    //deletion
    bool deleteFile(string, int = 0);
    //recursive deletion
    bool deleteFolder(string, int = 0);

    //Helper functions
    Folder* getParent(){return parent;}
    
    int getFileCount()const{return files.size();}
    
    int getSubfolderCount()const{return subfolders.size();}
    
    File& getFile(int i){return files[i];}
    
    Folder* getSubfolder(int i){return subfolders[i];}
    
    //recursive transversal
    string folderTraverse();

    void printFileList()const;

    void printSubfolderList()const;
};

#endif
