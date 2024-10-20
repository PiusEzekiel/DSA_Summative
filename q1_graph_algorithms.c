#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// Edge structure
typedef struct Edge {
    int destination;
    int weight;
    struct Edge* next;
} Edge;

// Graph structure
typedef struct Graph {
    Edge* adjacencyList[MAX_NODES];
    int nodeCount;
} Graph;

// Function prototypes
void initGraph(Graph* graph, int nodes);
void addEdge(Graph* graph, int u, int v, int weight);
void dijkstra(Graph* graph, int source);
void bellmanFord(Graph* graph, int source);
void freeGraph(Graph* graph);
void printDivider();
int isValidNode(int node, int maxNodes);
void printSampleGraph();
void promptUserForGraph(Graph* graph);

int main() {
    Graph graph;
    int choice;

    printDivider();
    printf(MAGENTA "\nWelcome to the Weighted Graph \nAlgorithm Visualizer!\n" RESET);
    printDivider();

    while (1) {
        printf(BLUE "What would you like to do?\n" RESET);
        printf("1. Input a graph and find shortest paths.\n");
        printf("2. Use our Sample Graph to find shortest paths.\n");
        printf("3. Exit.\n");
        printf(BLUE"Enter your choice (1, 2, or 3): " RESET);
        scanf("%d", &choice);

        if (choice == 1) {
            promptUserForGraph(&graph);
        } else if (choice == 2) {
            // Initialize the sample graph
            initGraph(&graph, 5);
            addEdge(&graph, 0, 1, 10);
            addEdge(&graph, 0, 3, 5);
            addEdge(&graph, 1, 2, 1);
            addEdge(&graph, 2, 4, 4);
            addEdge(&graph, 3, 1, 3);
            addEdge(&graph, 3, 2, 9);
            addEdge(&graph, 3, 4, 2);
            addEdge(&graph, 4, 0, 7);
            addEdge(&graph, 4, 2, -2); // Negative weight edge

            // Display the graph
            printf(MAGENTA "Graph connections:\n" RESET);
            for (int i = 0; i < 5; i++) {
                Edge* edge = graph.adjacencyList[i];
                while (edge) {
                    printf("%d -> %d (%d)\n", i, edge->destination, edge->weight);
                    edge = edge->next;
                }
            }
            printDivider();

            // Choose a source node
            int source;
            printf(BLUE "Enter source node (0-4): " RESET);
            scanf("%d", &source);
            if (!isValidNode(source, graph.nodeCount)) {
                printf(RED "Invalid source node.\n" RESET);
                continue;
            }

// Algorithm selection loop with changing source option
            while (1) {
                printf(BLUE "Choose the algorithm:\n" RESET);
                printf("1. Dijkstra's Algorithm\n");
                printf("2. Bellman-Ford Algorithm\n");
                printf("3. Change Source Node\n");
                printf("4. Go back to main menu\n");
                printf(BLUE "Enter your choice (1, 2, 3, or 4): " RESET);
                scanf("%d", &choice);

                if (choice == 1) {
                    dijkstra(&graph, source);
                } else if (choice == 2) {
                    bellmanFord(&graph, source);
                } else if (choice == 3) {
                    printf(BLUE "Enter new source node (0-4): " RESET);
                    scanf("%d", &source);
                    if (!isValidNode(source, graph.nodeCount)) {
                        printf(RED "Invalid source node.\n" RESET);
                        continue;
                    }
                } else if (choice == 4) {
                    break; // Go back to main menu
                } else {
                    printf(RED "Invalid choice of algorithm.\n" RESET);
                }
            }
        } else if (choice == 3) {
            printf(GREEN "Exiting the program. Goodbye!\n" RESET);
            break; // Exit the loop
        } else {
            printf(RED "Invalid choice.\n" RESET);
        }
    }

    freeGraph(&graph); // Free allocated memory
    return 0;
}

void initGraph(Graph* graph, int nodes) {
    graph->nodeCount = nodes;
    for (int i = 0; i < nodes; i++) {
        graph->adjacencyList[i] = NULL;
    }
}

