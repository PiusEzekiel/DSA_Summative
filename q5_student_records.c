#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 1000
#define RED 1
#define BLACK 0

// ANSI color codes
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED_COLOR "\033[31m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"

// Structure for student records
typedef struct Student {
    int id;
    char name[100];
    float grades;
} Student;

// Node structure for Red-Black Tree
typedef struct RBNode {
    Student student;
    struct RBNode *left, *right, *parent;
    int color; // RED or BLACK
} RBNode;

// Red-Black Tree structure
typedef struct RBTree {
    RBNode *root;
    RBNode *TNULL; // Sentinel node
} RBTree;

// Function prototypes
RBTree* createRBTree();
RBNode* createNode(Student student);
void leftRotate(RBTree *tree, RBNode *x);
void rightRotate(RBTree *tree, RBNode *y);
void insertFix(RBTree *tree, RBNode *k);
void insert(RBTree *tree, Student student);
RBNode* search(RBTree *tree, int id);
void inOrderTraversalForSave(RBTree *tree, RBNode *node, FILE *file);
void inOrderTraversal(RBTree *tree, RBNode *node, Student *students, int *count);
void displayStudents(RBTree *tree);
int compareByName(const void *a, const void *b);
void deleteFix(RBTree *tree, RBNode *x);
RBNode* minimum(RBTree *tree, RBNode *node);
void rbDelete(RBTree *tree, int id);
void updateStudent(RBTree *tree, int id, const char *name, float grades);
void saveToFile(RBTree *tree, const char *filename);
void loadFromFile(RBTree *tree, const char *filename);
void freeTree(RBTree *tree, RBNode *node);
void printLine(const char *color, const char *text);
void printMenu();
int getIntInput();
float getFloatInput();
void clearInputBuffer();

int main() {
    RBTree *tree = createRBTree();
    loadFromFile(tree, "resources/student_records.txt");

    int choice, id;
    Student student;

    while (1) {
        printMenu();
        printf(BLUE "Choose an option: " RESET);
        choice = getIntInput();

        switch (choice) {
            case 1:
    printf(GREEN "Enter student ID: " RESET);
    student.id = getIntInput(); // This function should handle the input cleanly
    // Consume the newline character left in the buffer
    getchar(); 
    printf(GREEN "Enter student name: " RESET);
    fgets(student.name, sizeof(student.name), stdin);
    student.name[strcspn(student.name, "\n")] = '\0'; // Remove newline
    printf(GREEN "Enter student grades (0-100): " RESET);
    student.grades = getFloatInput();
    insert(tree, student);
    printLine(CYAN, "=====================================");
    printf(GREEN "Student added successfully!\n" RESET);
    break;
            case 2:
                printf(GREEN "Enter student ID to search: " RESET);
                id = getIntInput();
                RBNode *result = search(tree, id);
                if (result) {
                    printf("ID: %d, Name: %s, Grades: %.2f\n", result->student.id, result->student.name, result->student.grades);
                } else {
                    printf(RED_COLOR "Student not found.\n" RESET);
                }
                break;

            case 3:
                printf(GREEN "Enter student ID to update: " RESET);
                id = getIntInput();
                printf(GREEN "Enter new name: " RESET);
                getchar(); // Consume newline
                fgets(student.name, sizeof(student.name), stdin);
                student.name[strcspn(student.name, "\n")] = '\0'; // Remove newline
                printf(GREEN "Enter new grades: " RESET);
                student.grades = getFloatInput();
                updateStudent(tree, id, student.name, student.grades);
                break;

            case 4:
                printf(GREEN "Enter student ID to delete: " RESET);
                id = getIntInput();
                getchar();
                printf(GREEN "Are you sure you want to delete student ID %d? (y/n): " RESET, id);
    
                char confirm[10]; // Buffer for confirmation input
                fgets(confirm, sizeof(confirm), stdin); // Read the whole line

                // Check if the first character is 'y' or 'Y'
                if (confirm[0] == 'y' || confirm[0] == 'Y') {
                    rbDelete(tree, id);
                    printf(GREEN "Student deleted successfully!\n" RESET);
                } else {
                    printf(RED_COLOR "Deletion canceled.\n" RESET);
                }
                break;

            case 5:
                printf(GREEN "All students in ascending order of names:\n" RESET);
                printLine(CYAN, "=====================================");
                // inOrderTraversal(tree, tree->root);
                displayStudents(tree);
                printLine(CYAN, "=====================================");
                break;

            case 6:
                saveToFile(tree, "resources/student_records.txt");
                freeTree(tree, tree->root);
                free(tree->TNULL);
                free(tree);
                printf(RED_COLOR "Exiting...\n" RESET);
                return 0;

            default:
                printf(RED_COLOR "Invalid choice. Please try again.\n" RESET);
        }
    }
}

