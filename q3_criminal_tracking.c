#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // For isdigit

#define MAX_PEOPLE 100

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// Node structure for adjacency list
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Graph structure
typedef struct Graph {
    int numVertices;
    char **names;
    Node **adjLists;
} Graph;

// Queue structure for BFS
typedef struct Queue {
    int *items;
    int front, rear;
} Queue;

// Function prototypes
Graph* createGraph(int vertices);
void addEdge(Graph* graph, int src, int dest);
void bfs(Graph* graph, int startVertex, int criminalIndex);
Queue* createQueue();
bool isEmpty(Queue* q);
void enqueue(Queue* q, int value);
int dequeue(Queue* q);
void printContacts(int level, int* contacts, int count, Graph* graph);
void freeGraph(Graph* graph);
void freeQueue(Queue* q);
void clearInputBuffer();
void initializeSampleGraph(Graph* graph);
void displayMenu();
int findCriminalIndex(Graph* graph, const char* name);

int main() {
    int numPeople, numConnections, choice;
    int criminalIndex;

    printf(MAGENTA "Welcome to the Criminal Tracking System!" RESET "\n");
    printf(YELLOW "=========================================\n" RESET);
    
    Graph* graph = createGraph(MAX_PEOPLE);
    if (!graph) {
        printf(RED "Memory allocation failed. Exiting." RESET "\n");
        return 1;
    }

    // Initialize sample graph
    initializeSampleGraph(graph);

    while (1) {
        displayMenu();
        printf(BLUE "Choose an option: " RESET);
        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 1) {
            printf(BLUE "Using the sample graph\n" RESET);
            // Display the sample graph people
            printf(GREEN "People in the Sample Graph:\n" RESET);
            printf(YELLOW "----------------------------\n" RESET);
            for (int i = 0; i < graph->numVertices; i++) {
                printf("%d: %s\n", i + 1, graph->names[i]);
            }

            // Prompt for criminal by index or name
            printf(BLUE "Enter the index (1 to %d) or name of the criminal: " RESET, graph->numVertices);
            char input[100];
            scanf("%s", input);
            clearInputBuffer();

            // Check if input is a number or a name
            if (isdigit(input[0])) {
                criminalIndex = atoi(input) - 1; // Convert to 0-based index
                if (criminalIndex < 0 || criminalIndex >= graph->numVertices) {
                    printf(RED "Invalid index. Please try again.\n" RESET);
                    continue;
                }
            } else {
                criminalIndex = findCriminalIndex(graph, input);
                if (criminalIndex == -1) {
                    printf(RED "Criminal not found. Please try again.\n" RESET);
                    continue;
                }
            }

            printf(GREEN "\nBFS contacts tracing from %s:\n" RESET, graph->names[criminalIndex]);
            bfs(graph, criminalIndex, criminalIndex);
        } else if (choice == 2) {
            printf(BLUE "Enter the number of people in the network (max %d): " RESET, MAX_PEOPLE);
            while (scanf("%d", &numPeople) != 1 || numPeople <= 0 || numPeople > MAX_PEOPLE) {
                clearInputBuffer();
                printf(RED "Invalid input. Please enter a valid number of people: " RESET);
            }

            // Input names
            for (int i = 0; i < numPeople; i++) {
                printf(BLUE "Enter the name of person %d: " RESET, i + 1);
                graph->names[i] = (char*)malloc(100 * sizeof(char));
                if (!graph->names[i]) {
                    printf(RED "Memory allocation failed for name. Exiting." RESET "\n");
                    freeGraph(graph);
                    return 1;
                }
                scanf("%s", graph->names[i]);
            }

            printf(BLUE "Enter the number of connections: " RESET);
            while (scanf("%d", &numConnections) != 1 || numConnections < 0) {
                clearInputBuffer();
                printf(RED "Invalid input. Please enter a valid number of connections: " RESET);
            }
            
            // Input connections
            for (int i = 0; i < numConnections; i++) {
                int src, dest;
                printf(BLUE "Enter connection (person1 person2 - as indices (1 to %d): " RESET, numPeople);
                while (scanf("%d %d", &src, &dest) != 2 || src < 1 || src > numPeople || dest < 1 || dest > numPeople) {
                    clearInputBuffer();
                    printf(RED "Invalid connection. Please enter valid indices (1 to %d): " RESET, numPeople);
                }
                addEdge(graph, src - 1, dest - 1); // Adjust for 0-based indexing
            }

            int newCriminalIndex;
            while (1) {
                printf(BLUE "Enter the index of the criminal (1 to %d) or '0' to return to menu: " RESET, numPeople);
                while (scanf("%d", &newCriminalIndex) != 1 || newCriminalIndex < 0 || newCriminalIndex > numPeople) {
                    clearInputBuffer();
                    printf(RED "Invalid index for criminal. Please enter a valid index (1 to %d): " RESET, numPeople);
                }
                if (newCriminalIndex == 0) break;

                newCriminalIndex--; // Adjust for 0-based indexing
                printf(GREEN "\nBFS contacts tracing from %s:\n" RESET, graph->names[newCriminalIndex]);
                bfs(graph, newCriminalIndex, newCriminalIndex);
            }
        } else if (choice == 3) {
            printf(RED "Exiting the program.\n" RESET);
            break;
        } else {
            printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }

    // Free graph memory
    freeGraph(graph);
    return 0;
}

