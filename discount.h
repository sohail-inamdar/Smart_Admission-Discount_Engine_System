#ifndef DISCOUNT_H
#define DISCOUNT_H

typedef struct {

    int discountId;
    char discountName[50];

    int discountType;     // 1=Flat, 2=Percentage
    float discountValue;

    int applicableBatchId;

    int discountCategory; // 1=EarlyBird
                          // 2=Loyalty
                          // 3=Individual
                          // 4=Combo
                          // 5=Group

    char startDate[15];
    char endDate[15];

    int studentId;
    int minCoursesCompleted;
    int minGroupSize;

    int comboBatch1;
    int comboBatch2;

} Discount;

extern Discount discounts[50];
extern int discountCount;

void addDiscount();
void viewDiscounts();
int isEligible(Discount d, int studentId, int batchId);
float applyDiscount(float fee, Discount d);
char* getCategoryName(int category);

#endif