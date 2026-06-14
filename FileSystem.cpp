#include <iostream>
#include <string>
#include <vector>

#include "Folder.h"
#include "File.h"
#include "FileSystem.h"

using namespace std;

enum menuOptions{
    CREATE_FILE = 1,
    CREATE_FOLDER,
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
    root = new Folder("Root");
    current = root; 
    loadFromFile(filename);
}

FileSystem::~FileSystem(){
    delete root;
    delete current;
}

void FileSystem::run(){
    menuOptions userChoice;
    do{
        cout << "\n===============Menu===============";
        showCurrentPath();
        cout << "==================================\n"
             << "1.  Create File\n"
             << "2.  Create Folder\n"
             << "3.  Display Current Folder\n"
             << "4.  Display Full Folder Tree\n"
             << "5.  Search File\n"
             << "6.  Enter Folder\n"
             << "7.  Go Back to Parent Folder\n"
             << "8.  Rename File\n"
             << "9.  Rename Folder\n"
             << "10. Delete File\n"
             << "11. Delete Folder\n"
             << "12. Show Current Path\n"
             << "13. Exit\n"
             << "==================================\n"
             << "Enter your choice: ";
        
        string input;
        getline(cin,input);
        cout << "==================================\n";
        try{
            userChoice = static_cast<menuOptions>(stoi(input));
        
            switch(userChoice){

                case CREATE_FILE: this->createFile(); break;
                
                case CREATE_FOLDER: this->createFolder(); break;
                
                case DISPLAY_CURRENT_FOLDER: this->displayCurrentFolder(); break;

                case DISPLAY_FULL_FOLDER_TREE: this->displayFullTree(); break;

                case SEARCH_FILE: this->searchFile(); break;

                case ENTER_FOLDER: this->enterFolder(); break;

                case GO_BACK_TO_PARENT_FOLDER: this->goBack(); break;

                case RENAME_FILE: this->renameFile(); break;

                case RENAME_FOLDER: this->renameFolder(); break;
                
                case DELETE_FILE: this->removeFile(); break;

                case DELETE_FOLDER:this->removeFolder(); break;

                case SHOW_CURRENT_PATH: this->showCurrentPath(); break;

                case EXIT: cout << "Exiting program. Goodbye!\n"; break;

                default: cout << "\nInvalid choice. Please try again.\n";
            }
        }
        catch(invalid_argument){
            cout << "\nInvalid choice. Please try again.\n";
        }
        catch(const ExceptionHandling& error){
            cout << "\n[Error] : " << error.getErrorMsg() << "\n";
        }
    }while(userChoice != EXIT);
}

