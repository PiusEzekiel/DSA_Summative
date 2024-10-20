#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// Node structure for the BST
typedef struct Node {
    char *word;
    char *definition;
    struct Node *left;
    struct Node *right;
} Node;

// Function prototypes
Node* createNode(const char *word, const char *definition);
Node* insert(Node* root, const char *word, const char *definition);
Node* search(Node* root, const char *word);
Node* deleteNode(Node* root, const char *word);
Node* findMin(Node* root);
void inOrderTraversal(Node* root);
void freeTree(Node* root);
void clearInputBuffer();
void printLine(const char *color, const char *text);
void printMenu();
void saveToFile(Node* root, FILE* file);
void loadFromFile(Node** root, FILE* file);

int main() {
    Node* root = NULL;
    FILE* file;

    // Load existing words from file
    file = fopen("resources/dictionary.txt", "a+");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    loadFromFile(&root, file);
    fclose(file);

    int choice;
    char word[100], definition[256];

    while (1) {
        printMenu();
        printf(BLUE "Choose an option: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                printf(GREEN "Enter word: " RESET);
                fgets(word, sizeof(word), stdin);
                word[strcspn(word, "\n")] = '\0'; // Remove newline

                printf(GREEN "Enter definition: " RESET);
                fgets(definition, sizeof(definition), stdin);
                definition[strcspn(definition, "\n")] = '\0'; // Remove newline

                root = insert(root, word, definition);
                printf(GREEN "Word added successfully!\n" RESET);
                break;

            case 2:
                printf(GREEN "Enter word to search: " RESET);
                fgets(word, sizeof(word), stdin);
                word[strcspn(word, "\n")] = '\0'; // Remove newline

                Node* result = search(root, word);
                if (result) {
                    printf(YELLOW "Definition of '%s': %s\n" RESET, result->word, result->definition);
                } else {
                    printf(RED "Word not found.\n" RESET);
                }
                break;

            case 3:
                printf(GREEN "Enter word to delete: " RESET);
                fgets(word, sizeof(word), stdin);
                word[strcspn(word, "\n")] = '\0'; // Remove newline

                root = deleteNode(root, word);
                printf(GREEN "Word deleted successfully!\n" RESET);
                break;

            case 4:
                printf(YELLOW "All words in alphabetical order:\n" RESET);
                printLine(CYAN, "=====================================");
                inOrderTraversal(root);
                printLine(CYAN, "=====================================");
                break;

            case 5:
                // Save words to file before exiting
                file = fopen("resources/dictionary.txt", "w");
                if (file) {
                    saveToFile(root, file);
                    fclose(file);
                } else {
                    perror("Failed to save to file");
                }
                freeTree(root);
                printf(RED "Exiting...\n" RESET);
                return 0;

            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}

// Create a new node
Node* createNode(const char *word, const char *definition) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->word = strdup(word);
    newNode->definition = strdup(definition);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Insert a node into the BST
Node* insert(Node* root, const char *word, const char *definition) {
    if (root == NULL) {
        return createNode(word, definition);
    }

    if (strcasecmp(word, root->word) < 0) {
        root->left = insert(root->left, word, definition);
    } else if (strcasecmp(word, root->word) > 0) {
        root->right = insert(root->right, word, definition);
    } else {
        // Word already exists, update definition
        free(root->definition);
        root->definition = strdup(definition);
    }
    return root;
}

// Search for a word in the BST (case-insensitive)
Node* search(Node* root, const char *word) {
    if (root == NULL || strcasecmp(word, root->word) == 0) {
        return root;
    }

    if (strcasecmp(word, root->word) < 0) {
        return search(root->left, word);
    } else {
        return search(root->right, word);
    }
}

// Delete a node from the BST (case-insensitive)
Node* deleteNode(Node* root, const char *word) {
    if (root == NULL) {
        return NULL;
    }

    if (strcasecmp(word, root->word) < 0) {
        root->left = deleteNode(root->left, word);
    } else if (strcasecmp(word, root->word) > 0) {
        root->right = deleteNode(root->right, word);
    } else {
        // Node with the word found
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root->word);
            free(root->definition);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root->word);
            free(root->definition);
            free(root);
            return temp;
        }

        // Node with two children: get the inorder successor
        Node* temp = findMin(root->right);
        free(root->word);
        free(root->definition);
        root->word = strdup(temp->word);
        root->definition = strdup(temp->definition);
        root->right = deleteNode(root->right, temp->word);
    }
    return root;
}

// Find the minimum value node in the BST
Node* findMin(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// In-order traversal to display all words
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%s: %s\n", root->word, root->definition);
        inOrderTraversal(root->right);
    }
}

// Free the allocated memory for the tree
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->word);
        free(root->definition);
        free(root);
    }
}

// Clear input buffer function
void clearInputBuffer() {
    while (getchar() != '\n');
}

// Print decorative line
void printLine(const char *color, const char *text) {
    printf("%s%s%s\n", color, text, RESET);
}

// Print menu with decorations
void printMenu() {
    printLine(CYAN, "=====================================");
    printf(MAGENTA "Dictionary Menu:\n" RESET);
    printf(GREEN "1. Add Word\n" RESET);
    printf(GREEN "2. Search Definition\n" RESET);
    printf(GREEN "3. Delete Word\n" RESET);
    printf(GREEN "4. Display All Words\n" RESET);
    printf(GREEN "5. Exit\n" RESET);
    printLine(CYAN, "=====================================");
}

// Save words to file
void saveToFile(Node* root, FILE* file) {
    if (root != NULL) {
        saveToFile(root->left, file);
        fprintf(file, "%s|%s\n", root->word, root->definition);
        saveToFile(root->right, file);
    }
}

// Load words from file
void loadFromFile(Node** root, FILE* file) {
    char word[100], definition[256];
    while (fscanf(file, "%99[^|]|%255[^\n]\n", word, definition) == 2) {
        *root = insert(*root, word, definition);
    }
}