#include <stdio.h>
#include <string.h>
#include "batch.h"
#include "course.h"

Batch batches[100];
int batchCount = 0;

void addBatch() {

    if(courseCount == 0) {
        printf("No courses available. Add course first.\n");
        return;
    }

    Batch b;
    b.batchId = batchCount + 1;

    printf("\nAvailable Courses:\n");
    viewCourses();

    printf("Enter Course ID: ");
    scanf("%d", &b.courseId);

    printf("Enter Fee: ");
    scanf("%f", &b.fee);

    printf("Enter Capacity: ");
    scanf("%d", &b.capacity);

    printf("Enter Mode (Online/Offline): ");
    scanf("%s", b.mode);

    printf("Enter Batch Start Date (YYYYMMDD): ");
    scanf("%s", b.startDate);

    batches[batchCount++] = b;

    printf("Batch Added Successfully!\n");
}

void viewBatches() {

    if(batchCount == 0) {
        printf("No batches available.\n");
        return;
    }

    printf("\n===== Available Batches =====\n");

    for(int i=0;i<batchCount;i++) {

        char courseName[50] = "Unknown";

        for(int j=0;j<courseCount;j++) {
            if(courses[j].courseId == batches[i].courseId) {
                strcpy(courseName, courses[j].courseName);
                break;
            }
        }

        printf("Batch ID: %d\n", batches[i].batchId);
        printf("Course  : %s\n", courseName);
        printf("Fee     : %.2f\n", batches[i].fee);
        printf("Mode    : %s\n", batches[i].mode);
        printf("Capacity: %d\n", batches[i].capacity);
        printf("-------------------------\n");
    }
}