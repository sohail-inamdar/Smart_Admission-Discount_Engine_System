#include <stdio.h>
#include<string.h>
#include "admin.h"
#include "student.h"
#include "course.h"
#include "batch.h"
#include "discount.h"
#include "registration.h"

void adminMenu() {
    int choice;

    while(1) {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Course\n");
        printf("2. Add Batch\n");
        printf("3. Add Discount\n");
        printf("4. View Reports\n");
        printf("5. Back\n");

        scanf("%d", &choice);

        switch(choice) {
            case 1: addCourse(); break;
            case 2: addBatch(); break;
            case 3: addDiscount(); break;
            case 4: showReports(); break;
            case 5: return;
            default: printf("Invalid Choice\n");
        }
    }
}

void studentMenu() {
    int choice;

    while(1) {
        printf("\n===== STUDENT MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Register For Batch\n");
        printf("3. Back\n");

        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(); break;
            case 2: registerStudent(); break;
            case 3: return;
            default: printf("Invalid Choice\n");
        }
    }
}

int main() {

    int choice;

    while(1) {
        printf("\n===== Smart Admission System =====\n");
        printf("1. Admin Login\n");
        printf("2. Student Panel\n");
        printf("3. Exit\n");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                if(adminLogin())
                    adminMenu();
                break;

            case 2:
                studentMenu();
                break;

            case 3:
                return 0;

            default:
                printf("Invalid Choice\n");
        }
    }

    return 0;
}