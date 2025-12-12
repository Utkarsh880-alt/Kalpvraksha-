#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for product
typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

int main() {
    int n, choice, i;

    printf("Enter initial number of products: ");
    scanf("%d", &n);

    // 1) Allocate initial memory using calloc()
    Product *p = (Product *)calloc(n, sizeof(Product));

    // Input initial products
    for (i = 0; i < n; i++) {
        printf("\nProduct %d ID: ", i + 1);
        scanf("%d", &p[i].id);

        printf("Name: ");
        getchar();
        fgets(p[i].name, 50, stdin);
        p[i].name[strcspn(p[i].name, "\n")] = 0;

        printf("Price: ");
        scanf("%f", &p[i].price);

        printf("Quantity: ");
        scanf("%d", &p[i].quantity);
    }

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Add Product\n2. View Products\n3. Update Quantity\n");
        printf("4. Search by ID\n5. Search by Name\n6. Price Range\n7. Delete Product\n8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // 2) ADD PRODUCT using realloc()
        if (choice == 1) {
            n++; 

            // Increase memory
            p = (Product *)realloc(p, n * sizeof(Product));

            printf("Enter ID: ");
            scanf("%d", &p[n - 1].id);

            printf("Name: ");
            getchar();
            fgets(p[n - 1].name, 50, stdin);
            p[n - 1].name[strcspn(p[n - 1].name, "\n")] = 0;

            printf("Price: ");
            scanf("%f", &p[n - 1].price);

            printf("Quantity: ");
            scanf("%d", &p[n - 1].quantity);

            printf("Product Added!\n");
        }

        // 3) VIEW PRODUCTS
        else if (choice == 2) {
            for (i = 0; i < n; i++) {
                printf("ID:%d | %s | %.2f | Qty:%d\n",
                    p[i].id, p[i].name, p[i].price, p[i].quantity);
            }
        }

        // 4) UPDATE QUANTITY
        else if (choice == 3) {
            int id, qty, found = 0;
            printf("Enter Product ID: ");
            scanf("%d", &id);

            for (i = 0; i < n; i++) {
                if (p[i].id == id) {
                    printf("Enter new quantity: ");
                    scanf("%d", &qty);
                    p[i].quantity = qty;
                    found = 1;
                    printf("Updated!\n");
                    break;
                }
            }
            if (!found) printf("Product not found!\n");
        }

        // 5) SEARCH BY ID
        else if (choice == 4) {
            int id, found = 0;
            printf("Enter Product ID: ");
            scanf("%d", &id);

            for (i = 0; i < n; i++) {
                if (p[i].id == id) {
                    printf("Found: %s | %.2f | Qty:%d\n",
                        p[i].name, p[i].price, p[i].quantity);
                    found = 1;
                }
            }

            if (!found) printf("Not found!\n");
        }

        // 6) SEARCH BY NAME (partial match)
        else if (choice == 5) {
            char key[50];
            int found = 0;

            printf("Enter name (partial allowed): ");
            getchar();
            fgets(key, 50, stdin);
            key[strcspn(key, "\n")] = 0;

            for (i = 0; i < n; i++) {
                if (strstr(p[i].name, key)) {
                    printf("ID:%d | %s | %.2f | Qty:%d\n",
                        p[i].id, p[i].name, p[i].price, p[i].quantity);
                    found = 1;
                }
            }
            if (!found) printf("No match found!\n");
        }
        // 7) PRICE RANGE SEARCH
        else if (choice == 6) {
            float min, max;
            printf("Enter minimum price: ");
            scanf("%f", &min);

            printf("Enter maximum price: ");
            scanf("%f", &max);

            for (i = 0; i < n; i++) {
                if (p[i].price >= min && p[i].price <= max) {
                    printf("ID:%d | %s | %.2f | Qty:%d\n",
                        p[i].id, p[i].name, p[i].price, p[i].quantity);
                }
            }
        }
        // 8) DELETE PRODUCT (shift + realloc)
        else if (choice == 7) {
            int id, index = -1;
            printf("Enter Product ID to delete: ");
            scanf("%d", &id);

            // Find index
            for (i = 0; i < n; i++) {
                if (p[i].id == id)
                    index = i;
            }
            if (index == -1) {
                printf("Product not found!\n");
            } else {
                // Shift elements
                for (i = index; i < n - 1; i++) {
                    p[i] = p[i + 1];
                }
                n--;
                p = (Product *)realloc(p, n * sizeof(Product));

                printf("Product Deleted!\n");
            }
        }
        // 9) EXIT (free memory)
        else if (choice == 8) {
            free(p);
            printf("Memory freed. Exiting..\n");
            break;
        }
        else {
            printf("Invalid choice!\n");
        }
    }
    return 0;
}