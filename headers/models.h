#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <map>

struct User {
    std::string username;
    std::string password;
    std::string name;
    std::string id;
    std::string role;
    std::string email;
};

struct Course {
    std::string code;
    std::string name;
    int max_score;
    std::string doctor_id;
};

struct StudentGrade {
    double score = 0.0;
    bool is_graded = false;
};

#endif