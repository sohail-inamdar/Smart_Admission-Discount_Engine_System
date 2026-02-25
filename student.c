#include <stdio.h>
#include "student.h"

Student students[100];
int studentCount = 0;

void saveStudents() {

    FILE *fp = fopen("students.dat", "wb");
    if(fp == NULL) return;

    fwrite(&studentCount, sizeof(int), 1, fp);
    fwrite(students, sizeof(Student), studentCount, fp);

    fclose(fp);
}

void loadStudents() {

    FILE *fp = fopen("students.dat", "rb");
    if(fp == NULL) return;

    fread(&studentCount, sizeof(int), 1, fp);
    fread(students, sizeof(Student), studentCount, fp);

    fclose(fp);
}

void addStudent() {
    Student s;
    s.id = studentCount + 1;

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);

    printf("Enter Email: ");
    scanf("%s", s.email);

    printf("Enter Phone: ");
    scanf("%s", s.phone);

    students[studentCount++] = s;
    printf("Student Registered Successfully!\n");
    printf("Student Registered ID = %d\n",s.id);

}

void viewStudents() {
    for(int i = 0; i < studentCount; i++) {
        printf("%d - %s - %s\n",
               students[i].id,
               students[i].name,
               students[i].email);
    }
}