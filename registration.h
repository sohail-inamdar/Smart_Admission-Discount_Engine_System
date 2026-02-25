#ifndef REGISTRATION_H
#define REGISTRATION_H

typedef struct {
    int registrationId;
    int studentId;
    int batchId;
    float originalFee;
    float finalAmount;
    int appliedDiscountId;
} Registration;

extern Registration registrations[200];
extern int registrationCount;

void showAllStudentDetails();

void saveRegistrations();
void loadRegistrations();

void registerStudent();
void showReports();

#endif