void addEdge(Graph* graph, int u, int v, int weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = v;
    newEdge->weight = weight;
    newEdge->next = graph->adjacencyList[u];
    graph->adjacencyList[u] = newEdge;
}

void dijkstra(Graph* graph, int source) {
    int distances[MAX_NODES];
    int visited[MAX_NODES] = {0};
    int prev[MAX_NODES] = {-1};

    for (int i = 0; i < graph->nodeCount; i++) {
        distances[i] = INT_MAX;
    }

    distances[source] = 0; // Start from the chosen source node

    // Check for negative weights in the graph
    for (int i = 0; i < graph->nodeCount; i++) {
        Edge* edge = graph->adjacencyList[i];
        while (edge != NULL) {
            if (edge->weight < 0) {
                printf(RED "Negative weight found. Dijkstra's algorithm can't work with negative weights.\n" RESET);
                return;
            }
            edge = edge->next;
        }
    }

    for (int count = 0; count < graph->nodeCount - 1; count++) {
        int minDistance = INT_MAX;
        int minIndex = -1;

        for (int v = 0; v < graph->nodeCount; v++) {
            if (!visited[v] && distances[v] <= minDistance) {
                minDistance = distances[v];
                minIndex = v;
            }
        }

        if (minIndex == -1) break; // No more reachable nodes

        visited[minIndex] = 1;

        Edge* edge = graph->adjacencyList[minIndex];
        while (edge != NULL) {
            if (!visited[edge->destination] && 
                distances[minIndex] != INT_MAX && 
                distances[minIndex] + edge->weight < distances[edge->destination]) {
                distances[edge->destination] = distances[minIndex] + edge->weight;
                prev[edge->destination] = minIndex; // Store the previous node for path
            }
            edge = edge->next;
        }
    }

    printDivider();
    printf(GREEN "Dijkstra's shortest paths from node %d (source):\n" RESET, source);
    printf("Vertex\tDistance from Source\tPath\n");
    for (int i = 0; i < graph->nodeCount; i++) {
        printf("%d\t\t", i);
        if (distances[i] == INT_MAX) {
            printf("Unreachable\t\t");
        } else {
            printf("%d\t\t\t", distances[i]);
        }

        // Print the path
        if (distances[i] != INT_MAX) {
            printf("Path: ");
            int j = i;
            // Print the path in reverse
            if (prev[j] != -1) {
                while (j != -1) {
                    printf("%d", j);
                    j = prev[j];
                    if (j != -1) {
                        printf(" <- ");
                    }
                }
            } else {
                printf("%d", source); // If source is the only node
            }
            printf("\n");
        } else {
            printf("No path\n");
        }
    }
    printDivider();
}
void bellmanFord(Graph* graph, int source) {
    int distances[MAX_NODES];
    int prev[MAX_NODES];

    for (int i = 0; i < graph->nodeCount; i++) {
        distances[i] = INT_MAX;
        prev[i] = -1;
    }
    distances[source] = 0;

    for (int i = 1; i < graph->nodeCount; i++) {
        for (int u = 0; u < graph->nodeCount; u++) {
            Edge* edge = graph->adjacencyList[u];
            while (edge != NULL) {
                if (distances[u] != INT_MAX && 
                    distances[u] + edge->weight < distances[edge->destination]) {
                    distances[edge->destination] = distances[u] + edge->weight;
                    prev[edge->destination] = u; // Track path
                }
                edge = edge->next;
            }
        }
    }

    // Check for negative weight cycles
    for (int u = 0; u < graph->nodeCount; u++) {
        Edge* edge = graph->adjacencyList[u];
        while (edge != NULL) {
            if (distances[u] != INT_MAX && 
                distances[u] + edge->weight < distances[edge->destination]) {
                printf(RED "Graph contains a negative weight cycle.\n" RESET);
                return;
            }
            edge = edge->next;
        }
    }

    printDivider();
    printf(CYAN "Bellman-Ford shortest paths from node %d (source):\n" RESET, source);
    printf("Vertex\tDistance from Source\tPath\n");
    for (int i = 0; i < graph->nodeCount; i++) {
        printf("%d\t\t", i);
        if (distances[i] == INT_MAX) {
            printf("Unreachable\t\t");
        } else {
            printf("%d\t\t\t", distances[i]);
        }

        // Print the path
        int j = i;
        if (distances[i] != INT_MAX) {
            printf("Path: ");
            while (prev[j] != -1) {
                printf("%d <- ", j);
                j = prev[j];
            }
            printf("%d\n", source);
        } else {
            printf("No path\n");
        }
    }
    printDivider();
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; i++) {
        Edge* edge = graph->adjacencyList[i];
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
}

