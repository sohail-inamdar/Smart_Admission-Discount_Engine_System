#include <stdio.h>
#include <string.h>
#include "admin.h"

int adminLogin() {

    char username[20];
    char password[20];

    printf("\n--- Admin Login ---\n");

    printf("Enter Username: ");
    scanf("%s", username);

    printf("Enter Password: ");
    scanf("%s", password);

    if(strcmp(username, "admin") == 0 &&
       strcmp(password, "1234") == 0) {

        printf("Login Successful!\n");
        return 1;
    }
    else {
        printf("Invalid Credentials!\n");
        return 0;
    }
}