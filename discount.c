#include <stdio.h>
#include <string.h>
#include <time.h>
#include "discount.h"
#include "registration.h"
#include "batch.h"



Discount discounts[50];
int discountCount = 0;

char* getCategoryName(int category) {

    switch(category) {
        case 1: return "Early Bird";
        case 2: return "Loyalty";
        case 3: return "Individual";
        case 4: return "Combo";
        case 5: return "Group";
        default: return "Unknown";
    }
}

void addDiscount() {

    Discount d;
    d.discountId = discountCount + 1;

    printf("\n===== Add New Discount =====\n");

    printf("Enter Discount Name: ");
    scanf(" %[^\n]", d.discountName);

    printf("\nSelect Discount Category:\n");
    printf("1 = Early Bird\n");
    printf("2 = Loyalty\n");
    printf("3 = Individual\n");
    printf("4 = Combo\n");
    printf("5 = Group\n");
    scanf("%d", &d.discountCategory);

    // Category-specific inputs
    if(d.discountCategory == 1) {
        printf("Start Date (YYYYMMDD): ");
        scanf("%s", d.startDate);
        printf("End Date (YYYYMMDD): ");
        scanf("%s", d.endDate);
    }

    else if(d.discountCategory == 2) {
        printf("Minimum Completed Courses Required: ");
        scanf("%d", &d.minCoursesCompleted);
    }

    else if(d.discountCategory == 3) {
        printf("Applicable Student ID: ");
        scanf("%d", &d.studentId);
    }

    else if(d.discountCategory == 4) {
        printf("Enter First Batch ID: ");
        scanf("%d", &d.comboBatch1);
        printf("Enter Second Batch ID: ");
        scanf("%d", &d.comboBatch2);
    }

    else if(d.discountCategory == 5) {
        printf("Minimum Group Size Required: ");
        scanf("%d", &d.minGroupSize);
    }

    printf("\nSelect Discount Type:\n");
    printf("1 = Flat Amount\n");
    printf("2 = Percentage\n");
    scanf("%d", &d.discountType);

    printf("Enter Discount Value: ");
    scanf("%f", &d.discountValue);

    discounts[discountCount++] = d;

    printf("Discount Added Successfully!\n");
}

void viewDiscounts() {

    for(int i=0;i<discountCount;i++) {

        printf("%d - %s | %s | ",
               discounts[i].discountId,
               discounts[i].discountName,
               getCategoryName(discounts[i].discountCategory));

        if(discounts[i].discountType == 1)
            printf("Flat %.2f\n", discounts[i].discountValue);
        else
            printf("Percent %.2f%%\n", discounts[i].discountValue);
    }
}

int isEligible(Discount d, int studentId, int batchId) {

    // REAL SYSTEM DATE
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char today[15];
    sprintf(today, "%04d%02d%02d",
            tm.tm_year + 1900,
            tm.tm_mon + 1,
            tm.tm_mday);

    // Early Bird
    if(d.discountCategory == 1) {

    // Get today's date
    time_t t = time(NULL);
    struct tm tm_today = *localtime(&t);

    int todayYear  = tm_today.tm_year + 1900;
    int todayMonth = tm_today.tm_mon + 1;
    int todayDay   = tm_today.tm_mday;

    // Find selected batch
    char batchDate[15];

    for(int i=0; i<batchCount; i++) {
        if(batches[i].batchId == batchId) {
            strcpy(batchDate, batches[i].startDate);
            break;
        }
    }

    // Parse batch date
    int batchYear, batchMonth, batchDay;
    sscanf(batchDate, "%4d%2d%2d",
           &batchYear, &batchMonth, &batchDay);

    struct tm tm_batch = {0};
    tm_batch.tm_year = batchYear - 1900;
    tm_batch.tm_mon  = batchMonth - 1;
    tm_batch.tm_mday = batchDay;

    time_t time_today = mktime(&tm_today);
    time_t time_batch = mktime(&tm_batch);

    double diffSeconds = difftime(time_batch, time_today);
    int diffDays = diffSeconds / (60 * 60 * 24);

    if(diffDays >= 10)
        return 1;
    else
        return 0;
}

    // Individual
    if(d.discountCategory == 3) {
        if(studentId == d.studentId)
            return 1;
        else
            return 0;
    }

    // Group Discount (REAL GROUP COUNT)
    if(d.discountCategory == 5) {

        int groupSize = 0;

        for(int i=0;i<registrationCount;i++) {
            if(registrations[i].batchId == batchId)
                groupSize++;
        }

        if(groupSize + 1 >= d.minGroupSize)
            return 1;
        else
            return 0;
    }

    return 1;
}

float applyDiscount(float fee, Discount d) {

    float finalAmount = fee;

    if(d.discountType == 1)
        finalAmount = fee - d.discountValue;

    else if(d.discountType == 2)
        finalAmount = fee - (fee * d.discountValue / 100);

    if(finalAmount < 0)
        finalAmount = 0;

    return finalAmount;
}