void printDivider() {
    printf(YELLOW "--------------------------------------\n" RESET);
}

int isValidNode(int node, int maxNodes) {
    return node >= 0 && node < maxNodes;
}

void printSampleGraph() {
    printf(MAGENTA "Sample Graph (5 nodes: 0-1-2-3-4)\n" RESET);
    printf("       Edges:\n");
    printf("0 -> 1 (10)\n");
    printf("0 -> 3 (5)\n");
    printf("1 -> 2 (1)\n");
    printf("2 -> 4 (4)\n");
    printf("3 -> 1 (3)\n");
    printf("3 -> 2 (9)\n");
    printf("3 -> 4 (2)\n");
    printf("4 -> 0 (7)\n");
    printf("4 -> 2 (-2)\n");
    printDivider();
}
void promptUserForGraph(Graph* graph) {
    int nodes, edges, u, v, weight;

    printf(BLUE "Enter the number of nodes (max %d): " RESET, MAX_NODES);
    scanf("%d", &nodes);
    if (!isValidNode(nodes, MAX_NODES)) {
        printf(RED "Invalid number of nodes.\n" RESET);
        return;
    }
    initGraph(graph, nodes);

    printf(BLUE "Enter the number of edges: " RESET);
    scanf("%d", &edges);
    printf(YELLOW "Enter the details for each edge:\n" RESET);
    printf(YELLOW "Format: 'source_node' 'destination_node' 'weight'\n" RESET);
    printf(YELLOW "Example: 0 1 5 (indicates an edge from node 0 to node 1 with a weight of 5)\n" RESET);

    for (int i = 0; i < edges; i++) {
        printf(BLUE "Please enter edge %d: " RESET, i + 1);
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf(RED "Invalid input. Please enter three integers.\n" RESET);
            i--; // Repeat this iteration
            while (getchar() != '\n'); // Clear the invalid input
            continue; // Skip to the next iteration
        }
        if (isValidNode(u, nodes) && isValidNode(v, nodes)) {
            addEdge(graph, u, v, weight);
        } else {
            printf(RED "Invalid edge input. Ensure nodes are valid.\n" RESET);
            i--; // Repeat this iteration
        }
    }

    // Display the graph
    printf(MAGENTA "Graph connections:\n" RESET);
    for (int i = 0; i < nodes; i++) {
        Edge* edge = graph->adjacencyList[i];
        while (edge) {
            printf("%d -> %d (%d)\n", i, edge->destination, edge->weight);
            edge = edge->next;
        }
    }
    printDivider();

    // Choose a source node
    int source;
    printf(BLUE "Enter source node (0-%d): " RESET, nodes - 1);
    scanf("%d", &source);
    if (!isValidNode(source, nodes)) {
        printf(RED "Invalid source node.\n" RESET);
        return;
    }

    // Algorithm selection loop with changing source option
    while (1) {
        printf(BLUE "Choose the algorithm to use:\n" RESET);
        printf("1. Dijkstra's Algorithm\n");
        printf("2. Bellman-Ford Algorithm\n");
        printf("3. Change Source Node\n");
        printf("4. Go back to main menu\n");
        printf(BLUE "Enter your choice (1, 2, 3, or 4): " RESET);
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            dijkstra(graph, source);
        } else if (choice == 2) {
            bellmanFord(graph, source);
        } else if (choice == 3) {
            printf(BLUE "Enter new source node (0-%d): " RESET, nodes - 1);
            scanf("%d", &source);
            if (!isValidNode(source, nodes)) {
                printf(RED "Invalid source node.\n" RESET);
            }
        } else if (choice == 4) {
            return; // Go back to main menu
        } else {
            printf(RED "Invalid choice of algorithm.\n" RESET);
        }
    }
}