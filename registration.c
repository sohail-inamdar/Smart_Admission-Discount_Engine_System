#include <stdio.h>
#include <string.h>
#include "student.h"
#include "registration.h"
#include "batch.h"
#include "course.h"
#include "discount.h"


Registration registrations[200];
int registrationCount = 0;
int regCounter = 1000;


void saveRegistrations() {

    FILE *fp = fopen("registrations.dat", "wb");
    if(fp == NULL) return;

    fwrite(&registrationCount, sizeof(int), 1, fp);
    fwrite(registrations, sizeof(Registration), registrationCount, fp);

    fclose(fp);
}

void loadRegistrations() {

    FILE *fp = fopen("registrations.dat", "rb");
    if(fp == NULL) return;

    fread(&registrationCount, sizeof(int), 1, fp);
    fread(registrations, sizeof(Registration), registrationCount, fp);

    fclose(fp);
}

void registerStudent() {

    int studentId;
    int batchId;
    int discountId;
    float fee = 0;
    float finalAmount;

    printf("Enter Student ID: ");
    scanf("%d", &studentId);


    int validStudent = 0;

    for(int i = 0; i < studentCount; i++) {
        if(students[i].id == studentId) {
            validStudent = 1;
            break;
        }
    }

    if(!validStudent) {
        printf("Invalid Student ID! Please add student first.\n");
        return;
    }

    printf("\nAvailable Batches:\n");
    viewBatches();

    printf("Enter Batch ID: ");
    scanf("%d", &batchId);

    for(int i=0;i<batchCount;i++) {
        if(batches[i].batchId == batchId)
            fee = batches[i].fee;
    }

    finalAmount = fee;

    //printf("\nEligible Discounts:\n");

   int found = 0;

for(int i = 0; i < discountCount; i++) {

    if(isEligible(discounts[i], studentId, batchId)) {

        if(found == 0) {
            printf("\nEligible Discounts:\n");
        }

        printf("Discount ID=%d | Name=%s | Batch=%d\n",
               discounts[i].discountId,
               discounts[i].discountName,
               discounts[i].applicableBatchId);

        found = 1;
    }
}

if(found == 0) {
    printf("\nNo Eligible Discounts for this batch.\n");
}
    
    printf("Select Discount ID (0 for none): ");
    scanf("%d", &discountId);

    int appliedDiscount = 0;

    if(discountId > 0) {
        for(int i=0;i<discountCount;i++) {
            if(discounts[i].discountId == discountId &&
               isEligible(discounts[i], studentId, batchId)) {

                finalAmount = applyDiscount(fee, discounts[i]);
                appliedDiscount = discountId;
            }
        }
    }

    Registration r;
    r.registrationId = regCounter++;
    r.studentId = studentId;
    r.batchId = batchId;
    r.originalFee = fee;
    r.finalAmount = finalAmount;
    r.appliedDiscountId = appliedDiscount;

    registrations[registrationCount++] = r;

    printf("Final Payable Amount: %.2f\n", finalAmount);
    printf("Registration ID: %d\n", r.registrationId);
}

void showReports() {

    printf("\n===== ADMIN REPORT =====\n");

    printf("Total Registrations: %d\n", registrationCount);

    float totalRevenue = 0;
    float discountRevenue[50] = {0};

    for(int i=0;i<registrationCount;i++) {
        totalRevenue += registrations[i].finalAmount;

        for(int j=0;j<discountCount;j++) {
            if(registrations[i].appliedDiscountId ==
               discounts[j].discountId) {

                discountRevenue[j] += registrations[i].finalAmount;
            }
        }
    }

    printf("\nTotal Revenue: %.2f\n", totalRevenue);

    printf("\nRevenue Per Discount Type:\n");

    for(int i=0;i<discountCount;i++) {
        printf("%s : %.2f\n",
               discounts[i].discountName,
               discountRevenue[i]);
    }
}

void showAllStudentDetails() {

    if(registrationCount == 0) {
        printf("\nNo registrations available.\n");
        return;
    }

    printf("\n===== STUDENT REGISTRATION DETAILS =====\n");

    for(int i = 0; i < registrationCount; i++) {

        Registration r = registrations[i];

        // 1️⃣ Find Student
        Student s;
        for(int j = 0; j < studentCount; j++) {
            if(students[j].id == r.studentId) {
                s = students[j];
                break;
            }
        }

        // 2️⃣ Find Batch
        Batch b;
        for(int j = 0; j < batchCount; j++) {
            if(batches[j].batchId == r.batchId) {
                b = batches[j];
                break;
            }
        }

        // 3️⃣ Find Course
        Course c;
        for(int j = 0; j < courseCount; j++) {
            if(courses[j].courseId == b.courseId) {
                c = courses[j];
                break;
            }
        }

        // 4️⃣ Find Discount
        char discountName[50] = "No Discount";

        for(int j = 0; j < discountCount; j++) {
            if(discounts[j].discountId == r.appliedDiscountId) {
                strcpy(discountName, discounts[j].discountName);
                break;
            }
        }

        printf("\n---------------------------------\n");
        printf("Student ID: %d\n", s.id);
        printf("Name      : %s\n", s.name);
        printf("Email     : %s\n", s.email);
        printf("Mobile    : %s\n", s.phone);
        printf("Course    : %s\n", c.courseName);
        printf("Batch ID  : %d\n", b.batchId);
        printf("Final Fee : %.2f\n", r.finalAmount);
        printf("Discount  : %s\n", discountName);
    }
}