#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHAR 256

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// Node structure for Huffman Tree
typedef struct Node {
    char character;
    unsigned frequency;
    struct Node *left, *right;
} Node;

// MinHeap structure
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    Node **array;
} MinHeap;

// Function prototypes
MinHeap* createMinHeap(unsigned capacity);
void insertMinHeap(MinHeap *minHeap, Node *node);
Node* extractMin(MinHeap *minHeap);
void minHeapify(MinHeap *minHeap, int idx);
bool isSizeOne(MinHeap *minHeap);
Node* createNode(char character, unsigned frequency);
void printCodes(Node *root, char *arr, int top, char codes[MAX_CHAR][MAX_CHAR]);
void compress(const char *inputFilePath, const char *outputFilePath);
void decompress(const char *inputFilePath, const char *outputFilePath);
void compareFileSizes(const char *originalFilePath, const char *compressedFilePath);
void freeTree(Node *root);
void printDivider();

int main() {
    char inputFilePath[256];
    char outputFilePath[256];
    char choice;

    printDivider();
    printf(MAGENTA "Huffman Coding for File Compression\n" RESET);
    printDivider();

    while (true) {
        printf(BLUE "Choose an operation:\n" RESET);
        printf("1. Compress a file\n");
        printf("2. Decompress a file\n");
        printf("3. Exit\n");
        printf(BLUE "Enter your choice (1/2/3): " RESET);
        scanf(" %c", &choice);

        if (choice == '1') {
            printf(BLUE "\nEnter the path of the file to compress (e.g. resources/text.txt): " RESET);
            scanf("%s", inputFilePath);
            printf(BLUE "Enter the output path for compressed file (e.g. resources/compressed.txt): " RESET);
            scanf("%s", outputFilePath);
            compress(inputFilePath, outputFilePath);
            // // Compare the sizes of the input and output files
            // compareFileSizes(inputFilePath, outputFilePath);
            // printf(GREEN "\nCompressed from: %s to: %s\n" RESET, inputFilePath, outputFilePath);
        } else if (choice == '2') {
            printf(BLUE "\nEnter the path of the compressed file (e.g. resources/compressed.txt): " RESET);
            scanf("%s", inputFilePath);
            printf(BLUE "Enter the output path for decompressed file (e.g. resources/text.txt): " RESET);
            scanf("%s", outputFilePath);
            decompress(inputFilePath, outputFilePath);
            // Compare the sizes after decompression
            // compareFileSizes(inputFilePath, outputFilePath);
            // printf(GREEN "\nDecompressed from: %s to: %s\n" RESET, inputFilePath, outputFilePath);
        } else if (choice == '3') {
            printf(GREEN "Exiting the program...\n" RESET);
            break;
        } else {
            printf(RED "Invalid choice. Please try again.\n" RESET);
        }

        printDivider();
    }

    return 0;
}

void printDivider() {
    printf(YELLOW "---------------------------------------------------------\n" RESET);
}

// Create a new MinHeap
MinHeap* createMinHeap(unsigned capacity) {
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (Node **)malloc(minHeap->capacity * sizeof(Node *));
    return minHeap;
}

// Create a new Huffman Tree node
Node* createNode(char character, unsigned frequency) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Swap two nodes
void swapNode(Node **a, Node **b) {
    Node *t = *a;
    *a = *b;
    *b = t;
}

