
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DATA_FILE "students.dat"
#define TEMP_FILE "students.tmp"

#define NAME_LEN  50
#define COURSE_LEN 40

typedef struct
{
    int id;
    char name[NAME_LEN];
    int age;
    char course[COURSE_LEN];
    float marks;
} Student;

int readLine(const char *prompt, char *buffer, size_t size)
{
    int ch;
    size_t len;

    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL)
        return 0;

    len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
    else
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            /* Discard extra input. */
        }
    }

    return 1;
}

/* Read an integer with validation. */
int readInt(const char *prompt, int *value)
{
    char input[100];
    char *end;
    long number;

    if (!readLine(prompt, input, sizeof(input)))
        return 0;

    errno = 0;
    number = strtol(input, &end, 10);

    while (*end == ' ' || *end == '\t')
        end++;

    if (input[0] == '\0' || *end != '\0' ||
        errno == ERANGE || number < -2147483647L - 1 ||
        number > 2147483647L)
    {
        printf("Invalid input! Please enter a whole number.\n");
        return -1;
    }

    *value = (int)number;
    return 1;
}

/* Read a floating-point number with validation. */
int readFloat(const char *prompt, float *value)
{
    char input[100];
    char *end;
    float number;

    if (!readLine(prompt, input, sizeof(input)))
        return 0;

    errno = 0;
    number = strtof(input, &end);

    while (*end == ' ' || *end == '\t')
        end++;

    if (input[0] == '\0' || *end != '\0' ||
        errno == ERANGE || number != number ||
        number > 3.4028234e+38F ||
        number < -3.4028234e+38F)
    {
        printf("Invalid input! Please enter a valid number.\n");
        return -1;
    }

    *value = number;
    return 1;
}

