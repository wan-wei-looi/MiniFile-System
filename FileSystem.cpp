#include <iostream>
#include <string>
#include <vector>

#include "Folder.h"
#include "File.h"
#include "FileSystem.h"

using namespace std;

enum menuOptions{
    CREATE_FOLDER = 1,
    CREATE_FILE,
    DISPLAY_CURRENT_FOLDER,
    DISPLAY_FULL_FOLDER_TREE,
    SEARCH_FILE,
    ENTER_FOLDER,
    GO_BACK_TO_PARENT_FOLDER,
    RENAME_FILE,
    RENAME_FOLDER,
    DELETE_FILE,
    DELETE_FOLDER,
    SHOW_CURRENT_PATH,
    EXIT
};

FileSystem::FileSystem(string filename){
    root = new Folder("Root",nullptr);
    current = root; 
    loadFromFile(filename);
}

FileSystem::~FileSystem(){ delete root;}

void FileSystem::run(){

    string fileName, folderName, extension;

    menuOptions userChoice;

    do{

        cout << endl
        << "Current Path: " << getCurrentPath() << endl
        << "===============Menu===============\n"
        << "1.  Create Folder\n"
        << "2.  Create File\n"
        << "3.  Display Current Folder\n"
        << "4.  Display Full Folder Tree\n"
        << "5.  Search File\n"
        << "6.  Enter Folder\n"
        << "7.  Go Back to Parent Folder\n"
        << "8.  Rename File\n"
        << "9.  Rename Folder\n"
        << "10.  Delete File\n"
        << "11. Delete Folder\n"
        << "12. Show Current Path\n"
        << "13. Exit\n"
        << "==================================\n"
        << "Enter your choice: ";
        
        int input;
        cin >> input;
        cin.ignore();
        userChoice = static_cast<menuOptions>(input);
        
        switch(userChoice){
            
            case CREATE_FOLDER: this->createFolder(); break;
            
            case CREATE_FILE: this->createFile(); break;
            
            case DISPLAY_CURRENT_FOLDER: this->displayCurrentFolder(); break;

            case DISPLAY_FULL_FOLDER_TREE: this-> displayFullTree(); break;

            case SEARCH_FILE: this->searchFile(); break;

            case ENTER_FOLDER: this->enterFolder(); break;

            case GO_BACK_TO_PARENT_FOLDER: this->goBack(); break;

            case RENAME_FILE: this->renameFile(); break;

            case RENAME_FOLDER: this->renameFolder(); break;
            
            case DELETE_FILE: this->removeFile(); break;

            case DELETE_FOLDER:this->removeFolder(); break;

            case SHOW_CURRENT_PATH: this->showCurrentPath(); break;

            case EXIT: cout << "Exiting program. Goodbye!\n"; break;

            default: cout << "Invalid choice. Please try again.\n";
        }

    }while (userChoice != EXIT);
}

void FileSystem::loadFromFile(string filename){

    ifstream fin(filename);

    if(!fin.is_open()){
        cout << "Warning: could not open " << filename
             << ". Starting with empty filesystem.\n";
        return;
    }

    string line;

    while(getline(fin, line)){
        if(line.empty()) continue;

        if(line.substr(0, 6) == "FOLDER"){

            string path = line.substr(7);

            // split into segments
            vector<string> segments;

            string segment = "";
            for(int i = 0; i < (int)path.size(); i++){

                if(path[i] == '/'){
                    segments.push_back(segment);
                    segment = "";
                } else {
                    segment += path[i];
                }
            }
            segments.push_back(segment);

            // 1st: child name
            string childName = segments.back();

            // 2nd: store parents and skip the Root by using int i=1 and stop before the child
            vector<string> parents;
            for(int i = 1; i < (int)segments.size() - 1; i++){
               parents.push_back(segments[i]);
            }

            // 3rd: walk parents then add child
            Folder* node = root;
            for(int i = 0; i < (int)parents.size(); i++){
                for(int j = 0; j < (int)node->getSubfolderCount(); j++){
                    if(node->getSubfolder(j)->getFolderName() == parents[i]){
                        node = node->getSubfolder(j);
                        break;
                    }
                }
            }
            node->addSubfolder(childName);

        } else if(line.substr(0, 4) == "FILE"){
            string path = line.substr(5);

            // split into segments
            vector<string> segments;
            string segment = "";
            for(int i = 0; i < (int)path.size(); i++){
                if(path[i] == '/'){
                    segments.push_back(segment);
                    segment = "";
                } else {
                    segment += path[i];
                }
            }
            segments.push_back(segment);

            // 1st: file field
            string fileField = segments.back();

            // 2nd: store parents
            vector<string> parents;
            for(int i = 1; i < (int)segments.size() - 1; i++){
               parents.push_back(segments[i]);
            }

            // parse name and extension
            string name = "";
            string ext  = "";
            size_t dot  = fileField.rfind('.');
            if(dot != string::npos){
                name = fileField.substr(0, dot);
                ext  = fileField.substr(dot + 1);
            }

            // 3rd: walk parents then add file
            Folder* node = root;
            for(int i = 0; i < (int)parents.size(); i++){
                for(int j = 0; j < (int)node->getSubfolderCount(); j++){
                    if(node->getSubfolder(j)->getFolderName() == parents[i]){
                        node = node->getSubfolder(j);
                        break;
                    }
                }
            }
            node->addFile(name, ext);
        }
    }

    fin.close();
}

