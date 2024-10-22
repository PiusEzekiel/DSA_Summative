# DSA Summative

---

# Weighted Graph Algorithm Visualizer

## Overview
The Weighted Graph Algorithm Visualizer is a command-line application written in C that allows users to visualize and analyze the shortest paths in a weighted graph. The application implements Dijkstra's and Bellman-Ford algorithms to find shortest paths from a source node to other nodes in the graph.

## Features
- Input a custom graph and find shortest paths.
- Use a sample graph to visualize shortest paths.
- Choose between Dijkstra's and Bellman-Ford algorithms.
- Display the graph's connections and paths.
- Exit the application gracefully.


## Installation
### Compile the Program

    gcc q1_graph_visualizer.c -o q1_graph_visualizer
    

### Run the Program

    ./q1_graph_visualizer.c

## Usage Instructions
1. **Input a Graph**: Choose to input your own graph. You will be prompted to enter the number of nodes and edges, followed by the details for each edge in the format `source_node destination_node weight`.
2. **Use Sample Graph**: Select the option to use a predefined sample graph with 5 nodes.
3. **Choose Source Node**: After inputting or selecting a graph, specify the source node from which to find shortest paths.
4. **Select Algorithm**: Choose between Dijkstra's Algorithm and Bellman-Ford Algorithm for pathfinding.
5. **View Results**: The application displays distances from the source node and the corresponding paths.
6. **Exit**: Select the exit option to terminate the application.

---
# Huffman Coding for File Compression

## Overview
The Huffman Coding program is a command-line application written in C that implements the Huffman algorithm for file compression and decompression. This program allows users to compress text files into smaller sizes and decompress them back to their original form.

## Features
- **Compress a File**: Reduce the size of a text file using Huffman coding.
- **Decompress a File**: Restore a compressed file back to its original text.
- **File Size Comparison**: View the sizes of original and compressed files.

## Installation
1. **Compile the Program**:
    ```bash
    gcc q2_compression.c - q2_compression
    ```

2. **Run the Program**:
    ```bash
    ./q2_compression
    ```

## Usage Instructions
1. **Compress a File**:
    - Choose the option to compress a file.
    - Enter the path of the file to compress (e.g., `resources/text.txt`).
    - Specify the output path for the compressed file (e.g., `resources/compressed.txt`).

2. **Decompress a File**:
    - Choose the option to decompress a file.
    - Enter the path of the compressed file (e.g., `resources/compressed.txt`).
    - Specify the output path for the decompressed file (e.g., `resources/text.txt`).

3. **Exit**: Select the exit option to close the application.

---

# Criminal Tracking System

## Overview
The Criminal Tracking System is a command-line application written in C that uses graph theory to model relationships between individuals. It allows users to trace contacts of a specified criminal using breadth-first search (BFS). The system supports both a sample graph and dynamic graph creation.

## Features
- **Sample Graph**: Automatically loads a predefined set of people and their connections.
- **Dynamic Graph Creation**: Users can input their own set of people and connections.
- **Contact Tracing**: Uses BFS to find and display contacts of a specified criminal.

## Installation
1. **Compile the Program**:
    ```bash
    gcc criminal_tracking.c -o criminal_tracking
    ```

2. **Run the Program**:
    ```bash
    ./criminal_tracking
    ```

## Usage Instructions
1. **Use Sample Graph**: Select this option to view a predefined set of people and their connections. You can trace contacts from any specified individual.
2. **Create New Criminal Connections**: Enter your own set of people and their relationships. Then, you can trace contacts from any criminal.
3. **Exit**: Choose this option to exit the application.

---

# Dictionary Application

## Overview
The Dictionary Application is a command-line program written in C that allows users to manage a simple dictionary using a binary search tree (BST). Users can add words with their definitions, search for definitions, delete words, and display all words in alphabetical order. The dictionary is saved to and loaded from a text file.

## Features
- **Add Word**: Insert a new word with its definition into the dictionary.
- **Search Definition**: Look up the definition of a word.
- **Delete Word**: Remove a word and its definition from the dictionary.
- **Display All Words**: List all words along with their definitions in alphabetical order.
- **Persistent Storage**: Save the dictionary to a file and load it on startup.

## Installation
1. **Compile the Program**:
    ```bash
    gcc dictionary.c -o dictionary
    ```

2. **Run the Program**:
    ```bash
    ./dictionary
    ```

## Usage Instructions
1. **Add Word**: Select option 1 to enter a new word and its definition.
2. **Search Definition**: Select option 2 to find the definition of a specific word.
3. **Delete Word**: Select option 3 to remove a word from the dictionary.
4. **Display All Words**: Select option 4 to view all entries in alphabetical order.
5. **Exit**: Select option 5 to save the dictionary to a file and exit the program.

---

# Student Records Management System

## Overview
The Student Records Management System is a command-line application written in C that allows users to manage student records efficiently using a Red-Black Tree data structure. Users can add, search, update, delete, and display student information, including ID, name, and grades. The system supports persistent storage by saving records to a file.

## Features
- **Add Student**: Insert a new student record with ID, name, and grades.
- **Search Student**: Look up student details by ID.
- **Update Student Record**: Modify existing student information.
- **Delete Student Record**: Remove a student record by ID.
- **Display All Students**: List all students in alphabetical order by name.
- **Persistent Storage**: Save records to a file and load them on startup.

## Installation
1. **Compile the Program**:
    ```bash
    gcc student_records.c -o student_records
    ```

3. **Run the Program**:
    ```bash
    ./student_records
    ```

## Usage Instructions
1. **Add Student**: Select option 1 to enter a new student's ID, name, and grades.
2. **Search Student by ID**: Select option 2 to find a student by their ID.
3. **Update Student Record**: Select option 3 to modify an existing student's information.
4. **Delete Student Record**: Select option 4 to remove a student from the records.
5. **Display All Students**: Select option 5 to view a list of all students.
6. **Exit**: Select option 6 to save the records to a file and exit the program.

---

# To-Do List Manager

## Overview
The To-Do List Manager is a command-line application written in C that allows users to manage their tasks efficiently. Users can add, remove, update, and mark tasks as completed, as well as display all tasks. The application supports persistent storage by saving tasks to a file.

## Features
- **Add Task**: Create a new task with a description.
- **Remove Task**: Delete a task by its ID.
- **Update Task**: Modify the description of an existing task.
- **Mark Task as Completed**: Change the status of a task to completed.
- **Remove Completed Tasks**: Delete all tasks that have been marked as completed.
- **Display All Tasks**: View a list of all tasks along with their status.
- **Persistent Storage**: Save tasks to a file and load them on startup.

## Installation
1. **Compile the Program**:
    ```bash
    gcc todo_list_manager.c -o todo_list_manager
    ```

2. **Run the Program**:
    ```bash
    ./todo_list_manager
    ```

## Usage Instructions
1. **Add Task**: Select option 1 to enter a new task's description.
2. **Remove Task**: Select option 2 to delete a task by its ID.
3. **Update Task**: Select option 3 to modify an existing task's description.
4. **Mark Task as Completed**: Select option 4 to mark a task as completed.
5. **Remove Completed Tasks**: Select option 5 to delete all completed tasks.
6. **Display All Tasks**: Select option 6 to view all tasks and their statuses.
7. **Exit**: Select option 7 to save tasks to a file and exit the program.
