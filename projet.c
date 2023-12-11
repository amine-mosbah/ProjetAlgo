#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[50];
    char author[50];
    char department[20];
};

struct Student {
    int id;
    char name[50];
};

const char bookFileName[] = "books.txt";
const char studentFileName[] = "students.txt";
const char issueFileName[] = "issues.txt";

void mainmenu();
void addbooks();
void deletebooks();
void editbooks();
void searchbooks();
void issuebooks();
void viewbooks();
void issuerecord();
int checkid(int);

int main() {
    mainmenu();
    return 0;
}

void deletebooks() {
    FILE* fp = fopen(bookFileName, "r");
    if (fp == NULL) {
        printf("Error opening book file for reading.\n");
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    int deleteId;
    printf("Enter the ID of the book to delete: ");
    scanf("%d", &deleteId);

    struct Book tempBook;
    int found = 0;

    while (fscanf(fp, "%d %[^\t\n] %[^\t\n] %[^\t\n]\n", &tempBook.id, tempBook.title, tempBook.author, tempBook.department) == 4) {
        if (tempBook.id == deleteId) {
            found = 1;
            printf("Book deleted successfully.\n");
        } else {
            fprintf(tempFile, "%d\t%s\t%s\t%s\n", tempBook.id, tempBook.title, tempBook.author, tempBook.department);
        }
    }

    fclose(fp);
    fclose(tempFile);

    remove(bookFileName);
    rename("temp.txt", bookFileName);

    if (!found) {
        printf("Book with ID %d not found.\n", deleteId);
    }
}

void editbooks() {
    FILE* fp = fopen(bookFileName, "r");
    if (fp == NULL) {
        printf("Error opening book file for reading.\n");
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    int editId;
    printf("Enter the ID of the book to edit: ");
    scanf("%d", &editId);

    struct Book tempBook;
    int found = 0;

    while (fscanf(fp, "%d %[^\t\n] %[^\t\n] %[^\t\n]\n", &tempBook.id, tempBook.title, tempBook.author, tempBook.department) == 4) {
        if (tempBook.id == editId) {
            found = 1;
            printf("Enter new title: ");
            getchar();
            fgets(tempBook.title, sizeof(tempBook.title), stdin);
            tempBook.title[strcspn(tempBook.title, "\n")] = '\0';

            printf("Enter new author: ");
            fgets(tempBook.author, sizeof(tempBook.author), stdin);
            tempBook.author[strcspn(tempBook.author, "\n")] = '\0';

            printf("Enter new department: ");
            fgets(tempBook.department, sizeof(tempBook.department), stdin);
            tempBook.department[strcspn(tempBook.department, "\n")] = '\0';

            printf("Book edited successfully.\n");
        }

        fprintf(tempFile, "%d\t%s\t%s\t%s\n", tempBook.id, tempBook.title, tempBook.author, tempBook.department);
    }

    fclose(fp);
    fclose(tempFile);

    remove(bookFileName);
    rename("temp.txt", bookFileName);

    if (!found) {
        printf("Book with ID %d not found.\n", editId);
    }
}

void searchbooks() {
    FILE* fp = fopen(bookFileName, "r");
    if (fp == NULL) {
        printf("Error opening book file for reading.\n");
        return;
    }

    char searchQuery[50];
    printf("Enter the title, author, or department to search: ");
    getchar();
    fgets(searchQuery, sizeof(searchQuery), stdin);
    searchQuery[strcspn(searchQuery, "\n")] = '\0';

    struct Book tempBook;
    int found = 0;

    printf("\nSearch Results:\n");
    printf("%-5s%-30s%-20s%-20s\n", "ID", "Title", "Author", "Department");
    printf("------------------------------------------------------------\n");

    while (fscanf(fp, "%d %[^\t\n] %[^\t\n] %[^\t\n]\n", &tempBook.id, tempBook.title, tempBook.author, tempBook.department) == 4) {
        if (strstr(tempBook.title, searchQuery) || strstr(tempBook.author, searchQuery) || strstr(tempBook.department, searchQuery)) {
            found = 1;
            printf("%-5d%-30s%-20s%-20s\n", tempBook.id, tempBook.title, tempBook.author, tempBook.department);
        }
    }

    fclose(fp);

    if (!found) {
        printf("No matching books found.\n");
    }
}

void issuebooks() {
    FILE* fp = fopen(issueFileName, "r");
    if (fp == NULL) {
        printf("Error opening issue file for reading.\n");
        return;
    }

    struct Book tempBook;
    struct Student tempStudent;

    printf("\nIssued Books:\n");
    printf("%-5s%-30s%-20s%-20s%-20s\n", "ID", "Title", "Author", "Department", "Student Name");
    printf("-------------------------------------------------------------------------------\n");

    while (fscanf(fp, "%d", &tempBook.id) == 1) {
        fseek(stdin, 0, SEEK_END); // Clear input buffer
        fscanf(fp, "\t%[^\t\n]\t%[^\t\n]\t%[^\t\n]\n", tempBook.title, tempBook.author, tempBook.department);

        printf("%-5d%-30s%-20s%-20s", tempBook.id, tempBook.title, tempBook.author, tempBook.department);

        FILE* studentFile = fopen(studentFileName, "r");
        if (studentFile != NULL) {
            while (fscanf(studentFile, "%d\t%[^\t\n]\n", &tempStudent.id, tempStudent.name) == 2) {
                if (tempStudent.id == tempBook.id) {
                    printf("%-20s\n", tempStudent.name);
                    break;
                }
            }
            fclose(studentFile);
        } else {
            printf("Error opening student file for reading.\n");
        }
    }

    fclose(fp);
}

void viewbooks() {
    FILE* fp = fopen(bookFileName, "r");
    if (fp == NULL) {
        printf("Error opening book file for reading.\n");
        return;
    }

    struct Book tempBook;

    printf("\nAll Books:\n");
    printf("%-5s%-30s%-20s%-20s\n", "ID", "Title", "Author", "Department");
    printf("------------------------------------------------------------\n");

    while (fscanf(fp, "%d %[^\t\n] %[^\t\n] %[^\t\n]\n", &tempBook.id, tempBook.title, tempBook.author, tempBook.department) == 4) {
        printf("%-5d%-30s%-20s%-20s\n", tempBook.id, tempBook.title, tempBook.author, tempBook.department);
    }

    fclose(fp);
}

void issuerecord() {
    FILE* issueFile = fopen(issueFileName, "a");
    if (issueFile == NULL) {
        printf("Error opening issue file for writing.\n");
        return;
    }

    struct Book tempBook;
    struct Student tempStudent;

    printf("Enter Book ID to issue: ");
    int issueId;
    scanf("%d", &issueId);

    if (!checkid(issueId)) {
        printf("Book with ID %d does not exist.\n", issueId);
        fclose(issueFile);
        return;
    }

    FILE* studentFile = fopen(studentFileName, "a");
    if (studentFile == NULL) {
        printf("Error opening student file for writing.\n");
        fclose(issueFile);
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &tempStudent.id);

    if (!checkid(tempStudent.id)) {
        printf("Student with ID %d does not exist.\n", tempStudent.id);
        fclose(issueFile);
        fclose(studentFile);
        return;
    }

    printf("Enter Student Name: ");
    getchar();
    fgets(tempStudent.name, sizeof(tempStudent.name), stdin);
    tempStudent.name[strcspn(tempStudent.name, "\n")] = '\0';

    fprintf(studentFile, "%d\t%s\n", tempStudent.id, tempStudent.name);

    fclose(studentFile);

    // Mark the book as issued in the book file
    FILE* bookFile = fopen(bookFileName, "r+");
    if (bookFile == NULL) {
        printf("Error opening book file for updating.\n");
        fclose(issueFile);
        return;
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(issueFile);
        fclose(bookFile);
        return;
    }

    while (fscanf(bookFile, "%d %[^\t\n] %[^\t\n] %[^\t\n]\n", &tempBook.id, tempBook.title, tempBook.author, tempBook.department) == 4) {
        if (tempBook.id == issueId) {
            fprintf(issueFile, "%d\t%s\t%s\t%s\n", tempBook.id, tempBook.title, tempBook.author, tempBook.department);
        } else {
            fprintf(tempFile, "%d\t%s\t%s\t%s\n", tempBook.id, tempBook.title, tempBook.author, tempBook.department);
        }
    }

    fclose(bookFile);
    fclose(tempFile);

    remove(bookFileName);
    rename("temp.txt", bookFileName);

    printf("Book issued successfully.\n");

    fclose(issueFile);
}

int checkid(int id) {
    FILE* fp = fopen(bookFileName, "r");
    if (fp == NULL) {
        printf("Error opening book file for reading.\n");
        return 0;
    }

    struct Book tempBook;
    int found = 0;

    while (fscanf(fp, "%d %[^\t\n] %[^\t\n] %[^\t\n]\n", &tempBook.id, tempBook.title, tempBook.author, tempBook.department) == 4) {
        if (tempBook.id == id) {
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}

void mainmenu() {
    int choice;
    do {
        printf("\n=== Library Management System ===\n");
        printf("1. Add Book\n");
        printf("2. Delete Book\n");
        printf("3. Edit Book\n");
        printf("4. Search Book\n");
        printf("5. Issue Books\n");
        printf("6. View Books\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addbooks();
                break;
            case 2:
                deletebooks();
                break;
            case 3:
                editbooks();
                break;
            case 4:
                searchbooks();
                break;
            case 5:
                issuebooks();
                break;
            case 6:
                viewbooks();
                break;
            case 7:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
}

void addbooks() {
    FILE* fp = fopen(bookFileName, "a");
    if (fp == NULL) {
        printf("Error opening book file for writing.\n");
        return;
    }

    struct Book newBook;

    printf("Enter Book ID: ");
    scanf("%d", &newBook.id);

    // Check if book with the given ID already exists
    if (checkid(newBook.id)) {
        printf("Book with ID %d already exists. Aborting.\n", newBook.id);
        fclose(fp);
        return;
    }

    printf("Enter Title: ");
    getchar();  // Consume the newline character left by previous scanf
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';  // Remove trailing newline

    printf("Enter Author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';

    printf("Enter Department: ");
    fgets(newBook.department, sizeof(newBook.department), stdin);
    newBook.department[strcspn(newBook.department, "\n")] = '\0';

    fprintf(fp, "%d\t%s\t%s\t%s\n", newBook.id, newBook.title, newBook.author, newBook.department);

    printf("Book added successfully.\n");

    fclose(fp);
}