string FileSystem::getCurrentPath(){

    string path = "";
    Folder* node = current;
    
    // when node reach the root and its parent(next node) will be nullptr
    while(node != nullptr){
        if(path.empty())
            path = node->getFolderName();
        else
            path = node->getFolderName() + "/" + path;

        node = node->getParent();
    }
    return path;   // returns "Root/Courses/OOP"
}

bool FileSystem::recursiveSearch(Folder* node, string target, string& foundPath, string currentPath){
    // check all files in current node
    for(int i = 0; i < (int)node->getFileCount(); i++){
        if(node->getFile(i).getFileFullName() == target){
            foundPath = currentPath + "/" + target;  // save the full path
            return true;                              // found, stop searching
        }
    }

    // not found in this folder, recurse into each subfolder
    for(int i = 0; i < (int)node->getSubfolderCount(); i++){
        Folder* sub = node->getSubfolder(i);
        bool found = recursiveSearch(sub, target, foundPath, currentPath + "/" + sub->getFolderName());
        if(found) return true;   // bubble the result back up
    }

    return false;   // not found in this branch
}

void FileSystem::createFolder(){

    string name;
    cout << "\nEnter folder name: ";
    getline(cin, name);

    try {
        // check 1: empty name
        if(name.empty())
            throw runtime_error("Folder name cannot be empty.");

        // check 2: duplicate name
        for(int i = 0; i < (int)current->getSubfolderCount(); i++){
            if(current->getSubfolder(i)->getFolderName() == name)
                throw runtime_error("Folder '" + name + "' already exists here.");
        }

        // passed both checks, safe to create
        current->addSubfolder(name);
        cout << "\nFolder '" << name << "' created successfully.\n";

    } catch(runtime_error& e){
        cout << "\nError: " << e.what() << "\n";
    }
}

void FileSystem::createFile(){

    string name,ext;
    cout << "\nEnter file name: ";
    getline(cin, name);
    cout << "Enter file extension(without dot): ";
    getline(cin,ext);

    try {
        // check 1: empty name
        if(name.empty())
            throw runtime_error("File name cannot be empty.");
        
        //check 2: empty file extension
        if(ext.empty())
            throw runtime_error("File extension cannot be empty.");

        // check 3: duplicate full name
        for(int i = 0; i < (int)current->getFileCount(); i++){
            if(current->getFile(i).getFileFullName() == name + "." + ext)
                throw runtime_error("File '" + name + "."  + ext + "' already exists here.");
        }

        // passed both checks, safe to create
        current->addFile(name,ext);
        cout << "\nFile '" << name + "." + ext << "' created successfully.\n";

    } catch(runtime_error& e){
        cout << "\nError: " << e.what() << "\n";
    }
}

void FileSystem::displayCurrentFolder(){

    cout<<  "______________________________________________";
    cout << "Current Folder: " << current->getFolderName() << endl;
    cout << "Current Path  : " << getCurrentPath() << endl;
    cout << "_______________________________________________" << endl;

    if(current->getSubfolderCount() == 0 && current->getFileCount() == 0){
        cout << "(empty)" << endl;
        return;
    }

    for(int i = 0; i < (int)current->getSubfolderCount(); i++){
        cout << "[FOLDER]: " << current->getSubfolder(i)->getFolderName() << endl;
    }

    for(int i = 0; i < (int)current->getFileCount(); i++){
        cout << "[FILE]: " << current->getFile(i).getFileFullName() << endl;
    }
}

