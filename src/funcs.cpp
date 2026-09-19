#include "../headers/funcs.h"
#include <iostream>
#include <algorithm>

using namespace std;

map<string, User> system_users;
map<string, Course> system_courses;
map<string, map<string, StudentGrade>> student_enrollments;

bool is_valid_email(const string& email) {
    size_t at_pos = email.find('@');
    size_t dot_pos = email.rfind('.');
    return (at_pos != string::npos && 
            dot_pos != string::npos && 
            at_pos > 0 && 
            dot_pos > at_pos + 1 && 
            dot_pos < email.length() - 1);
}

void seed_dummy_data() {

    User doc1 = {"doc_smith", "pass123", "Dr. John Smith", "DOC101", "doctor", "smith@univ.edu"};
    system_users[doc1.username] = doc1;
    User stu1 = {"ibrahim_a", "30712142700654", "Ibrahim Ahmed", "STU501", "student", "ibrahim@student.edu"};
    system_users[stu1.username] = stu1;
    Course c1 = {"CS101", "Introduction to Programming", 100, "DOC101"};
    Course c2 = {"CS201", "Data Structures", 100, "DOC101"};
    system_courses[c1.code] = c1;
    system_courses[c2.code] = c2;
    student_enrollments["STU501"]["CS101"] = {88.5, true};
}

void main_menu() {
    while (true) {
        cout << "\n=== Educational Management System ===" << endl;
        cout << "1. Sign In\n2. Sign Up\n0. Exit\nChoice: ";
        char choice;
        cin >> choice;

        if (choice == '1') sign_in();
        else if (choice == '2') sign_up();
        else if (choice == '0') break;
        else cout << "Invalid choice!\n";
    }
}

void sign_in() {
    string username, password;
    cout << "\nEnter Username: ";
    cin >> username;

    if (!system_users.contains(username)) {
        cout << "User not found!\n";
        return;
    }

    User& user = system_users[username];
    for (int attempts = 3; attempts > 0; --attempts) {
        cout << "Enter Password: ";
        cin >> password;

        if (user.password == password) {
            cout << "\nWelcome, " << user.name << "!\n";
            if (user.role == "student") student_dashboard(user);
            else if (user.role == "doctor") doctor_dashboard(user);
            return;
        }
        cout << "Wrong password! Attempts left: " << attempts - 1 << endl;
    }
}

void sign_up() {
    User user;
    cout << "\nSelect Role (1 for Student, 2 for Doctor): ";
    char role_choice;
    cin >> role_choice;
    user.role = (role_choice == '2') ? "doctor" : "student";

    cout << "Enter Username: ";
    cin >> user.username;
    if (system_users.contains(user.username)) {
        cout << "Username already exists!\n";
        return;
    }

    cout << "Enter Password: ";
    cin >> user.password;
    cout << "Enter ID Number: ";
    cin >> user.id;
    cin.ignore();

    cout << "Enter Full Name: ";
    getline(cin, user.name);

    if (user.role == "student") {
        while (true) {
            cout << "Enter Valid Email: ";
            cin >> user.email;
            if (is_valid_email(user.email)) break;
            cout << "Invalid email format (example@domain.com). Try again.\n";
        }
    }

    system_users[user.username] = user;
    cout << "Registration successful! You can now sign in.\n";
}

void student_dashboard(const User& student) {
    while (true) {
        cout << "\n--- Student Dashboard --- (" << student.name << ")\n";
        cout << "1. View Enrolled Courses & Grade Report\n";
        cout << "2. Register for a Course\n";
        cout << "0. Log Out\nChoice: ";
        char choice;
        cin >> choice;

        if (choice == '1') student_view_courses_and_grades(student);
        else if (choice == '2') student_register_course(student);
        else if (choice == '0') break;
        else cout << "Invalid choice!\n";
    }
}

