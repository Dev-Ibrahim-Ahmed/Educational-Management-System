#ifndef FUNCS_H
#define FUNCS_H

#include "models.h"

bool is_valid_email(const std::string& email);
void seed_dummy_data();

void main_menu();
void sign_in();
void sign_up();

void student_dashboard(const User& student);
void doctor_dashboard(const User& doctor);

void doctor_add_course(const User& doctor);
void doctor_list_courses(const User& doctor);
void doctor_grade_students(const User& doctor);

void student_register_course(const User& student);
void student_view_courses_and_grades(const User& student);

#endif