void FileSystem::displayFullTree(){
    cout << "\n" << root->getFolderName() << endl;
    root->folderTraversal();
}

void FileSystem::searchFile(){
    string target;
    cout << "\nEnter file name to search (e.g. notes.pdf): ";
    getline(cin, target);

    try {
        if(target.empty())
            throw runtime_error("File name cannot be empty.");

        string foundPath = "";
        bool found = recursiveSearch(root, target, foundPath, root->getFolderName());

        if(!found)
            throw runtime_error("File '" + target + "' not found in the filesystem.");

        cout << "\nFile found at: " << foundPath << "\n";

    } catch(runtime_error& e){
        cout << "\nError: " << e.what() << "\n";
    }
}

void FileSystem::enterFolder(){

    string name;
    cout << "\nEnter folder name to enter: ";
    getline(cin, name);

    try {
        if(name.empty())
            throw runtime_error("Folder name cannot be empty.");

        for(int i = 0; i < (int)current->getSubfolderCount(); i++){
            if(current->getSubfolder(i)->getFolderName() == name){
                current = current->getSubfolder(i);  // move into folder
                cout << "\nEntered folder: " << name << "\n";
                return;  // exit function after entering
            }
        }

        // if loop finishes without finding, throw
        throw runtime_error("Folder '" + name + "' does not exist in '" + current->getFolderName() + "'.");

    } catch(runtime_error& e){
        cout << "\nError: " << e.what() << "\n";
    }
}

void FileSystem::goBack(){
    try {
        if(current->getParent() == nullptr)
            throw runtime_error("Already at Root. Cannot go further back.");

        current = current->getParent();
        cout << "\nMoved back to: " << current->getFolderName() << "\n";

    } catch(runtime_error& e){
        cout << "\nError: " << e.what() << "\n";
    }
}

void FileSystem::renameFile(){
    //check if there is any file to rename
    if(current->getFileCount() ==0){
        cout<< endl
            << "No file to rename in current folder" << endl;
            return;
    }

    //display file names and index
    for(int i = 0; i < (int)current->getFileCount(); i++){
        cout << "[FILE " << (i+1) << "]: " << current->getFile(i).getFileFullName() << endl;
    }
    
    //let user pick the index
    int choice;
    cout << endl;
    cout << "Please select the file number you wish to rename: ";
    cin >> choice;
    cin.ignore();
    
    //input viladation: ensure choice within range of file couunt
    while(choice > (int)current->getFileCount()||choice < 1){
       cout << "Invalid choice, please choose within range 1-" << current->getFileCount() << ": ";
       cin >> choice;
       cin.ignore();
    }
    
    choice--; // adjust choice to align with 0-based indexing
    
    
    //rename the file object
    //use setFileName() & setFileExt()
    cout << endl;
    cout << "Please rename your file: (original file name: " << current->getFile(choice).getFileFullName() << ")";
    cout << endl;
    
    //accept input from user for new name and etension
    string name,ext;
    cout << "\nEnter file name(without extension): ";
    getline(cin, name);
    cout << "Enter file extension(without dot): ";
    getline(cin, ext);
    cin.ignore();

    try {
        // check 1: empty name
        if(name.empty())
            throw runtime_error("File name cannot be empty.");
        
        //check 2: empty file extension
        if(ext.empty())
            throw runtime_error("File extension cannot be empty.");

        // check 3: duplicate full name
        for(int i = 0; i < (int)current->getFileCount(); i++){
            if(current->getFile(i).getFileFullName() == name + "." + ext)
                throw runtime_error("File '" + name + "."  + ext + "' already exists here.");
        }

        // passed both checks, safe to rename
        current->getFile(choice).setFileName(name);
        current->getFile(choice).setFileExt(ext);
        cout<< endl
            << "File renamed successfully to " << current->getFile(choice).getFileFullName() << endl;

    } catch(runtime_error& e){
        cout << "\nError: " << e.what() << "\n";
    }

}