void student_register_course(const User& student) {
    cout << "\n--- Available Courses ---\n";
    if (system_courses.empty()) {
        cout << "No available courses at the moment.\n";
        return;
    }

    for (const auto& [code, course] : system_courses) {
        cout << "- " << course.code << " : " << course.name << " (Max Score: " << course.max_score << ")\n";
    }

    cout << "Enter Course Code to Register: ";
    string code;
    cin >> code;

    if (!system_courses.contains(code)) {
        cout << "Invalid Course Code!\n";
        return;
    }

    if (student_enrollments[student.id].contains(code)) {
        cout << "You are already registered in this course!\n";
        return;
    }

    student_enrollments[student.id][code] = {0.0, false};
    cout << "Successfully enrolled in " << system_courses[code].name << "!\n";
}

void student_view_courses_and_grades(const User& student) {
    cout << "\n--- Enrolled Courses & Grade Report ---\n";
    if (!student_enrollments.contains(student.id) || student_enrollments[student.id].empty()) {
        cout << "You are not enrolled in any courses.\n";
        return;
    }

    for (const auto& [code, grade] : student_enrollments[student.id]) {
        Course c = system_courses[code];
        cout << "Course: " << c.name << " [" << c.code << "]\n";
        cout << "Grade: ";
        if (grade.is_graded) cout << grade.score << " / " << c.max_score << "\n";
        else cout << "Not Evaluated Yet\n";
        cout << "-----------------------------------\n";
    }
}

void doctor_dashboard(const User& doctor) {
    while (true) {
        cout << "\n--- Doctor Dashboard --- (" << doctor.name << ")\n";
        cout << "1. Add New Course\n";
        cout << "2. List My Courses\n";
        cout << "3. Grade Enrolled Students\n";
        cout << "0. Log Out\nChoice: ";
        char choice;
        cin >> choice;

        if (choice == '1') doctor_add_course(doctor);
        else if (choice == '2') doctor_list_courses(doctor);
        else if (choice == '3') doctor_grade_students(doctor);
        else if (choice == '0') break;
        else cout << "Invalid choice!\n";
    }
}

void doctor_add_course(const User& doctor) {
    Course c;
    cout << "\nEnter Course Code (e.g. CS102): ";
    cin >> c.code;
    cin.ignore();
    cout << "Enter Course Name: ";
    getline(cin, c.name);
    cout << "Enter Max Score: ";
    cin >> c.max_score;
    c.doctor_id = doctor.id;

    system_courses[c.code] = c;
    cout << "Course created successfully!\n";
}

void doctor_list_courses(const User& doctor) {
    cout << "\n--- Managed Courses ---\n";
    bool found = false;
    for (const auto& [code, course] : system_courses) {
        if (course.doctor_id == doctor.id) {
            cout << "- " << course.code << " : " << course.name << " (Max: " << course.max_score << ")\n";
            found = true;
        }
    }
    if (!found) cout << "No courses added yet.\n";
}

void doctor_grade_students(const User& doctor) {
    cout << "Enter Course Code to view enrolled students: ";
    string code;
    cin >> code;

    if (!system_courses.contains(code) || system_courses[code].doctor_id != doctor.id) {
        cout << "Course not found or you don't instruct this course!\n";
        return;
    }

    cout << "\n--- Enrolled Students ---\n";
    vector<string> enrolled_student_ids;
    for (auto& [st_id, courses_map] : student_enrollments) {
        if (courses_map.contains(code)) {
            enrolled_student_ids.push_back(st_id);
            cout << "Student ID: " << st_id;
            if (courses_map[code].is_graded) {
                cout << " | Grade: " << courses_map[code].score;
            } else {
                cout << " | Grade: Ungraded";
            }
            cout << "\n";
        }
    }

    if (enrolled_student_ids.empty()) {
        cout << "No students currently enrolled in this course.\n";
        return;
    }

    cout << "Enter Student ID to grade: ";
    string target_id;
    cin >> target_id;

    if (student_enrollments[target_id].contains(code)) {
        cout << "Enter Marks (Max " << system_courses[code].max_score << "): ";
        double score;
        cin >> score;
        student_enrollments[target_id][code] = {score, true};
        cout << "Grade saved successfully!\n";
    } else {
        cout << "Student ID not found in this course!\n";
    }
}