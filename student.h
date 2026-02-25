#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    int id;
    char name[50];
    char email[50];
    char phone[15];
} Student;

void addStudent();
void viewStudents();

void saveStudents();
void loadStudents();

#endif