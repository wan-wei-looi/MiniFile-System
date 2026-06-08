#ifndef FOLDER_H
#define FOLDER_H

#include <iostream>
#include <string>
#include <vector>
#include "File.h"

class Folder{
private:
    std::string name;
    Folder* parent;
    std::vector<File> files;
    std::vector<Folder*> subfolders;

public:
    Folder(std::string n, Folder* f_ptr): name(n), parent(f_ptr){}

    ~Folder(){ 
        for(Folder* sub : subfolders){
            delete sub;
        }
    }
    
    void setFolderName(std::string n){name = n;}
    std::string getFolderName()const{return name;}

    //manage file
    void renameFile();
    
    void addFile(std::string fileName, std::string ext){
        files.push_back(File(fileName, ext));
    }

    //manage subfolder
    void renameSubFolder();
    void addSubfolder(std::string folderName){
        subfolders.push_back(new Folder(folderName, this));
    }
    //recursive printing
    void folderTraversal(std::string = "", bool = true)const;

    //recursive searching
    Folder* folderSearch(std::string, int = 0);

    //deletion
    void deleteFile(int);
    //recursive deletion
    void deleteFolder(Folder*, int = 0);

    //Helper function
    int getSubfolderCount() const { return subfolders.size(); }

    Folder* getSubfolder(int i) { return subfolders[i]; }

    Folder* getParent(){return parent;}

    int getFileCount()  const { return files.size(); }
    
    File getFile(int i)  const { return files[i]; }

};

#endif