// Min-Heapify function
void minHeapify(MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Check if size of the heap is one
bool isSizeOne(MinHeap *minHeap) {
    return (minHeap->size == 1);
}

// Extract the minimum node from the heap
Node* extractMin(MinHeap *minHeap) {
    Node *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a node into the min heap
void insertMinHeap(MinHeap *minHeap, Node *node) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && node->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

// Print the codes for characters and store them
void printCodes(Node *root, char *arr, int top, char codes[MAX_CHAR][MAX_CHAR]) {
    if (root->left) {
        arr[top] = '0';
        printCodes(root->left, arr, top + 1, codes);
    }

    if (root->right) {
        arr[top] = '1';
        printCodes(root->right, arr, top + 1, codes);
    }

    if (!root->left && !root->right) {
        arr[top] = '\0';
        strcpy(codes[(unsigned char)root->character], arr);
    }
}

// Compress the file
void compress(const char *inputFilePath, const char *outputFilePath) {
    int frequency[MAX_CHAR] = {0};
    FILE *inputFile = fopen(inputFilePath, "r");
    char c;

    if (!inputFile) {
        printf(RED "Error opening input file: %s\n" RESET, inputFilePath);
        return;
    }

    // Count frequency of each character
    while ((c = fgetc(inputFile)) != EOF) {
        frequency[(unsigned char)c]++;
    }
    fclose(inputFile);

    MinHeap *minHeap = createMinHeap(MAX_CHAR);
    for (int i = 0; i < MAX_CHAR; i++) {
        if (frequency[i]) {
            Node *node = createNode((char)i, frequency[i]);
            insertMinHeap(minHeap, node);
        }
    }

    // Build the Huffman Tree
    while (minHeap->size != 1) {
        Node *left = extractMin(minHeap);
        Node *right = extractMin(minHeap);
        Node *top = createNode('\0', left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    // Generate codes
    char arr[MAX_CHAR];
    char codes[MAX_CHAR][MAX_CHAR];
    printCodes(minHeap->array[0], arr, 0, codes);

    // Write the compressed data to the output file
    FILE *outputFile = fopen(outputFilePath, "wb");
    if (!outputFile) {
        printf(RED "Error opening output file: %s\n" RESET, outputFilePath);
        return;
    }

    // Write frequency data for decompression
    fwrite(frequency, sizeof(int), MAX_CHAR, outputFile);

    // Write encoded data
    inputFile = fopen(inputFilePath, "r");
    if (!inputFile) {
        printf(RED "Error opening input file for encoding: %s\n" RESET, inputFilePath);
        fclose(outputFile);
        return;
    }

    unsigned char buffer = 0;
    int bitCount = 0;

    while ((c = fgetc(inputFile)) != EOF) {
        const char *code = codes[(unsigned char)c];
        for (int i = 0; code[i] != '\0'; i++) {
            buffer = (buffer << 1) | (code[i] - '0');
            bitCount++;
            if (bitCount == 8) {
                fwrite(&buffer, sizeof(unsigned char), 1, outputFile);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Write any remaining bits
    if (bitCount > 0) {
        buffer <<= (8 - bitCount); // Pad with zeros
        fwrite(&buffer, sizeof(unsigned char), 1, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    // Compare the sizes of the input and output files
            compareFileSizes(inputFilePath, outputFilePath);
            printf(GREEN "\nCompressed from: %s to: %s\n" RESET, inputFilePath, outputFilePath);

    // Free memory
    freeTree(minHeap->array[0]);
    free(minHeap->array);
    free(minHeap);
}

// Decompress the file
void decompress(const char *inputFilePath, const char *outputFilePath) {
    FILE *inputFile = fopen(inputFilePath, "rb");
    if (!inputFile) {
        printf(RED "Error opening compressed file: %s\n" RESET, inputFilePath);
        return;
    }

    // Read frequency data
    int frequency[MAX_CHAR];
    fread(frequency, sizeof(int), MAX_CHAR, inputFile);

    MinHeap *minHeap = createMinHeap(MAX_CHAR);
    for (int i = 0; i < MAX_CHAR; i++) {
        if (frequency[i]) {
            Node *node = createNode((char)i, frequency[i]);
            insertMinHeap(minHeap, node);
        }
    }

    // Build the Huffman Tree
    while (minHeap->size != 1) {
        Node *left = extractMin(minHeap);
        Node *right = extractMin(minHeap);
        Node *top = createNode('\0', left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    // Decode the data
    FILE *outputFile = fopen(outputFilePath, "w");
    if (!outputFile) {
        printf(RED "Error opening output file for decompression: %s\n" RESET, outputFilePath);
        fclose(inputFile);
        return;
    }

    Node *current = minHeap->array[0];
    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, inputFile)) {
        for (int i = 0; i < 8; i++) {
            current = (buffer & (1 << (7 - i))) ? current->right : current->left;
            if (!current->left && !current->right) {
                fputc(current->character, outputFile);
                current = minHeap->array[0]; // Go back to the root
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    // Display sizes of compressed and decompressed files
    compareFileSizes(inputFilePath, outputFilePath);
    printf(GREEN "\nDecompressed from: %s to: %s\n" RESET, inputFilePath, outputFilePath);

    // Free memory
    freeTree(minHeap->array[0]);
    free(minHeap->array);
    free(minHeap);
}

// Compare file sizes
void compareFileSizes(const char *originalFilePath, const char *newFilePath) {
    FILE *originalFile = fopen(originalFilePath, "rb");
    fseek(originalFile, 0, SEEK_END);
    long originalSize = ftell(originalFile);
    fclose(originalFile);

    FILE *newFile = fopen(newFilePath, "rb");
    fseek(newFile, 0, SEEK_END);
    long newSize = ftell(newFile);
    fclose(newFile);

    printf(MAGENTA "\nOriginal Size of '%s': %ld bytes\n" RESET, originalFilePath, originalSize);
    printf(GREEN "New Size of '%s': %ld bytes\n" RESET, newFilePath, newSize);
}

// Free the Huffman tree
void freeTree(Node *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}