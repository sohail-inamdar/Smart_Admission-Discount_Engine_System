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

    if(registrationCount > 0)
        regCounter = registrations[registrationCount - 1].registrationId + 1;
}

void registerStudent() {

    int studentId;
    int batchId;
    int discountId = 0;
    float fee = 0;
    float finalAmount;

    printf("Enter Student ID: ");
    scanf("%d", &studentId);

    // 1️⃣ Validate Student
    int validStudent = 0;
    for(int i = 0; i < studentCount; i++) {
        if(students[i].id == studentId) {
            validStudent = 1;
            break;
        }
    }

    if(!validStudent) {
        printf("Invalid Student ID!\n");
        return;
    }

    // 2️⃣ Show Batches
    printf("\nAvailable Batches:\n");
    viewBatches();

    printf("Enter Batch ID: ");
    scanf("%d", &batchId);

    // 3️⃣ Validate Batch + Get Fee
    int batchFound = 0;

    for(int i=0;i<batchCount;i++) {
        if(batches[i].batchId == batchId) {
            fee = batches[i].fee;
            batchFound = 1;
            break;
        }
    }

    if(!batchFound) {
        printf("Invalid Batch ID!\n");
        return;
    }

    finalAmount = fee;

    // 4️⃣ Prevent Duplicate Registration
    for(int i=0; i<registrationCount; i++) {
        if(registrations[i].studentId == studentId &&
           registrations[i].batchId == batchId) {

            printf("Student already registered in this batch!\n");
            return;
        }
    }

    // 5️⃣ Seat Availability Check
    int seatsFilled = 0;

    for(int i=0; i<registrationCount; i++) {
        if(registrations[i].batchId == batchId)
            seatsFilled++;
    }

    for(int i=0; i<batchCount; i++) {
        if(batches[i].batchId == batchId) {

            if(seatsFilled >= batches[i].capacity) {
                printf("Seats Full! Cannot Register.\n");
                return;
            }
            break;
        }
    }

    // 6️⃣ Show Eligible Discounts
    int found = 0;

    for(int i = 0; i < discountCount; i++) {

        if(isEligible(discounts[i], studentId, batchId)) {

            if(found == 0) {
                printf("\nEligible Discounts:\n");
            }

            printf("Discount ID=%d | Name=%s\n",
                   discounts[i].discountId,
                   discounts[i].discountName);

            found = 1;
        }
    }

    if(found == 0) {
        printf("\nNo Eligible Discounts for this batch.\n");
    }

    // 7️⃣ Select Discount
    printf("Select Discount ID (0 for none): ");
    scanf("%d", &discountId);

    // 🔥 Check if selected discount is Group Discount
    for(int i = 0; i < discountCount; i++) {

    if(discounts[i].discountId == discountId) {

        if(discounts[i].discountCategory == 5) {

            registerGroup(batchId, discounts[i]);
            return;   // stop normal registration flow
            }
        }
    }

    int appliedDiscount = 0;

    if(discountId > 0) {

        int validDiscount = 0;

        for(int i=0;i<discountCount;i++) {
            if(discounts[i].discountId == discountId &&
               isEligible(discounts[i], studentId, batchId)) {

                finalAmount = applyDiscount(fee, discounts[i]);
                appliedDiscount = discountId;
                validDiscount = 1;
                break;
            }
        }

        if(!validDiscount) {
            printf("Invalid or Not Eligible Discount.\n");
        }
    }

    printf("Original Fee: %.2f\n", fee);
    printf("Final Payable Amount: %.2f\n", finalAmount);

    // 8️⃣ Confirm Registration
    char confirm;
    printf("\nConfirm Registration? (Y/N): ");
    scanf(" %c", &confirm);

    if(confirm != 'Y' && confirm != 'y') {
        printf("Registration Cancelled.\n");
        return;
    }

    // 9️⃣ Save Registration
    Registration r;
    r.registrationId = regCounter++;
    r.studentId = studentId;
    r.batchId = batchId;
    r.originalFee = fee;
    r.finalAmount = finalAmount;
    r.appliedDiscountId = appliedDiscount;

    registrations[registrationCount++] = r;

    printf("Registration Successful!\n");
    printf("Registration ID: %d\n", r.registrationId);
}
void showReports() {

    printf("\n===== ADMIN REPORT =====\n");

    printf("Total Registrations: %d\n", registrationCount);

    float totalRevenue = 0;

    // Revenue per batch
    float batchRevenue[100] = {0};

    for(int i = 0; i < registrationCount; i++) {
        totalRevenue += registrations[i].finalAmount;

        for(int j = 0; j < batchCount; j++) {
            if(registrations[i].batchId == batches[j].batchId) {
                batchRevenue[j] += registrations[i].finalAmount;
            }
        }
    }

    printf("\nTotal Revenue: %.2f\n", totalRevenue);

    printf("\nRevenue Per Batch:\n");

    for(int i = 0; i < batchCount; i++) {

        // Find course name
        char courseName[50] = "Unknown";

        for(int j = 0; j < courseCount; j++) {
            if(courses[j].courseId == batches[i].courseId) {
                strcpy(courseName, courses[j].courseName);
                break;
            }
        }

        printf("Batch ID: %d | Course: %s | Revenue: %.2f\n",
               batches[i].batchId,
               courseName,
               batchRevenue[i]);
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

void searchStudentByName() {

    char searchName[50];
    int found = 0;

    printf("Enter Student Name to Search: ");
    scanf(" %[^\n]", searchName);

    for(int i=0; i<registrationCount; i++) {

        for(int j=0; j<studentCount; j++) {

            if(registrations[i].studentId == students[j].id &&
               strstr(students[j].name, searchName) != NULL) {

                printf("\nStudent ID: %d\n", students[j].id);
                printf("Name      : %s\n", students[j].name);
                printf("Email     : %s\n", students[j].email);
                printf("Mobile    : %s\n", students[j].phone);
                printf("Batch ID  : %d\n", registrations[i].batchId);
                printf("Final Fee : %.2f\n", registrations[i].finalAmount);
                printf("-------------------------\n");

                found = 1;
            }
        }
    }

    if(!found)
        printf("No student found with this name.\n");
}


void registerGroup(int batchId, Discount d) {

    int groupSize;

    printf("Enter number of students in group: ");
    scanf("%d", &groupSize);

    if(groupSize < d.minGroupSize) {
        printf("Group size less than minimum required (%d)!\n",
               d.minGroupSize);
        return;
    }

    // 🔥 Calculate remaining seats
    int seatsFilled = 0;

    for(int i = 0; i < registrationCount; i++) {
        if(registrations[i].batchId == batchId)
            seatsFilled++;
    }

    int remainingSeats = 0;
    float fee = 0;

    for(int i = 0; i < batchCount; i++) {
        if(batches[i].batchId == batchId) {
            remainingSeats = batches[i].capacity - seatsFilled;
            fee = batches[i].fee;
            break;
        }
    }

    if(groupSize > remainingSeats) {
        printf("Not enough seats available!\n");
        printf("Remaining Seats: %d\n", remainingSeats);
        return;
    }

    // 🔥 Calculate discount per student (percentage based)
    float discountPerStudent;

    if(d.discountType == 2)
        discountPerStudent = fee * d.discountValue / 100;
    else
        discountPerStudent = d.discountValue;

    float finalPerStudent = fee - discountPerStudent;

    if(finalPerStudent < 0)
        finalPerStudent = 0;

    float totalOriginal = fee * groupSize;
    float totalFinal = finalPerStudent * groupSize;

    printf("\n===== Group Summary =====\n");
    printf("Original Total Fee: %.2f\n", totalOriginal);
    printf("Discount Per Student: %.2f\n", discountPerStudent);
    printf("Total Payable Amount: %.2f\n", totalFinal);

    // 🔥 Temporary storage for students
    Student tempStudents[50];

    for(int i = 0; i < groupSize; i++) {

        tempStudents[i].id = studentCount + i + 1;

        printf("\nEnter Name for Student %d: ", i+1);
        scanf(" %[^\n]", tempStudents[i].name);

        printf("Enter Email: ");
        scanf("%s", tempStudents[i].email);

        printf("Enter Phone: ");
        scanf("%s", tempStudents[i].phone);
    }

    // 🔥 Confirmation before saving
    char confirm;
    printf("\nConfirm Group Registration? (Y/N): ");
    scanf(" %c", &confirm);

    if(confirm != 'Y' && confirm != 'y') {
        printf("Group Registration Cancelled.\n");
        return;
    }

    // 🔥 Save all students after confirmation
    for(int i = 0; i < groupSize; i++) {

        students[studentCount++] = tempStudents[i];

        Registration r;
        r.registrationId = regCounter++;
        r.studentId = tempStudents[i].id;
        r.batchId = batchId;
        r.originalFee = fee;
        r.finalAmount = finalPerStudent;
        r.appliedDiscountId = d.discountId;

        registrations[registrationCount++] = r;
    }

    printf("\nGroup Registration Successful!\n");
}