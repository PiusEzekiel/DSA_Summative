#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define YELLOW "\033[33m"
#define FILENAME "resources/tasks.txt"

typedef struct Task {
    int id;
    char description[256];
    int completed; // 0 for not completed, 1 for completed
    struct Task* next;
} Task;

Task* head = NULL; // Head of the linked list
int taskIdCounter = 1; // Counter for task IDs

// Function prototypes
void addTask(const char* description);
void removeTask(int id);
void updateTask(int id, const char* newDescription);
void markTaskCompleted(int id);
void removeCompletedTasks();
void displayTasks();
Task* findTask(int id);
void freeTasks();
void printLine(const char* color, const char* text);
void printMenu();
void saveTasks();
void loadTasks();
int isValidDescription(const char* description);
int isValidID(int id);

int main() {
    loadTasks(); // Load tasks from file at startup

    int choice;
    char description[256];
    int id;

    while (1) {
        printMenu();
        printf(BLUE "Choose an option: " RESET);
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                printf(BLUE "\nEnter task description: " RESET);
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = 0; // Remove newline

                if (!isValidDescription(description)) {
                    printf(RED "Invalid description. Please try again.\n" RESET);
                    break;
                }
                addTask(description);
                break;

            case 2:
                printf(BLUE "Enter task ID to remove: " RESET);
                scanf("%d", &id);
                if (!isValidID(id)) {
                    printf(RED "Invalid ID. Please enter a valid task ID.\n" RESET);
                    break;
                }
                removeTask(id);
                break;

            case 3:
                printf(BLUE "Enter task ID to update: " RESET);
                scanf("%d", &id);
                if (!isValidID(id)) {
                    printf(RED "Invalid ID. Please enter a valid task ID.\n" RESET);
                    break;
                }
                printf(BLUE "Enter new description: " RESET);
                getchar(); // Consume newline
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = 0; // Remove newline

                if (!isValidDescription(description)) {
                    printf(RED "Invalid description. Please try again.\n" RESET);
                    break;
                }
                updateTask(id, description);
                break;

            case 4:
                printf(GREEN "Enter task ID to mark as completed: " RESET);
                scanf("%d", &id);
                if (!isValidID(id)) {
                    printf(RED "Invalid ID. Please enter a valid task ID.\n" RESET);
                    break;
                }
                markTaskCompleted(id);
                break;

            case 5:
                removeCompletedTasks();
                break;

            case 6:
                displayTasks(); // Show all tasks, both completed and uncompleted
                break;

            case 7:
                saveTasks();
                freeTasks();
                printf(RED "Exiting...\n" RESET);
                return 0;

            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}

// Print a decorative line
void printLine(const char* color, const char* text) {
    printf("%s%s%s\n", color, text, RESET);
}

// Print the menu
void printMenu() {
    printLine(CYAN, "=========================");
    printf(MAGENTA "       To-Do List Manager\n" RESET);
    printLine(CYAN, "=========================");
    printf(MAGENTA "1. Add Task\n" RESET);
    printf(MAGENTA "2. Remove Task\n" RESET);
    printf(MAGENTA "3. Update Task\n" RESET);
    printf(MAGENTA "4. Mark Task as Completed\n" RESET);
    printf(MAGENTA "5. Remove Completed Tasks\n" RESET);
    printf(MAGENTA "6. Display All Tasks\n" RESET);
    printf(MAGENTA "7. Exit\n" RESET);
    printLine(CYAN, "=========================");
}

// Add a new task
void addTask(const char* description) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->id = taskIdCounter++;
    strncpy(newTask->description, description, sizeof(newTask->description));
    newTask->completed = 0; // Not completed
    newTask->next = head; // Insert at the beginning
    head = newTask;
    printf(GREEN "Task added with ID: %d\n" RESET, newTask->id);
}

// Remove a task by ID
void removeTask(int id) {
    Task *current = head, *previous = NULL;

    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf(RED "Task with ID %d not found.\n" RESET, id);
        return;
    }

    if (previous == NULL) {
        head = current->next; // Remove head
    } else {
        previous->next = current->next; // Bypass the current task
    }

    free(current);
    printf(GREEN "Task with ID %d removed.\n" RESET, id);
}

// Update a task's description
void updateTask(int id, const char* newDescription) {
    Task* task = findTask(id);
    if (task) {
        strncpy(task->description, newDescription, sizeof(task->description));
        printf(GREEN "Task %d updated.\n" RESET, id);
    } else {
        printf(RED "Task with ID %d not found.\n" RESET, id);
    }
}

// Mark a task as completed
void markTaskCompleted(int id) {
    Task* task = findTask(id);
    if (task) {
        task->completed = 1;
        printf(GREEN "Task %d marked as completed.\n" RESET, id);
        // removeCompletedTasks(); // Automatically remove completed tasks
    } else {
        printf(RED "Task with ID %d not found.\n" RESET, id);
    }
}

// Remove completed tasks from the list
void removeCompletedTasks() {
    Task *current = head, *previous = NULL;

    while (current != NULL) {
        if (current->completed) {
            if (previous == NULL) {
                head = current->next; // Removing head
                free(current);
                current = head;
            } else {
                previous->next = current->next; // Bypass current task
                free(current);
                current = previous->next;
            }
            printf(GREEN "Completed task removed.\n" RESET);
        } else {
            previous = current;
            current = current->next;
        }
    }
}

// Display all tasks (both completed and uncompleted)
void displayTasks() {
    Task* current = head;
    if (current == NULL) {
        printf(RED "No tasks available.\n" RESET);
        return;
    }

    printLine(CYAN, "Your Tasks:");
    printf(MAGENTA "%-5s | %-30s | %-10s\n", "ID", "Description", "Status" RESET);
    printLine(CYAN, "---------------------------------------------------");
    
    while (current != NULL) {
        printf(YELLOW"%-5d | %-30s | %-10s\n",
               current->id, current->description,
               current->completed ? "Completed" : "Not Completed" RESET);
        printLine(CYAN, "---------------------------------------------------");
        current = current->next;
    }
}

// Find a task by ID
Task* findTask(int id) {
    Task* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Not found
}

// Free all tasks
void freeTasks() {
    Task* current = head;
    while (current != NULL) {
        Task* next = current->next;
        free(current);
        current = next;
    }
}

// Save tasks to a file
void saveTasks() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf(RED "Error: Could not open file to save tasks.\n" RESET);
        return;
    }

    Task* current = head;
    while (current != NULL) {
        fprintf(file, "%d|%s|%d\n", current->id, current->description, current->completed);
        current = current->next;
    }

    fclose(file);
    printf(GREEN "Tasks saved to %s successfully.\n" RESET, FILENAME);
}

// Load tasks from a file
void loadTasks() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf(YELLOW "No existing tasks found. Starting fresh.\n" RESET);
        return;
    }

    Task tempTask;
    while (fscanf(file, "%d|%255[^|]|%d\n", &tempTask.id, tempTask.description, &tempTask.completed) == 3) {
        Task* newTask = (Task*)malloc(sizeof(Task));
        *newTask = tempTask; // Copy the temp task to new task
        newTask->next = head;
        head = newTask;
        taskIdCounter = (tempTask.id >= taskIdCounter) ? tempTask.id + 1 : taskIdCounter; // Update taskIdCounter
    }

    fclose(file);
    printf(GREEN "Tasks loaded from %s successfully.\n" RESET, FILENAME);
}

// Validate task description
int isValidDescription(const char* description) {
    return strlen(description) > 0; // Description cannot be empty
}

// Validate task ID
int isValidID(int id) {
    return id > 0; // ID must be a positive integer
}