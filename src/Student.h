
#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
public:
    std::string name;
    int gatorId;
    std::string gatorIdStr;

    Student(std::string name , int gatorId , std::string gatorIdStr);
};

#endif