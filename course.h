#ifndef COURSE_H
#define COURSE_H

typedef struct {
    int courseId;
    char courseName[50];
} Course;

extern Course courses[50];
extern int courseCount;

void saveCourses();
void loadCourses();

void addCourse();
void viewCourses();



#endif
