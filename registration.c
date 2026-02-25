#include <stdio.h>
#include "registration.h"
#include "batch.h"
#include "course.h"
#include "discount.h"

Registration registrations[200];
int registrationCount = 0;
int regCounter = 1000;

void registerStudent() {

    int studentId;
    int batchId;
    int discountId;
    float fee = 0;
    float finalAmount;

    printf("Enter Student ID: ");
    scanf("%d", &studentId);

    printf("\nAvailable Batches:\n");
    viewBatches();

    printf("Enter Batch ID: ");
    scanf("%d", &batchId);

    for(int i=0;i<batchCount;i++) {
        if(batches[i].batchId == batchId)
            fee = batches[i].fee;
    }

    finalAmount = fee;

    printf("\nEligible Discounts:\n");

    for(int i=0;i<discountCount;i++) {
        if(isEligible(discounts[i], studentId, batchId)) {
            printf("%d - %s\n",
                   discounts[i].discountId,
                   discounts[i].discountName);
        }
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