void FileSystem::loadFromFile(string filename){
    ifstream fin(filename);
    if(!fin){
        cout << "Warning: could not open " << filename << ". Starting with empty filesystem.\n";
        return;
    }

    string line;
    while(getline(fin, line)){
        if(line.empty()) continue;
        if(line.substr(0, 6) == "FOLDER"){
            string path = line.substr(7);

            //split into segments
            vector<string> segments;

            string segment = "";
            for(int i = 0; i < path.size(); i++){
                if(path[i] == '/'){
                    segments.push_back(segment);
                    segment = "";
                }else{
                    segment += path[i];
                }
            }
            segments.push_back(segment);

            //1st: child name
            string childName = segments.back();

            //2nd: store parents and skip the Root by using int i=1 and stop before the child
            vector<string> parents;
            for(int i = 1; i < segments.size() - 1; i++){
               parents.push_back(segments[i]);
            }

            //3rd: walk parents then add child
            Folder* node = root;
            for(int i = 0; i < parents.size(); i++){
                for(int j = 0; j < node->getSubfolderCount(); j++){
                    if(node->getSubfolder(j)->getFolderName() == parents[i]){
                        node = node->getSubfolder(j);
                        break;
                    }
                }
            }
            node->addSubfolder(childName);

        }else if(line.substr(0, 4) == "FILE"){
            string path = line.substr(5);

            //split into segments
            vector<string> segments;
            string segment = "";
            for(int i = 0; i < path.size(); i++){
                if(path[i] == '/'){
                    segments.push_back(segment);
                    segment = "";
                }else{
                    segment += path[i];
                }
            }
            segments.push_back(segment);

            //1st: file field
            string fileField = segments.back();

            //2nd: store parents
            vector<string> parents;
            for(int i = 1; i < segments.size() - 1; i++){
               parents.push_back(segments[i]);
            }

            //parse name and extension
            string name = "";
            string ext  = "";
            name = fileField.substr(0, fileField.find('.'));
            ext  = fileField.substr(fileField.find('.') + 1);

            //3rd: walk parents then add file
            Folder* node = root;
            for(int i = 0; i < parents.size(); i++){
                for(int j = 0; j < node->getSubfolderCount(); j++){
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

void FileSystem::createFolder(){
    string folderName;
    cout << "\nEnter folder name: ";
    getline(cin, folderName);

    //check 1: empty name
    if(folderName.empty())
        throw ExceptionHandling("Folder name cannot be empty.");

    //check 2: duplicate name
    Folder* duplicateFolder = current->folderSearch(folderName);
    if(duplicateFolder != nullptr){
        throw ExceptionHandling("Folder '" + folderName + "' already exists.");
    }

    //check 3: blank spaces
    if(folderName == " " || folderName == "\t"){
        throw ExceptionHandling("Folder name can not only contain black spaces.");
    }

    //passed all checks, safe to create
    current->addSubfolder(folderName);
    cout << "\nFolder '" << folderName << "' created successfully.\n";    
}

void FileSystem::createFile(){
    string fileName;
    cout << "\nEnter file name (with extension): ";
    getline(cin, fileName);

    ////check 1: empty name
    if(fileName.empty())
        throw ExceptionHandling("File name cannot be empty.");
    
    //check 2: blank spaces
    if(fileName == " " || fileName == "\t"){
        throw ExceptionHandling("Folder name can not only contain black spaces.");
    }

    //check 3: no extension
    int dotPosition = fileName.rfind('.');
    if(dotPosition >= fileName.size() - 1){
        throw ExceptionHandling("File does not have an extension.");
    }
    
    //check 4: duplicate full name
    File* duplicateFile = current->fileSearch(fileName);
    if(duplicateFile != nullptr){
        throw ExceptionHandling("File '" + fileName + "' already exists here.");
    }
    
    //passed all checks, safe to create
    current->addFile(fileName.substr(0, dotPosition), fileName.substr(dotPosition + 1));
    cout << "\nFile '" << fileName << "' created successfully.\n";
}

void FileSystem::displayCurrentFolder(){

    cout << "_______________________________________________" << endl;
    cout << "Current Folder: " << current->getFolderName() << endl;
    cout << "Current Path  : " << current->folderTraverse() << endl;
    cout << "_______________________________________________" << endl;

    if(current->getSubfolderCount() == 0 && current->getFileCount() == 0){
        cout << "(empty)" << endl;
        return;
    }

    for(int i = 0; i < current->getSubfolderCount(); i++){
        cout << "[FOLDER] : " << current->getSubfolder(i)->getFolderName() << endl;
    }

    for(int i = 0; i < current->getFileCount(); i++){
        cout << "[FILE] : " << current->getFile(i).getFileFullName() << endl;
    }
}

void FileSystem::displayFullTree(){
    cout << endl
         << root->getFolderName() << endl;
    root->folderPrintTree();
}

void FileSystem::searchFile(){
    string fileName;
    cout << "\nEnter file name to search (e.g. notes.pdf): ";
    getline(cin, fileName);

    if(fileName.empty()){
        throw ExceptionHandling("Empty input is invalid.");
    }
    File* result = current->fileSearch(fileName, true);
    if(result == nullptr){
        throw ExceptionHandling("File does not exist in the current folder and its subfolders.");
    }
}

void FileSystem::enterFolder(){

    string folderName;
    cout << "\nEnter folder name to enter: ";
    getline(cin, folderName);

    if(folderName.empty()){
        throw ExceptionHandling("Folder name cannot be empty.");
    }
    /*
    system prioritise entering the first folder with input name
    when there exist subfolders with the same name
    */
    if(current->folderSearch(folderName) == nullptr){
        throw ExceptionHandling("Folder '" + folderName + "' does not exist");
    };
    current = current->folderSearch(folderName);    
}

void FileSystem::goBack(){

    if(current->getParent() == nullptr){
        throw ExceptionHandling("Already at Root. Cannot go further back.");
    }
    current = current->getParent();
    cout << "\nMoved back to: " << current->getFolderName() << "\n";
}

void FileSystem::renameFile(){
    if(current->getFileCount() == 0){
        cout << endl
             << "No file to rename in current folder" << endl;
        return;
    }

    //display file names and index
    current->printFileList();
    
    //let user pick the index
    int choice;
    cout << endl;
    cout << "Please select the file number you wish to rename: ";
    cin >> choice; cin.ignore();
    
    //input viladation: ensure choice within range of file couunt
    while(true){
        try{
            if(choice > current->getFileCount() || choice < 1){
                throw InvalidRange("Invalid choice, please choose within range: ");
            }
            break;
        }
        catch(const InvalidRange& error){
            cout << error.getErrorMsg();
            cout << "1 to " << current->getFileCount() << endl;
            cout << "Input: ";
            cin >> choice; cin.ignore();
        }
    }
    
    choice--; //adjust choice to align with 0-based indexing
    
    
    //rename the file object
    //use setFileName() & setFileExt()
    cout << endl;
    cout << "Please rename your file: (original file name: " << current->getFile(choice).getFileFullName() << ")";
    cout << endl;
    
    //accept input from user for new name and extension
    string fileName;
    getline(cin, fileName);

    ////check 1: empty name
    if(fileName.empty())
        throw ExceptionHandling("File name cannot be empty.");
    
    //check 2: blank spaces
    if(fileName == " " || fileName == "\t"){
        throw ExceptionHandling("Folder name can not only contain black spaces.");
    }

    //check 3: no extension
    int dotPosition = fileName.rfind('.');
    if(dotPosition >= fileName.size() - 1){
        throw ExceptionHandling("File does not have an extension.");
    }
    
    //check 4: duplicate full name
    File* duplicateFile = current->fileSearch(fileName);
    if(duplicateFile != nullptr){
        throw ExceptionHandling("File '" + fileName + "' already exists here.");
    }

    //passed all tests, safe to rename
    current->getFile(choice).setFileName(fileName.substr(0, dotPosition));
    current->getFile(choice).setFileExt(fileName.substr(dotPosition + 1));
    cout << endl
         << "File renamed successfully to " << current->getFile(choice).getFileFullName() << endl;
}

void FileSystem::renameFolder(){
    //check if there is any subfolder to rename
    if(current->getSubfolderCount() == 0){
        cout << endl
             << "No subfolder to rename in current folder" << endl;
        return;
    }

    //display folder names and index
    current->printSubfolderList();
    
    //let user pick the index
    int choice;
    cout << endl;
    cout << "Please select the folder you wish to rename: ";
    cin >> choice;
    cin.ignore();
    
    //input viladation: ensure choice within range of file couunt
    while(true){
        try{
            if(choice > current->getSubfolderCount() || choice < 1){
                throw InvalidRange("Invalid choice, please choose within range: ");
            }
            break;
        }
        catch(const InvalidRange& error){
            cout << error.getErrorMsg();
            cout << "1 to " << current->getSubfolderCount() << endl;
            cout << "Input: ";
            cin >> choice; cin.ignore();
        }
    }
    
    choice--; //adjust choice to align with 0-based indexing

    //rename the subfolder object
    //use setFolderName()
    cout << endl;
    cout << "Please rename your subfolder: (original subfolder name: " << current->getSubfolder(choice)->getFolderName() << ")";
    cout << endl;
    
    //accept input from user for new subbfolder name
    string folderName;
    cout << "\nEnter folder name: ";
    getline(cin, folderName);

    //check 1: empty name
    if(folderName.empty())
        throw ExceptionHandling("Folder name cannot be empty.");

    //check 2: duplicate name
    Folder* duplicateFolder = current->folderSearch(folderName);
    if(duplicateFolder != nullptr){
        throw ExceptionHandling("Folder '" + folderName + "' already exists.");
    }

    //check 3: blank spaces
    if(folderName == " " || folderName == "\t"){
        throw ExceptionHandling("Folder name can not only contain black spaces.");
    }

    //passed both checks, safe to rename
    current->getSubfolder(choice)->setFolderName(folderName);
    cout << endl
         << "Folder renamed successfully to " << current->getSubfolder(choice)->getFolderName() << endl;
}

void FileSystem::removeFile(){
    //check if there is any file to remove
    if(current->getFileCount() == 0){
        cout << endl
             << "No file to delete in current folder" << endl;
        return;
    }

    //display file names
    current->printFileList();
    
    //receive file name from user to delete
    string fileName;
    cout << endl;
    cout << "Please type the name and extension of the file you wish to delete: ";
    getline(cin, fileName);

    //checks if deletefolder() is able to delete the folder
    if(!root->deleteFile(fileName)){
        throw ExceptionHandling("File is not found, please try again.");
    }
    cout << "\nFile '" << fileName << "' deleted successfully.\n";
}

void FileSystem::removeFolder(){
    //check if there is any folder to remove
    if(current->getSubfolderCount() == 0){
        cout << endl
             << "No subfolder to delete in current folder" << endl;
        return;
    }

    //display subfolder names
    current->printSubfolderList();
    
    //receive subfolder name from user to delete
    string subfolderName;
    cout << endl;
    cout << "Please type the name of the folder you wish to delete: ";
    getline(cin, subfolderName);

    //checks if deletefolder() is able to delete the folder
        if(!root->deleteFolder(subfolderName)){
            throw ExceptionHandling("Folder is not found, please try again.");
        }
        cout << "\nFolder '" << subfolderName << "' and all its contents deleted successfully.\n";
}

void FileSystem::showCurrentPath(){
    cout << "\nCurrent Path: " << current->folderTraverse() << "\n";
}
