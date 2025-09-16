#pragma once
#include "define.h"

class Grade
{
private:
    float root2;
    float root1;
    float score1;
    float score2;
    float key;

public:
    Grade(const int &total);
    void insert(const int &type);
    int getGrade(void);
    void reset(const int &total);
};

