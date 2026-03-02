#include <stdio.h>
#include <string.h>
#include "batch.h"
#include "course.h"
#include "registration.h"

Batch batches[100];
int batchCount = 0;

void saveBatches() {

    FILE *fp = fopen("batches.dat", "wb");
    if(fp == NULL) return;

    fwrite(&batchCount, sizeof(int), 1, fp);
    fwrite(batches, sizeof(Batch), batchCount, fp);

    fclose(fp);
}

void loadBatches() {

    FILE *fp = fopen("batches.dat", "rb");
    if(fp == NULL) return;

    fread(&batchCount, sizeof(int), 1, fp);
    fread(batches, sizeof(Batch), batchCount, fp);

    fclose(fp);
}

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

    printf("Enter Batch Start Date (DDMMYYYY): ");
    scanf("%s", b.startDate);

    printf("Enter Batch Start Date (DDMMYYYY): ");
    scanf("%s", b.endDate);
    
    batches[batchCount++] = b;

    printf("Batch Added Successfully!\n");
}

void viewBatches() {

    if(batchCount == 0) {
        printf("No batches available.\n");
        return;
    }

    printf("\n===== Available Batches =====\n");

    for(int i = 0; i < batchCount; i++) {

        char courseName[50] = "Unknown";

        // Find Course Name
        for(int j = 0; j < courseCount; j++) {
            if(courses[j].courseId == batches[i].courseId) {
                strcpy(courseName, courses[j].courseName);
                break;
            }
        }

        // 🔥 Count Seats Filled
        int seatsFilled = 0;

        for(int k = 0; k < registrationCount; k++) {
            if(registrations[k].batchId == batches[i].batchId)
                seatsFilled++;
        }

        int remainingSeats = batches[i].capacity - seatsFilled;

        if(remainingSeats < 0)
            remainingSeats = 0;

        printf("Batch ID   : %d\n", batches[i].batchId);
        printf("Course     : %s\n", courseName);
        printf("Fee        : %.2f\n", batches[i].fee);
        printf("Mode       : %s\n", batches[i].mode);
        printf("Start Date : %s\n", batches[i].startDate);
        printf("End Date   : %s\n", batches[i].endDate);
        printf("Capacity   : %d\n", batches[i].capacity);
        printf("Seats Filled    : %d\n", seatsFilled);
        printf("Remaining Seats : %d\n", remainingSeats);
        printf("-------------------------------\n");
    }
}