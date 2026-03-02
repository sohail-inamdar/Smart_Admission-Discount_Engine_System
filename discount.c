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


void saveDiscounts() {

    FILE *fp = fopen("discounts.dat", "wb");
    if(fp == NULL) return;

    fwrite(&discountCount, sizeof(int), 1, fp);
    fwrite(discounts, sizeof(Discount), discountCount, fp);

    fclose(fp);
}

void loadDiscounts() {

    FILE *fp = fopen("discounts.dat", "rb");
    if(fp == NULL) return;

    fread(&discountCount, sizeof(int), 1, fp);
    fread(discounts, sizeof(Discount), discountCount, fp);

    fclose(fp);
}


void viewDiscountsBatchWise() {

    if(batchCount == 0) {
        printf("No batches available.\n");
        return;
    }

    printf("\nAvailable Batches:\n");
    viewBatches();

    int batchId;
    printf("Enter Batch ID to view discounts: ");
    scanf("%d", &batchId);

    int found = 0;

    printf("\nDiscounts for Batch %d:\n", batchId);

    for(int i = 0; i < discountCount; i++) {

        if(discounts[i].applicableBatchId == batchId) {

            printf("ID: %d | %s | %s | ",
                   discounts[i].discountId,
                   discounts[i].discountName,
                   getCategoryName(discounts[i].discountCategory));

            if(discounts[i].discountType == 1)
                printf("Flat %.2f\n", discounts[i].discountValue);
            else
                printf("Percent %.2f%%\n", discounts[i].discountValue);

            found = 1;
        }
    }

    if(!found)
        printf("No discounts available for this batch.\n");
}


void addDiscount() {

    Discount d;
    d.discountId = discountCount + 1;

    printf("\n===== Add New Discount =====\n");

    printf("\nAvailable Batches:\n");
    viewBatches();

    printf("Enter Batch ID for which this discount is applicable: ");
    scanf("%d", &d.applicableBatchId);


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
        printf("Minimum Completed Batches Required: ");
        scanf("%d", &d.minBatchesCompleted);
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

        printf("%d - %s | Batch ID: %d | %s\n",
       discounts[i].discountId,
       discounts[i].discountName,
       discounts[i].applicableBatchId,
       getCategoryName(discounts[i].discountCategory));

        if(discounts[i].discountType == 1)
            printf("Flat %.2f\n", discounts[i].discountValue);
        else
            printf("Percent %.2f%%\n", discounts[i].discountValue);
    }
}

int isEligible(Discount d, int studentId, int batchId) {

    // 1️⃣ Batch Match First
    if(d.applicableBatchId != batchId)
        return 0;

    // 2️⃣ Early Bird
    if(d.discountCategory == 1) {

        time_t t = time(NULL);
        struct tm tm_today = *localtime(&t);

        tm_today.tm_hour = 0;
        tm_today.tm_min = 0;
        tm_today.tm_sec = 0;

        time_t time_today = mktime(&tm_today);

        char batchDate[15];
        int found = 0;

        for(int i=0; i<batchCount; i++) {
            if(batches[i].batchId == batchId) {
                strcpy(batchDate, batches[i].startDate);
                found = 1;
                break;
            }
        }

        if(!found)
            return 0;

        int batchYear, batchMonth, batchDay;
        sscanf(batchDate, "%2d%2d%4d", &batchDay, &batchMonth, &batchYear);

        struct tm tm_batch = {0};
        tm_batch.tm_year = batchYear - 1900;
        tm_batch.tm_mon  = batchMonth - 1;
        tm_batch.tm_mday = batchDay;
        tm_batch.tm_hour = 0;
        tm_batch.tm_min  = 0;
        tm_batch.tm_sec  = 0;

        time_t time_batch = mktime(&tm_batch);

        double diffSeconds = difftime(time_batch, time_today);
        int diffDays = diffSeconds / (60 * 60 * 24);

        if(diffDays >= 10)
            return 1;
        else
            return 0;
    }

    //loyalty Discount

    // Loyalty Discount
if(d.discountCategory == 2) {

    int completedBatches = 0;

    for(int i = 0; i < registrationCount; i++) {
        if(registrations[i].studentId == studentId)
            completedBatches++;
    }

    if(completedBatches >= d.minBatchesCompleted)
        return 1;
    else
        return 0;
}

    // 3️⃣ Individual
    if(d.discountCategory == 3) {
        if(studentId == d.studentId)
            return 1;
        else
            return 0;
    }


    // Combo Discount
    if(d.discountCategory == 4) {

        int batch1Done = 0;
        int batch2Done = 0;

        for(int i=0;i<registrationCount;i++) {

            if(registrations[i].studentId == studentId) {

                if(registrations[i].batchId == d.comboBatch1)
                    batch1Done = 1;

                if(registrations[i].batchId == d.comboBatch2)
                    batch2Done = 1;
            }
        }

        if(batch1Done && batch2Done)
            return 1;
        else
            return 0;
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