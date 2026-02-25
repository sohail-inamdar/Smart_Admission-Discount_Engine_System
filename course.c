#include <stdio.h>
#include "course.h"

Course courses[50];
int courseCount = 0;


void saveCourses() {

    FILE *fp = fopen("courses.dat", "wb");
    if(fp == NULL) return;

    fwrite(&courseCount, sizeof(int), 1, fp);
    fwrite(courses, sizeof(Course), courseCount, fp);

    fclose(fp);
}

void loadCourses() {

    FILE *fp = fopen("courses.dat", "rb");
    if(fp == NULL) return;

    fread(&courseCount, sizeof(int), 1, fp);
    fread(courses, sizeof(Course), courseCount, fp);

    fclose(fp);
}

void addCourse() {
    Course c;
    c.courseId = courseCount + 1;

    printf("Enter Course Name: ");
    scanf(" %[^\n]", c.courseName);

    courses[courseCount++] = c;
    printf("Course Added Successfully!\n");
    
}

void viewCourses() {
    if(courseCount == 0) {
        printf("No courses available.\n");
        return;
    }

    for(int i=0;i<courseCount;i++) {
        printf("%d - %s\n",
               courses[i].courseId,
               courses[i].courseName);
    }
}