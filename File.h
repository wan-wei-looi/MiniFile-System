#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string>

using namespace std;

class File{
private:
    string name;
    string extension;
public:
    //constructor
    File(string n, string e): name(n), extension(e){}

    //setters
    void setFileName(string n){name = n;}
    void setFileExt(string e){extension = e;}

    //getters
    string getFileFullName()const{return (name + '.' + extension);}
};

#endif
