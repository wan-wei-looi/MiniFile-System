#include <iostream>
#include <string>
#include <vector>

#include "Folder.h"
#include "File.h"

using namespace std;

void Folder::folderPrintTree(string prefix, bool isLast)const{
    
    //Display subfolder
    //Prefix is the line to connect all of the parent of parents
    
    for(int sub = 0; sub < subfolders.size(); sub++){

        isLast = (sub == subfolders.size() - 1 && files.empty()) ? true : false;

        cout << prefix << ((isLast) ? "└── " : "├── ")
             << subfolders[sub]->name << endl;

        string newPrefix = prefix + ((isLast) ? "    " : "│   ");
        subfolders[sub]->folderPrintTree(newPrefix, isLast);
    }
    
    //Display files
    for(int f = 0; f < files.size(); f++){
        cout << prefix;
        cout << ((f != files.size() - 1) ? "├── " : "└── ");
        cout << files[f].getFileFullName() << endl;
    }
}

File* Folder::fileSearch(const string& fileName, bool showPath){
     for(File& file : files){
          if(fileName == file.getFileFullName()){
               if(showPath){
                    cout << "[LOCATION]: ";
                    cout << this->folderTraverse();
                    cout << endl;
               }
               return &file;
          }
     }
     for(Folder* sub : subfolders){
          File* result = sub->fileSearch(fileName, showPath);
          if(result != nullptr){
               return result;
          }
     }
     return nullptr;
}

Folder* Folder::folderSearch(const string& folderName){
     for(Folder* sub : subfolders){
          if(folderName == sub->getFolderName()){
               return sub;
          }
          Folder* result = sub->folderSearch(folderName);
          if(result != nullptr){
               return result;
          }
     }
     return nullptr;
}

bool Folder::deleteFile(string fileName, int index){
     if(index >= 0 && index < files.size()){
          if(fileName == files[index].getFileFullName()){
               files.erase(files.begin() + index);
               return true;
          }else{
               return deleteFile(fileName, index + 1);
          }
     }else{
          return false;
     }
}

bool Folder::deleteFolder(string folderName, int index){
     if(index >= 0 && index < subfolders.size()){
          if(folderName == subfolders[index]->getFolderName()){
               delete subfolders[index];
               subfolders.erase(subfolders.begin() + index);
               return true;
          }else{
               return deleteFolder(folderName, index + 1);
          }
     }else{
          return false;
     }
}

string Folder::folderTraverse(){
     if(this->getParent() != nullptr){
          return (this->getParent()->folderTraverse() + "/" + getFolderName());
     }else{
          return this->getFolderName();
     }
}

void Folder::printFileList()const{
     int index = 1;
     for(File file : files){
          cout << "[FILE " << index++ << "]: "
               << file.getFileFullName() << endl;
     }
}

void Folder::printSubfolderList()const{
     int index = 1;
     for(Folder* sub : subfolders){
          cout << "[Folder " << index++ << "]: "
               << sub->getFolderName() << endl;
     }
}
