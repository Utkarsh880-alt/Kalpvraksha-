#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FN "users.txt"
#define TN "temp_users.txt"
#define NL 50 


struct U { 
    int i;
    char n[NL];
    int a;
};


void clr_buf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int is_id_unique(int id) {
    FILE *f = fopen(FN, "r");
    if (!f) return 1; 
    
    struct U temp_u;
    int is_unique = 1;

    
    while (fscanf(f, "%d,%49[^,],%d\n", &temp_u.i, temp_u.n, &temp_u.a) == 3) {
        if (temp_u.i == id) {
            is_unique = 0; 
            break;
        }
    }
    
    fclose(f);
    return is_unique;
}


int main() {
    int id, fnd, scan_res;
    char cmd;
    FILE *f, *t;
    struct U u; 

    printf("--- Simple C File CRUD ---\n");

    while (1) {
        printf("\nC/R/U/D: ");
        scan_res = scanf(" %c", &cmd);
        if (scan_res != 1) { printf("\nExiting.\n"); 
            break; }
        cmd = toupper(cmd);
        clr_buf();

        switch (cmd) {
            // --- C: CREATE (ADD UNIQUE RECORD) ---
            case 'C':
                f = fopen(FN, "a");
                if (!f) { perror("File Error"); break; }
                
                printf("ID Name Age: ");
                
                if (scanf("%d %s %d", &u.i, u.n, &u.a) == 3 && u.i > 0 && u.a >= 0) {
                    
                    
                    if (is_id_unique(u.i)) {
                        fprintf(f, "%d,%s,%d\n", u.i, u.n, u.a);
                        printf("ID %d added.\n", u.i);
                    } else {
                        printf("ERROR: ID %d already exists. Must be unique.\n", u.i);
                    }
                } else {
                    printf("ERROR: Invalid input or data.\n");
                }
                clr_buf();
                fclose(f);
                break;

            // --- R: READ ---
            case 'R':
                f = fopen(FN, "r");
                if (!f) { printf("No records found.\n"); 
                    break; }
                
                printf("\nID\tNAME\tAGE\n---\t----\t---\n");
                fnd = 0;
                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3) {
                    printf("%d\t%s\t%d\n", u.i, u.n, u.a);
                    fnd = 1;
                }
                if (!fnd) { printf("(File is empty.)\n"); }
                
                fclose(f);
                break;
                
            // --- U: UPDATE ---
            case 'U':
                f = fopen(FN, "r");
                if (!f) { printf("No records found.\n"); 
                    break; }
                t = fopen(TN, "w");

                if (!t) { perror("Temp Error"); fclose(f); 
                    break; }

                printf("Target ID to UPDATE: "); 
                if (scanf("%d", &id) != 1) { 
                    printf("Invalid ID.\n"); 
                    clr_buf(); fclose(f); fclose(t); remove(TN); 
                    break;
                }
                clr_buf();
                fnd = 0;

                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3) {
                    if (u.i == id) {
                        fnd = 1;
                        printf("Found ID %d. New Name/Age: ", id);
                        char new_n[NL]; int new_a;
                        if (scanf("%s %d", new_n, &new_a) == 2 && new_a >= 0) {
                            fprintf(t, "%d,%s,%d\n", u.i, new_n, new_a);
                            printf("ID %d UPDATED.\n", id);
                        } else {
                            fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a);
                            printf("WARNING: Update failed, original kept.\n");
                        }
                        clr_buf(); 
                    } else {
                        fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a); 
                    }
                }
                
                fclose(f); fclose(t);
                if (fnd) { 
                    remove(FN); 
                    rename(TN, FN); 
                } else { 
                    remove(TN); 
                    printf("ERROR: ID %d not found.\n", id); 
                }
                break;

            //  D: DELETE 
            case 'D':
                f = fopen(FN, "r");
                if (!f) { printf("No records found.\n"); 
                    break; }
                t = fopen(TN, "w");
                if (!t) { perror("Temp Error"); fclose(f); 
                    break; }

                printf("Target ID to DELETE: "); 
                if (scanf("%d", &id) != 1) { 
                    printf("Invalid ID.\n"); 
                    clr_buf(); fclose(f); fclose(t); remove(TN);
                    break;
                }
                clr_buf();
                fnd = 0;

                while (fscanf(f, "%d,%49[^,],%d\n", &u.i, u.n, &u.a) == 3) {
                    if (u.i == id) {
                        fnd = 1;
                        printf("ID %d DELETED.\n", id); 
                    } else {
                        fprintf(t, "%d,%s,%d\n", u.i, u.n, u.a); 
                    }
                }
                
                fclose(f); fclose(t);
                if (fnd) { 
                    remove(FN); 
                    rename(TN, FN); 
                } else { 
                    remove(TN); 
                    printf("ERROR: ID %d not found.\n", id); 
                }
                break;
                
            default:
                printf("Invalid command.\n");
        }
    }
    return 0;
}