// Create a graph with a specified number of vertices
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->numVertices = vertices;
    graph->names = (char**)malloc(vertices * sizeof(char*));
    graph->adjLists = (Node**)malloc(vertices * sizeof(Node*));

    for (int i = 0; i < vertices; i++) {
        graph->names[i] = NULL;
        graph->adjLists[i] = NULL;
    }
    return graph;
}

// Add an edge to the graph
void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = src;
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode; // Undirected graph
}

// Create a queue for BFS
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->items = (int*)malloc(MAX_PEOPLE * sizeof(int));
    q->front = -1;
    q->rear = -1;
    return q;
}

// Check if the queue is empty
bool isEmpty(Queue* q) {
    return q->front == -1;
}

// Enqueue an item
void enqueue(Queue* q, int value) {
    if (q->rear == MAX_PEOPLE - 1) {
        return; // Queue is full
    }
    if (isEmpty(q)) {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = value;
}

// Dequeue an item
int dequeue(Queue* q) {
    if (isEmpty(q)) {
        return -1; // Queue is empty
    }
    int item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = q->rear = -1; // Reset queue
    }
    return item;
}

// Perform BFS to find connected contacts
void bfs(Graph* graph, int startVertex, int criminalIndex) {
    bool *visited = (bool*)calloc(graph->numVertices, sizeof(bool));
    int *level = (int*)calloc(graph->numVertices, sizeof(int));
    int contacts[MAX_PEOPLE][MAX_PEOPLE] = {0}; // Ensure this has a clear size and is initialized
    int count[MAX_PEOPLE] = {0};
    
    Queue* q = createQueue();
    if (!q) {
        printf(RED "Memory allocation for queue failed. Exiting." RESET "\n");
        free(visited);
        free(level);
        return;
    }

    visited[startVertex] = true;
    enqueue(q, startVertex);

    while (!isEmpty(q)) {
        int current = dequeue(q);
        int currentLevel = level[current];

        // Store the current contact in the appropriate level, skip criminal
        if (current != criminalIndex && currentLevel < MAX_PEOPLE) {
            contacts[currentLevel][count[currentLevel]++] = current;
        }

        Node* temp = graph->adjLists[current];
        while (temp) {
            if (!visited[temp->vertex]) {
                visited[temp->vertex] = true;
                level[temp->vertex] = currentLevel + 1; // Set level for the next contact
                enqueue(q, temp->vertex);
            }
            temp = temp->next;
        }
    }

    // Print contacts by level
    for (int i = 0; i < MAX_PEOPLE; i++) { // Ensure not to exceed the bounds
        if (count[i] > 0) {
            printContacts(i, contacts[i], count[i], graph);
        }
    }

    free(visited);
    free(level);
    freeQueue(q);
}

// Print contacts at a given level with formatting
void printContacts(int level, int* contacts, int count, Graph* graph) {
    // printf("\n" CYAN "================================================" RESET "\n");
    printf(YELLOW "Level %d Contacts:\n" RESET, level);
    printf(YELLOW "------------------------------------------------" RESET "\n");
    for (int i = 0; i < count; i++) {
        printf("%-10s", graph->names[contacts[i]]);
    }
    printf("\n" CYAN "===============================================" RESET "\n");
}

// Free graph memory
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->names[i]) free(graph->names[i]);
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph->names);
    free(graph->adjLists);
    free(graph);
}

// Free queue memory
void freeQueue(Queue* q) {
    free(q->items);
    free(q);
}

// Clear input buffer
void clearInputBuffer() {
    while (getchar() != '\n');
}

// Initialize a sample graph with connections
void initializeSampleGraph(Graph* graph) {
    graph->numVertices = 10; // Sample with 10 people
    graph->names[0] = "Samson";
    graph->names[1] = "Cynthia";
    graph->names[2] = "Anthony";
    graph->names[3] = "John";
    graph->names[4] = "Ada";
    graph->names[5] = "Ngozi";
    graph->names[6] = "Daniel";
    graph->names[7] = "Joy";
    graph->names[8] = "Peter";
    graph->names[9] = "Grace";

    // Sample connections
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 3, 5);
    addEdge(graph, 4, 6);
    addEdge(graph, 1, 6);
    addEdge(graph, 5, 7);
    addEdge(graph, 6, 8);
    addEdge(graph, 7, 9);
}

// Display menu options
void displayMenu() {
    printf("\n" YELLOW "Menu Options:" RESET "\n");
    printf("1. Use Sample Graph\n");
    printf("2. Create New Criminal Connections\n");
    printf("3. Exit\n");
}

// Find the criminal index by name
int findCriminalIndex(Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->names[i], name) == 0) {
            return i; // Return 0-based index
        }
    }
    return -1; // Not found
}