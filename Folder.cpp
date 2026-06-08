#include <iostream>
#include <string>
#include <vector>

#include "Folder.h"
#include "File.h"

using namespace std;

void Folder::renameFile(){
     string name("");
     string ext("");
     //display file names and index
     int index = 0;
     for(File fl : files){
          cout << endl;
          cout << ++index << ".\t";
          cout << fl.getFileFullName();
          cout << endl;
     }
    

     //let user pick the index
     int choice;
     cout << endl;
     cout << "Please select the file you wish to manage: ";
     cin >> choice; 
     cin.ignore();

     while(choice> (int)files.size()||choice < 1){
        cout << "Invalid choice, please choose within range 1-" << files.size() << ": ";
        cin >> choice;
        cin.ignore();
     }

     index = --choice;

     //access the file object
     //use setFileName() & setFileExt()
     cout << endl;
     cout << "Please rename your file: (original file name: " + files[index].getFileFullName() + ")";
     cout << endl;
     getline(cin,name,'.'); getline(cin,ext);
     files[index].setFileName(name);
     files[index].setFileExt(ext);

     cout << endl;
     cout << "Congratulations, file successfully renamed to " + files[index].getFileFullName();
     cout << endl;
}

void Folder::renameSubFolder(){
     string name("");

     //display file names and index
     int index = 0;
     for(Folder* f_ptr : subfolders){
          cout << endl;
          cout << index++ << ".\t";
          cout << f_ptr->getFolderName();
          cout << endl;
     }
    
     //let user pick the index
     cout << endl;
     cout << "Please select the folder you wish to manage: ";
     cin >> index; cin.ignore(); --index;

     //access the file object
     cout << endl;
     cout << "Please rename your folder: (original file name: " + subfolders[index]->getFolderName() + ")";
     cout << endl;
     getline(cin,name);
     subfolders[index]->setFolderName(name);

     cout << endl;
     cout << "Congratulations, folder successfully renamed to " + subfolders[index]->getFolderName();
     cout << endl;
}

void Folder::folderTraversal(string prefix, bool isLast)const{
    
    //Display subfolder
    //Prefix is the line to connect all of the parent of parents
    
    for(int sub = 0; sub < (int)subfolders.size(); sub++){

        isLast = (sub == (int)subfolders.size() - 1 && files.empty()) ? true : false;

        cout << prefix << ((isLast) ? "└── " : "├── ")
             << subfolders[sub]->name << endl;

        string newPrefix = prefix + ((isLast) ? "    " : "│   ");
        subfolders[sub]->folderTraversal(newPrefix, isLast);
    }
    
    //Display files
    for(int f = 0; f < (int)files.size(); f++){
        cout << prefix;
        cout << ((f != (int)files.size() - 1) ? "├── " : "└── ");
        cout << files[f].getFileFullName() << endl;
    }
}

Folder* Folder::folderSearch(string folderName, int index){
     if(index >= 0 && index < subfolders.size()){
          if(folderName == subfolders[index]->getFolderName()){
               return subfolders[index];
          }else{
               Folder* folder = subfolders[index]->folderSearch(folderName);
               if(folder != nullptr){
                    return folder;
               }else{
                    return folderSearch(folderName, index + 1);
               }
          }
     }else{
          return nullptr;
     }
}

void Folder::deleteFile(int index){
    if(index == files.size()){
        throw runtime_error("File not found.");
    }
    
    files.erase(files.begin() + index);
}

void Folder::deleteFolder(Folder* folder, int index){ //default index = 0 for recursive call
     //delete subfolder(recursively)
     cout << folder->subfolders.size() << " subfolder(s) to delete\n";
     while(folder->subfolders.size() > 0){
          cout << "entering recursive function\n";
          deleteFolder(folder->getSubfolder(0));
          cout << "exited recursive function\n";
     }

     //delete file(from last index)
     while(folder->files.size() > 0){
          cout << "deleting file\n";
          folder->files.pop_back();
          cout << "deleted one file\n";
     }

     //done delete all files and subfolders, now delete itself
     cout << "deleting one folder\n";
     delete folder;
     folder->parent->subfolders.erase(folder->parent->subfolders.begin() + index);
     cout << "deleted one folder\n";
}