// Clear input buffer
void clearInputBuffer() {
    while (getchar() != '\n'); // Discard characters until newline
}

// Create a new Red-Black Tree
RBTree* createRBTree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->TNULL = (RBNode*)malloc(sizeof(RBNode));
    tree->TNULL->color = BLACK;
    tree->root = tree->TNULL;
    return tree;
}

// Create a new node
RBNode* createNode(Student student) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->student = student;
    node->left = node->right = node->parent = NULL;
    node->color = RED; // New nodes are red
    return node;
}

// Left rotate
void leftRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Right rotate
void rightRotate(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != tree->TNULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// Fix the Red-Black Tree after insertion
void insertFix(RBTree *tree, RBNode *k) {
    RBNode *u; // Uncle node
    while (k->parent && k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) {
            u = k->parent->parent->right;
            if (u && u->color == RED) {
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(tree, k->parent->parent);
            }
        } else {
            u = k->parent->parent->left;
            if (u && u->color == RED) {
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(tree, k->parent->parent);
            }
        }
        if (k == tree->root) {
            break;
        }
    }
    tree->root->color = BLACK;
}

// Insert a student record
void insert(RBTree *tree, Student student) {
    RBNode *node = createNode(student);
    RBNode *y = NULL;
    RBNode *x = tree->root;

    while (x != tree->TNULL) {
        y = x;
        if (node->student.id < x->student.id) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == NULL) {
        tree->root = node; // Tree was empty
    } else if (node->student.id < y->student.id) {
        y->left = node;
    } else {
        y->right = node;
    }

    node->left = node->right = tree->TNULL;

    insertFix(tree, node);
}