void FileSystem::renameFolder(){
    //check if there is any subfolder to rename
    if(current->getSubfolderCount() == 0){
        cout<< endl
            << "No subfolder to rename in current folder" << endl;
            return;
    }

    //display folder names and index
    for(int i = 0; i < (int)current->getSubfolderCount(); i++){
        cout << "[FOLDER " << (i+1) << "]: " << current->getSubfolder(i)->getFolderName() << endl;
    }
    
    //let user pick the index
    int choice;
    cout << endl;
    cout << "Please select the folder you wish to rename: ";
    cin >> choice;
    cin.ignore();
    
    //input viladation: ensure choice within range of file couunt
    while(choice > (int)current->getSubfolderCount() || choice < 1){
       cout << "Invalid choice, please choose within range 1-" << current->getSubfolderCount() << ": ";
       cin >> choice;
       cin.ignore();
    }
    choice--; // adjust choice to align with 0-based indexing

    //rename the subfolder object
    //use setFolderName()
    cout << endl;
    cout << "Please rename your subfolder: (original subfolder name: " << current->getSubfolder(choice)->getFolderName() << ")";
    cout << endl;
    
    //accept input from user for new subbfolder name
    string name;
    cout << "\nEnter folder name: ";
    getline(cin, name);

    try {
        // check 1: empty name
        if(name.empty())
            throw runtime_error("Folder name cannot be empty.");
        
        // check 2: duplicate folder name
        for(int i = 0; i < (int)current->getSubfolderCount(); i++){
            if(current->getSubfolder(i)->getFolderName() == name)
                throw runtime_error("Subfolder '" + name + "' already exists here.");
        }

        // passed both checks, safe to rename
        current->getSubfolder(choice)->setFolderName(name);
        cout<< endl
            << "Folder renamed successfully to " << current->getSubfolder(choice)->getFolderName() << endl;

    } catch(runtime_error& e){
        cout << "\nError: " << e.what() << "\n";
    }

}

void FileSystem::removeFile(){
    //check if there is any file to remove
    if(current->getFileCount() ==0){
        cout<< endl
            << "No file to delete in current folder" << endl;
            return;
    }

    //display file names
    for(int i = 0; i < (int)current->getFileCount(); i++){
        cout << "[FILE]: " << current->getFile(i).getFileFullName() << endl;
    }
    
    //receive file name from user to delete
    string fileName;
    cout << endl;
    cout << "Please select the file you wish to delete: ";
    getline(cin, fileName);

    //find index of the file to delete
    int index = 0;
    for(; index < (int)current->getFileCount(); index++){
        if(current->getFile(index).getFileFullName() == fileName){
            break;
        }
    }

    // if index is out of bound, means file not found, throw exception
    try{
        //throw exception if file not find
        current->deleteFile(index);
        cout << "\nFile '" << fileName << "' deleted successfully.\n";
    }
    catch(runtime_error& e){
        cout << "\nError: " << e.what() << endl;
    }

}

void FileSystem::removeFolder(){
    //check if there is any folder to remove
    if(current->getSubfolderCount() ==0){
        cout<< endl
            << "No subfolder to delete in current folder" << endl;
            return;
    }

    //display subfolder names
    for(int i = 0; i < (int)current->getSubfolderCount(); i++){
        cout << "[FOLDER]: " << current->getSubfolder(i)->getFolderName() << endl;
    }
    
    //receive subfolder name from user to delete
    string subfolderName;
    cout << endl;
    cout << "Please select the folder you wish to delete: ";
    getline(cin, subfolderName);

    //find index of the folder to delete
    int index = 0;
    for(; index < (int)current->getSubfolderCount(); index++){
        if(current->getSubfolder(index)->getFolderName() == subfolderName){
            break;
        }
    }

    // if index is out of bound, means subfolder not found, throw exception
    try{
        //throw exception if subfolder not find
        cout << "Attempting to delete folder\n";
        current->deleteFolder(current->getSubfolder(index), index);
        cout << "\nFile '" << subfolderName << "' and all its contents deleted successfully.\n";
    }
    catch(runtime_error& e){
        cout << "\nError: " << e.what() << endl;
    }

}

void FileSystem::showCurrentPath(){
    cout << "\nCurrent Path: " << getCurrentPath() << "\n";
}
