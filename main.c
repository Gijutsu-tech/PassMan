#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct
{
    char siteName[100];
    char passwd[100];
}passForm;

typedef struct {
    char userName[50];
    char masterPasswd[100];
} user;

int searchSites(user loggedUser);
int createPassForm(user loggedUser);
int registerUser(void);
int loginUser(user *loggedUser);
void clrBuffer();

int main(){
    int choice;
    user loggedUser;

    loginUser(&loggedUser);

    printf("Welcome to password manager!\n\n");

    printf("1. Register a user\n");
    printf("2. Create a password\n");
    printf("3. Search a password\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        registerUser();
        break;

    case 2:
        createPassForm(loggedUser);
        break;

    case 3:
        searchSites(loggedUser);
        break;
    
    default:
        printf("Invalid option");
        break;
    }

    return 0;
}

int createPassForm(user loggedUser){
    FILE *file;
    passForm newForm;
    char path[250];

    clrBuffer();
    printf("Enter the website: ");
    fgets(newForm.siteName, sizeof(newForm.siteName), stdin);

    printf("Enter the password: ");
    fgets(newForm.passwd, sizeof(newForm.passwd), stdin);

    newForm.siteName[strcspn(newForm.siteName, "\n")] = 0;
    newForm.passwd[strcspn(newForm.passwd, "\n")] = 0;

    printf("The site is %s and the password is %s\n", newForm.siteName, newForm.passwd);

    snprintf(path, sizeof(path), "./passVault/%s/%s.txt", loggedUser.userName, loggedUser.userName);
    file = fopen(path, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    fprintf(file, "Site: %s|Password: %s\n", newForm.siteName, newForm.passwd);
    fclose(file);

    return 0;
}

int searchSites(user loggedUser){
    FILE *file;

    char searchSite[100];
    char line[256];
    char path[250];
    int found = 0; // To make future implications based on this

    clrBuffer();
    printf("Enter the website: ");
    fgets(searchSite, sizeof(searchSite), stdin);
    searchSite[strcspn(searchSite, "\n")] = 0;

    snprintf(path, sizeof(path), "./passVault/%s/%s.txt", loggedUser.userName, loggedUser.userName);
    file = fopen(path, "r");
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, searchSite)) {
            printf("\nFound: %s", line);
            found++;
        }
    }
    if (!found)
    {
        printf("No entry as %s", searchSite);
    }
    
    fclose(file);
    found = 0;
    return 0;
}

int registerUser(void){
    FILE *file;

    user newUser;
    char cmd[100];

    clrBuffer();
    printf("Username: ");
    fgets(newUser.userName, sizeof(newUser.userName), stdin);
    printf("Password (Remember this!): ");
    fgets(newUser.masterPasswd, sizeof(newUser.masterPasswd), stdin);

    newUser.userName[strcspn(newUser.userName, "\n")] = 0;
    newUser.masterPasswd[strcspn(newUser.masterPasswd, "\n")] = 0;

    snprintf(cmd, sizeof(cmd), "mkdir -p ./passVault/%s", newUser.userName);
    system(cmd);
    snprintf(cmd, sizeof(cmd), "touch ./passVault/%s/%s.txt", newUser.userName, newUser.userName);
    system(cmd);

    file = fopen("./users.txt", "a");
    fprintf(file, "Username: %s|Password: %s\n", newUser.userName, newUser.masterPasswd);
    fclose(file);
}

int loginUser(user *loggedUser){
    FILE *file;

    char searchUser[50];
    char searchPasswd[100];
    char searchLine[200];
    char line[200];
    int userExists = 1;

    printf("Login: \n");

    printf("Username: ");
    fgets(searchUser, sizeof(searchUser), stdin);
    printf("Password: ");
    fgets(searchPasswd, sizeof(searchPasswd), stdin);

    searchUser[strcspn(searchUser, "\n")] = 0;
    searchPasswd[strcspn(searchPasswd, "\n")] = 0;

    file = fopen("./users.txt", "r");
    while (fgets(searchLine, sizeof(searchLine), file)) {
        if (strstr(searchLine, searchUser) != NULL) {
            strcpy(line, searchLine);
            userExists = 0;
            if (strstr(searchLine, searchPasswd) != NULL)
            {
                strcpy(loggedUser->userName, searchUser);
                strcpy(loggedUser->masterPasswd, searchPasswd);

                printf("Logged in as \"%s\"!\n", loggedUser->userName);
            }
            else {
                printf("Invalid username or password\n");
            }
        }
    }

    if (userExists == 1)
    {
        printf("No such user named \"%s\"\n", searchUser);
        return 1;
    }

    fclose(file);
    return 0;
}

void clrBuffer()
{
    int c;
    // Check if there's input to clear (non-empty buffer)
    if (feof(stdin) || ferror(stdin))
        return; // Exit if EOF or error
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}