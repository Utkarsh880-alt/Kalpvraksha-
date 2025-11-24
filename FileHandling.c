#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAIN_FILENAME "users.txt"
#define NAME_LENGTH 50 

struct USER_STRUCT { 
    int id;
    char name[NAME_LENGTH];
    int age;
};

void clear_input_buffer() {
    int character;
    while ((character = getchar()) != '\n' && character != EOF);
}

int is_id_unique(int check_id) {
    FILE *file_ptr = fopen(MAIN_FILENAME, "r");
    if (!file_ptr) return 1; 
    
    struct USER_STRUCT temp_user;
    int is_unique_flag = 1;

    while (fscanf(file_ptr, "%d,%49[^,],%d\n", &temp_user.id, temp_user.name, &temp_user.age) == 3) {
        if (temp_user.id == check_id) {
            is_unique_flag = 0; 
            break;
        }
    }
    
    fclose(file_ptr);
    return is_unique_flag;
}

int main() {
    int target_id, found_flag, scan_result;
    char command;
    FILE *file_ptr;
    struct USER_STRUCT user_record; 

    printf("--- Simple C File CRUD ---\n");

    while (1) {
        printf("\nC/R/U/D (or any other key to exit): ");
        scan_result = scanf(" %c", &command);
        if (scan_result != 1) { 
            printf("\nExiting.\n"); 
            break; 
        }
        command = toupper(command);
        clear_input_buffer();

        switch (command) {
            // --- C: CREATE (ADD UNIQUE RECORD) ---
            case 'C':
                file_ptr = fopen(MAIN_FILENAME, "a");
                if (!file_ptr) { 
                    perror("File Error"); 
                    break; 
                }
                
                printf("ID Name Age: ");
                
                if (scanf("%d %s %d", &user_record.id, user_record.name, &user_record.age) == 3 && user_record.id > 0 && user_record.age >= 0) {
                    clear_input_buffer();
                    
                    if (is_id_unique(user_record.id)) {
                        fprintf(file_ptr, "%d,%s,%d\n", user_record.id, user_record.name, user_record.age);
                        printf("ID %d added.\n", user_record.id);
                    } else {
                        printf("ERROR: ID %d already exists. Must be unique.\n", user_record.id);
                    }
                } else {
                    printf("ERROR: Invalid input or data.\n");
                    clear_input_buffer();
                }
                
                fclose(file_ptr);
                break;

            // --- R: READ (ALL RECORDS) ---
            case 'R':
                file_ptr = fopen(MAIN_FILENAME, "r");
                if (!file_ptr) { 
                    printf("No records found.\n"); 
                    break; 
                }
                
                printf("\nID\tNAME\tAGE\n---\t----\t---\n");
                found_flag = 0;
                while (fscanf(file_ptr, "%d,%49[^,],%d\n", &user_record.id, user_record.name, &user_record.age) == 3) {
                    if (user_record.id != -1) { 
                        printf("%d\t%s\t%d\n", user_record.id, user_record.name, user_record.age);
                        found_flag = 1;
                    }
                }
                if (!found_flag) { 
                    printf("(File is empty or only contains deleted records.)\n"); 
                }
                
                fclose(file_ptr);
                break;
                
            // --- U: UPDATE  ---
            case 'U':
                file_ptr = fopen(MAIN_FILENAME, "r+"); 
                if (!file_ptr) { 
                    printf("No records found.\n"); 
                    break; 
                }
                
                printf("Target ID to UPDATE: "); 
                if (scanf("%d", &target_id) != 1) { 
                    printf("Invalid ID.\n"); 
                    clear_input_buffer(); 
                    fclose(file_ptr);
                    break;
                }
                clear_input_buffer();
                found_flag = 0;
                long position = 0;

                while (fscanf(file_ptr, "%d,%49[^,],%d\n", &user_record.id, user_record.name, &user_record.age) == 3) {
                    long next_position = ftell(file_ptr);
                    
                    if (user_record.id == target_id) {
                        found_flag = 1;
                        printf("Found ID %d. New Name/Age: ", target_id);
                        char new_name[NAME_LENGTH]; 
                        int new_age;
                        
                        if (scanf("%s %d", new_name, &new_age) == 2 && new_age >= 0) {
                            
                            char new_line[2 * NAME_LENGTH];
                            sprintf(new_line, "%d,%s,%d\n", user_record.id, new_name, new_age);
                            
                            int old_len = next_position - position;
                            int new_len = strlen(new_line);
                            
                            if (new_len <= old_len) {
                                fseek(file_ptr, position, SEEK_SET);
                                fprintf(file_ptr, "%s", new_line);
                                for (int i = new_len; i < old_len; i++) {
                                    fputc(' ', file_ptr);
                                }
                                printf("ID %d UPDATED in place.\n", target_id);
                            } else {
                                printf("WARNING: New record is longer. Cannot update in place. Update failed.\n");
                            }
                        } else {
                            printf("WARNING: Update input failed.\n");
                        }
                        clear_input_buffer();
                        break;
                    }
                    position = next_position;
                }
                
                fclose(file_ptr);
                if (!found_flag) { 
                    printf("ERROR: ID %d not found.\n", target_id); 
                }
                break;

            case 'D':
                file_ptr = fopen(MAIN_FILENAME, "r+"); 
                if (!file_ptr) { 
                    printf("No records found.\n"); 
                    break; 
                }

                printf("Target ID to DELETE: "); 
                if (scanf("%d", &target_id) != 1) { 
                    printf("Invalid ID.\n"); 
                    clear_input_buffer(); 
                    fclose(file_ptr);
                    break;
                }
                clear_input_buffer();
                found_flag = 0;
                position = 0;

                while (fscanf(file_ptr, "%d,%49[^,],%d\n", &user_record.id, user_record.name, &user_record.age) == 3) {
                    long next_position = ftell(file_ptr);
                    
                    if (user_record.id == target_id) {
                        found_flag = 1;
                        
                        char deleted_line[2 * NAME_LENGTH];
                        sprintf(deleted_line, "%d,%s,%d\n", -1, user_record.name, user_record.age);
                        
                        int old_len = next_position - position;
                        int new_len = strlen(deleted_line);

                        fseek(file_ptr, position, SEEK_SET);
                        fprintf(file_ptr, "%s", deleted_line);
                        for (int i = new_len; i < old_len; i++) {
                            fputc(' ', file_ptr);
                        }
                        
                        printf("ID %d LOGICALLY DELETED (ID set to -1).\n", target_id); 
                        break;
                    }
                    position = next_position;
                }
                
                fclose(file_ptr);
                if (!found_flag) { 
                    printf("ERROR: ID %d not found.\n", target_id); 
                }
                break;
                
            default:
                printf("Invalid command.\n");
        }
    }
    return 0;
}
