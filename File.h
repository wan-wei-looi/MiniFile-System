#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string>
class File{
private:
    std::string name;
    std::string extension;
public:
    //constructor
    File(std::string n, std::string e): name(n), extension(e){}

    //setters
    void setFileName(std::string n){name = n;}
    void setFileExt(std::string e){extension = e;}

    //getters
    std::string getFileFullName()const{return (name + '.' + extension);}
};

#endif