// Search for a student by ID
RBNode* search(RBTree *tree, int id) {
    RBNode *current = tree->root;
    while (current != tree->TNULL) {
        if (id == current->student.id) {
            return current;
        } else if (id < current->student.id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL; // Not found
}

// In-order traversal to display all students by name
void inOrderTraversal(RBTree *tree, RBNode *node, Student *students, int *count) {
    if (node != tree->TNULL) {
        inOrderTraversal(tree, node->left, students, count);
        students[(*count)++] = node->student; // Collect student
        inOrderTraversal(tree, node->right, students, count);
    }
}

// Display students in a table format
void displayStudents(RBTree *tree) {
    Student *students = malloc(sizeof(Student) * MAX_STUDENTS);
    int count = 0;

    // Collect students using in-order traversal
    inOrderTraversal(tree, tree->root, students, &count);

    // Check if any students were found
    if (count == 0) {
        printf(RED_COLOR "No student records found.\n" RESET);
        free(students);
        return;
    }

    // Sort the collected students by name
    qsort(students, count, sizeof(Student), compareByName);

    // Display sorted students in a tabular format
    printf("%-5s | %-30s | %-10s\n", "ID", "Name", "Grades");
    printf("----------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-30s | %-10.2f\n", students[i].id, students[i].name, students[i].grades);
    }

    free(students);
}

// Compare students by name for sorting
int compareByName(const void *a, const void *b) {
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

// Fix the Red-Black Tree after deletion
void deleteFix(RBTree *tree, RBNode *x) {
    RBNode *s;
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s && s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                s = x->parent->right;
            }
            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(tree, s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            s = x->parent->left;
            if (s && s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                s = x->parent->left;
            }
            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(tree, s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Find the node with minimum value
RBNode* minimum(RBTree *tree, RBNode *node) {
    while (node->left != tree->TNULL) {
        node = node->left;
    }
    return node;
}

// Delete a student record by ID
void rbDelete(RBTree *tree, int id) {
    if (tree == NULL || tree->root == NULL) {
        printf(RED_COLOR "Tree is empty.\n" RESET);
        return; // Tree is empty
    }

    RBNode *z = tree->root;
    RBNode *x, *y;

    // Find the node to delete
    while (z != tree->TNULL) {
        if (z->student.id == id) {
            break;
        } else if (id < z->student.id) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    // Node not found
    if (z == tree->TNULL) {
        printf(RED_COLOR "Student not found.\n" RESET);
        return;
    }

    y = z;
    int yOriginalColor = y->color;

    // Case 1: Node has no left child
    if (z->left == tree->TNULL) {
        x = z->right;
        if (x != tree->TNULL) x->parent = z->parent;
        if (z->parent == NULL) {
            tree->root = x; // Tree was empty
        } else if (z == z->parent->left) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
    } 
    // Case 2: Node has no right child
    else if (z->right == tree->TNULL) {
        x = z->left;
        if (x != tree->TNULL) x->parent = z->parent;
        if (z->parent == NULL) {
            tree->root = x; // Tree was empty
        } else if (z == z->parent->left) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
    } 
    // Case 3: Node has two children
    else {
        y = minimum(tree, z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            if (x != tree->TNULL) x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            y->right->parent = y;
        }

        if (z->parent == NULL) {
            tree->root = y;
        } else if (z == z->parent->left) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    // Fix the Red-Black Tree properties
    if (yOriginalColor == BLACK) {
        deleteFix(tree, x);
    }
}
// Update a student's record
void updateStudent(RBTree *tree, int id, const char *name, float grades) {
    RBNode *node = search(tree, id);
    if (node) {
        strncpy(node->student.name, name, sizeof(node->student.name));
        node->student.grades = grades;
        printf(GREEN "Student record updated successfully!\n" RESET);
    } else {
        printf(RED_COLOR "Student not found.\n" RESET);
    }
}

// In-order traversal function to write student records to file
void inOrderTraversalForSave(RBTree *tree, RBNode *node, FILE *file) {
    if (node != tree->TNULL) {
        inOrderTraversalForSave(tree, node->left, file);
        fprintf(file, "%d|%s|%.2f\n", node->student.id, node->student.name, node->student.grades);
        inOrderTraversalForSave(tree, node->right, file);
    }
}

// Save student records to a file
void saveToFile(RBTree *tree, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Could not open file for writing");
        return;
    }
    // In-order traversal to save records
    inOrderTraversalForSave(tree, tree->root, file);
    fclose(file);
}

// Load student records from a file
void loadFromFile(RBTree *tree, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Could not open file for reading");
        return;
    }
    Student student;
    while (fscanf(file, "%d|%99[^|]|%f\n", &student.id, student.name, &student.grades) == 3) {
        insert(tree, student); // Assume insert function is defined elsewhere
    }
    fclose(file);
}

// Free the allocated memory for the tree
void freeTree(RBTree *tree, RBNode *node) {
    if (node != tree->TNULL) {
        freeTree(tree, node->left);
        freeTree(tree, node->right);
        free(node);
    }
}

// Print a decorative line
void printLine(const char *color, const char *text) {
    printf("%s%s%s\n", color, text, RESET);
}

// Print the menu
void printMenu() {
    printLine(CYAN, "=====================================");
    printf(MAGENTA "Student Records Management System:\n" RESET);
    printf(MAGENTA "1. Add Student\n" RESET);
    printf(MAGENTA "2. Search Student by ID\n" RESET);
    printf(MAGENTA "3. Update Student Record\n" RESET);
    printf(MAGENTA "4. Delete Student Record\n" RESET);
    printf(MAGENTA "5. Display All Students\n" RESET);
    printf(MAGENTA "6. Exit\n" RESET);
    printLine(CYAN, "=====================================");
}

// Get integer input with validation
int getIntInput() {
    int value;
    while (1) {
        // printf(GREEN "Enter a valid student ID (positive integer): " RESET);
        if (scanf("%d", &value) != 1 || value <= 0) {
            printf(RED_COLOR "Invalid input. Please enter a positive integer.\n" RESET);
            while (getchar() != '\n'); // Clear invalid input
        } else {
            break; // Valid input
        }
    }
    return value;
}

// Get float input with validation
float getFloatInput() {
    float value;
    while (scanf("%f", &value) != 1) {
        printf(RED_COLOR "Invalid input. Please enter a float: " RESET);
        while(getchar() != '\n'); // Clear invalid input
    }
    return value;
}