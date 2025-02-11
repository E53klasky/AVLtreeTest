
#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
public:
    std::string name;
    int gatorId;

    Student(std::string name , int id);
};

#endif