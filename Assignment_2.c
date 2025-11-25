#include <stdio.h>
#include<stdlib.h>

#define SUBJECTS 3 

struct Student {
    int roll_number;
    int marks[SUBJECTS];
    char name[50];
    char grade;
    float total_marks;
    float average_marks;
};
//these are functions declarations
void calculate_performance(struct Student *stu);
void print_pattern(char grade);
void print_roll_numbers_recursive(int current_roll, int max_roll);

void calculate_performance(struct Student *stu) { 
    //total sum
    float subject_sum = 0;
    
    for (int i = 0; i < SUBJECTS; i++) {
        subject_sum += stu->marks[i];
    }
    
    stu->total_marks = subject_sum;
    stu->average_marks = subject_sum / (float)SUBJECTS;
    
    if (stu->average_marks >= 85) {
        stu->grade = 'A';
    } else if (stu->average_marks >= 70) {
        stu->grade = 'B';
    } else if (stu->average_marks >= 50) {
        stu->grade = 'C';
    } else if (stu->average_marks >= 35) {
        stu->grade = 'D';
    } else {
        stu->grade = 'F';
    }
}

void print_pattern(char grade) {
    int stars;
    switch (grade) {
        case 'A': stars = 5; 
        break;
        case 'B': stars = 4; 
        break;
        case 'C': stars = 3; 
        break;
        case 'D': stars = 2; 
        break;
        default: stars = 0; // Grade 'F'
    }
    
    if (stars == 0) {
        printf("Performance: (Skipped, Grade F)\n"); 
        return; 
    }
    
    printf("Performance: ");
    for (int i = 0; i < stars; i++) {
        printf("*");
    }
    printf("\n");
}
//function to print roll numbers recursive
void print_roll_numbers_recursive(int current_roll, int max_roll) {

    printf("%d", current_roll);
    if (current_roll < max_roll) {
        printf(" ");
    }
    print_roll_numbers_recursive(current_roll + 1, max_roll);
}
int main() {
    int num_students;
    
    if (scanf("%d", &num_students) != 1 || num_students < 1 || num_students > 100) {
        fprintf(stderr, "Error: Invalid number of students (N).\n");
        return 1;
    }
    struct Student students[num_students];
    
    for (int i = 0; i < num_students; i++) {
        struct Student *current_student = &students[i];
        

        if (scanf("%d %s %d %d %d",
                &current_student->roll_number,
                current_student->name,
                &current_student->marks[0],
                &current_student->marks[1],
                &current_student->marks[2]) != 5) {
            fprintf(stderr, "Error reading data for student %d. Program may continue with incomplete data.\n", i + 1);
        }
        calculate_performance(current_student);

        printf("Roll: %d\n", current_student->roll_number);
        printf("Name: %s\n", current_student->name);
        printf("Total: %.0f\n", current_student->total_marks);
        printf("Average: %.2f\n", current_student->average_marks); 
        printf("Grade: %c\n", current_student->grade);
        print_pattern(current_student->grade);  
        if (i < num_students - 1) {
            printf("\n");
        }
    }
    printf("\n");
    printf("List of Roll Numbers (via recursion): ");
    print_roll_numbers_recursive(1, num_students);
    printf("\n");

    return 0;
}