/* Find a student by ID. */
int findStudent(int id, Student *found)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    Student s;

    if (fp == NULL)
    {
        if (errno == ENOENT)
            return 0;

        perror("Error opening student file");
        return -1;
    }

    while (fread(&s, sizeof(Student), 1, fp) == 1)
    {
        if (s.id == id)
        {
            if (found != NULL)
                *found = s;

            fclose(fp);
            return 1;
        }
    }

    if (ferror(fp))
    {
        perror("Error reading student file");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

/* Display one student record. */
void printStudent(Student s)
{
    printf("\n+---------------------------------------------------------------+\n");
    printf("| Student ID : %-47d |\n", s.id);
    printf("| Name       : %-47s |\n", s.name);
    printf("| Age        : %-47d |\n", s.age);
    printf("| Course     : %-47s |\n", s.course);
    printf("| Marks      : %-47.2f |\n", s.marks);
    printf("+---------------------------------------------------------------+\n");
}

/* Add a new student. */
void addStudent(void)
{
    Student s;
    int status;

    printf("\n========== ADD STUDENT ==========\n");

    status = readInt("Enter Student ID: ", &s.id);
    if (status <= 0)
        return;

    if (s.id <= 0)
    {
        printf("Student ID must be positive.\n");
        return;
    }

    status = findStudent(s.id, NULL);

    if (status == 1)
    {
        printf("A student with this ID already exists!\n");
        return;
    }

    if (status == -1)
        return;

    if (!readLine("Enter Name: ", s.name, sizeof(s.name)))
        return;

    if (s.name[0] == '\0')
    {
        printf("Name cannot be empty.\n");
        return;
    }

    status = readInt("Enter Age: ", &s.age);
    if (status <= 0)
        return;

    if (s.age < 1 || s.age > 120)
    {
        printf("Please enter an age between 1 and 120.\n");
        return;
    }

    if (!readLine("Enter Course: ", s.course, sizeof(s.course)))
        return;

    if (s.course[0] == '\0')
    {
        printf("Course cannot be empty.\n");
        return;
    }

    status = readFloat("Enter Marks (0-100): ", &s.marks);
    if (status <= 0)
        return;

    if (s.marks < 0 || s.marks > 100)
    {
        printf("Marks must be between 0 and 100.\n");
        return;
    }

    FILE *fp = fopen(DATA_FILE, "ab");

    if (fp == NULL)
    {
        perror("Unable to open student file");
        return;
    }

    if (fwrite(&s, sizeof(Student), 1, fp) != 1)
    {
        perror("Error saving student record");
        fclose(fp);
        return;
    }

    if (fclose(fp) != 0)
    {
        perror("Error closing student file");
        return;
    }

    printf("\nStudent record added successfully!\n");
}

/* Display all student records. */
void displayStudents(void)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    Student s;
    int count = 0;

    printf("\n================ STUDENT RECORDS ================\n");

    if (fp == NULL)
    {
        if (errno == ENOENT)
            printf("No student records found.\n");
        else
            perror("Unable to open student file");

        return;
    }

    printf("%-8s %-24s %-6s %-18s %-8s\n",
           "ID", "NAME", "AGE", "COURSE", "MARKS");
    printf("-----------------------------------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, fp) == 1)
    {
        printf("%-8d %-24.24s %-6d %-18.18s %-8.2f\n",
               s.id, s.name, s.age, s.course, s.marks);
        count++;
    }

    if (ferror(fp))
        perror("Error reading student file");

    fclose(fp);

    printf("-----------------------------------------------------------------------\n");
    printf("Total student records: %d\n", count);
}

/* Search for a student by ID. */
void searchStudent(void)
{
    int id, status;
    Student s;

    printf("\n========== SEARCH STUDENT ==========\n");

    status = readInt("Enter Student ID to search: ", &id);
    if (status <= 0)
        return;

    status = findStudent(id, &s);

    if (status == 1)
    {
        printf("\nStudent found!\n");
        printStudent(s);
    }
    else if (status == 0)
    {
        printf("Student with ID %d not found.\n", id);
    }
}

/* Rewrite the file after updating or deleting a record. */
int rewriteFile(int id, const Student *replacement, int deleteRecord)
{
    FILE *src = fopen(DATA_FILE, "rb");
    FILE *tmp;
    Student s;
    int found = 0;
    int ok = 1;

    if (src == NULL)
    {
        perror("Unable to open student file");
        return 0;
    }

    tmp = fopen(TEMP_FILE, "wb");

    if (tmp == NULL)
    {
        perror("Unable to create temporary file");
        fclose(src);
        return 0;
    }

    while (fread(&s, sizeof(Student), 1, src) == 1)
    {
        if (s.id == id)
        {
            found = 1;

            if (deleteRecord)
                continue;

            s = *replacement;
        }

        if (fwrite(&s, sizeof(Student), 1, tmp) != 1)
        {
            ok = 0;
            break;
        }
    }

    if (ferror(src))
        ok = 0;

    if (fclose(src) != 0)
        ok = 0;

    if (fclose(tmp) != 0)
        ok = 0;

    if (!ok || !found)
    {
        if (!ok)
            printf("File operation failed. Original records were retained.\n");
        else
            printf("Student with ID %d not found.\n", id);

        remove(TEMP_FILE);
        return 0;
    }

    
    if (remove(DATA_FILE) != 0)
    {
        perror("Unable to replace original file");
        remove(TEMP_FILE);
        return 0;
    }

    if (rename(TEMP_FILE, DATA_FILE) != 0)
    {
        perror("Unable to rename temporary file");
        printf("The updated data remains in %s.\n", TEMP_FILE);
        return 0;
    }

    return 1;
}

/* Update an existing student. */
void updateStudent(void)
{
    int id, status;
    Student s;

    printf("\n========== UPDATE STUDENT ==========\n");

    status = readInt("Enter Student ID to update: ", &id);
    if (status <= 0)
        return;

    status = findStudent(id, &s);

    if (status != 1)
    {
        if (status == 0)
            printf("Student with ID %d not found.\n", id);
        return;
    }

    printf("\nCurrent student details:\n");
    printStudent(s);
    printf("Enter the new details below.\n");

    if (!readLine("Enter New Name: ", s.name, sizeof(s.name)))
        return;

    if (s.name[0] == '\0')
    {
        printf("Name cannot be empty.\n");
        return;
    }

    status = readInt("Enter New Age: ", &s.age);
    if (status <= 0)
        return;

    if (s.age < 1 || s.age > 120)
    {
        printf("Please enter an age between 1 and 120.\n");
        return;
    }

    if (!readLine("Enter New Course: ", s.course, sizeof(s.course)))
        return;

    if (s.course[0] == '\0')
    {
        printf("Course cannot be empty.\n");
        return;
    }

    status = readFloat("Enter New Marks (0-100): ", &s.marks);
    if (status <= 0)
        return;

    if (s.marks < 0 || s.marks > 100)
    {
        printf("Marks must be between 0 and 100.\n");
        return;
    }

    if (rewriteFile(id, &s, 0))
        printf("Student record updated successfully!\n");
}

/* Delete a student by ID. */
void deleteStudent(void)
{
    int id, status;
    char confirm[10];
    Student s;

    printf("\n========== DELETE STUDENT ==========\n");

    status = readInt("Enter Student ID to delete: ", &id);
    if (status <= 0)
        return;

    status = findStudent(id, &s);

    if (status != 1)
    {
        if (status == 0)
            printf("Student with ID %d not found.\n", id);
        return;
    }

    printf("\nRecord to be deleted:\n");
    printStudent(s);

    if (!readLine("Confirm deletion? (y/n): ",
                  confirm, sizeof(confirm)))
        return;

    if (confirm[0] != 'y' && confirm[0] != 'Y')
    {
        printf("Deletion cancelled.\n");
        return;
    }

    if (rewriteFile(id, NULL, 1))
        printf("Student record deleted successfully!\n");
}

/* Main menu. */
int main(void)
{
    int choice;
    int status;

    do
    {
        printf("\n");
        printf("========================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("  1. Add Student\n");
        printf("  2. Display All Students\n");
        printf("  3. Search Student\n");
        printf("  4. Update Student\n");
        printf("  5. Delete Student\n");
        printf("  6. Exit\n");
        printf("========================================\n");

        status = readInt("Enter your choice (1-6): ", &choice);

        if (status == 0)
            break;

        if (status == -1)
            continue;

        switch (choice)
        {
            case 1:
                addStudent();
                break;

            case 2:
                displayStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                printf("\nThank you for using Student Management System!\n");
                break;

            default:
                printf("Invalid choice! Please select 1 to 6.\n");
        }

    } while (choice != 6);

    return 0;
}
