#include<stdio.h>
#include<stdlib.h>

#define SUBJECTS 3
#define MAX_STUDENTS 100

struct Student {//dta type for storing student data
    int roll_no;
    int marks[SUBJECTS];
    float total_marks;
    float average_marks;
    char name[50];
    char grade;
};
//functions declarations
int read_no_of_students();
void read_student_data(struct Student *stu);
void calculate_ratings(struct Student *stu);//find out total marks average marks and grade
void print_student_data(struct Student *stu);
//print all the things of student
void print_pattern(char grade);
void print_recursive_roll_no(struct Student students[], int index, int max_index);
void process_students(struct Student students[], int count);
void print_all_roll_no(Students, num_students);
//function definee
int read_no_of_students() {
    int n;
    if(scanf("%d", &n) != 1 || n<1 || n>MAX_STUDENTS) {
        printf( "Error: Invalid number of students.\n");
        exit(1);
    }
    return n;
}
void read_student_data(struct Student *stu){
    if(scanf("%d %s %d %d %d", &stu->roll_no, stu->name, &stu->marks[0], &stu->marks[1], &stu->marks[2]) != 5){
        printf( "Error reading student data.\n");
    }
}
void calculate_ratings(struct Student *stu) {
    float sum = 0;
    for(int i=0; i<SUBJECTS; i++){
        sum += stu->marks[i];
    }
    stu->total_marks=sum;
    stu->average_marks=sum/SUBJECTS;
    if(stu->average_marks >= 85)
    stu->grade = 'A';
    else if(stu->average_marks >= 70)
    stu->grade = 'B';
    else if(stu->average_marks >= 50)
    stu->grade = 'C';
    else if(stu->average_marks >= 35)
    stu->grade = 'D';
    else
    stu->grade = 'F';
}
void print_pattern(char grade) {
    int stars = 0;

    switch(grade) {
        case 'A': stars = 5;
        break;
        case 'B': stars = 4;
        break;
        case 'C': stars = 3;
        break;
        case 'D': stars = 2;
        break;
        default: stars = 0;
    }
    if (stars == 0){
        printf("Ratings: (Skipped, Grade F)\n");
        return;
    }
    printf("Performance: ");
    for (int i=0; i<stars; i++) {
        printf("*");
    }
    printf("\n");
}
void print_student_data(struct Student *stu) {
    printf("Roll: %d\n", stu->roll_no);
    printf("Name: %s\n", stu->name);
    printf("Total: %.0f\n", stu->total_marks);
    printf("Average: %.2f\n", stu->average_marks);
    printf("Grade: %c\n", stu->grade);
    print_pattern(stu->grade);
}

void process_students(struct Student students[], int count){
    for(int i=0; i<count; i++) {
        read_student_data(&students[i]);
        calculate_ratings(&students[i]);
        print_student_data(&students[i]);
        if(i<count-1) {
            printf("\n");
        }
    }
}
void print_recursive_roll_no(struct Student students[], int index, int max_index){
    if(index >= max_index)
    return;
    printf("%d", students[index].roll_no);
    if (index < max_index-1)
    printf(" ");
    print_recursive_roll_no(students, index + 1, max_index);
}
void print_all_roll_no(struct Student students[], int count){
        printf("\nList of Roll No (from recursion): ");
        print_recursive_roll_no(students, 0, count);
        printf("\n");
}
int main(){
    int num_students=read_no_of_students();
    struct Student students[MAX_STUDENTS];
    process_students(students, num_students);
    print_all_roll_no(students, num_students);
    return 0;
}
