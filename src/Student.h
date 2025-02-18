#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
public:
    // student class a student has a name as a string and ufid number as an int 
    std::string name;
    int ufId;

    Student(std::string name , int ufId);
};

#endif