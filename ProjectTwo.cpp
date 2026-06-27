//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Sarah See
// Version     : 1.0
// Description : ABCU Advising Assistance Program
//               Uses a Binary Search Tree to store course information.
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure used to store course info
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node used by the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Constructor
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    // Recursively insert a course into the tree
    void addNode(Node* node, Course course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    // Traverse tree in order to print courses alphabetically
    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

public:
    // Constructor
    BinarySearchTree() {
        root = nullptr;
    }

    // Insert a course into the Binary Search Tre
    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    // Search for a corse by course number
    Course Search(string courseNumber) {
        Node* current = root;

        while (current != nullptr) {
            if (current->course.courseNumber == courseNumber) {
                return current->course;
            }
            else if (courseNumber < current->course.courseNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        Course emptyCourse;
        return emptyCourse;
    }

    // Start in-order traversal
    void InOrder() {
        inOrder(root);
    }
};

// Converts user input to uppercase to allow case-insensitive searches
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

// Read course information from a CSV file
void loadCourses(string fileName, BinarySearchTree* bst) {
    ifstream file(fileName);

    // Check that the file opened correctly
    if (!file.is_open()) {
        cout << "Error: File could not be opened." << endl;
        return;
    }

    string line;

    // Read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Separate line into comma-delimited values
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Each valid line must have at least course number and title
        if (tokens.size() < 2) {
            cout << "Invalid course format." << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        // Store prerequisite courses
        for (unsigned int i = 2; i < tokens.size(); i++) {
            course.prerequisites.push_back(tokens[i]);
        }

        // Add course to the tree
        bst->Insert(course);
    }

    file.close();
}

// Display course info and prerequisites
void printCourse(Course course) {
    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;
    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        for (unsigned int i = 0; i < course.prerequisites.size(); i++) {
            cout << course.prerequisites[i];

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

// Display user menu
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl;
}

// Main program execution starts here
int main() {
    // Create Binary Search Tree
    BinarySearchTree bst;

    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    // Continue displaying menu until user exits
    while (choice != 9) {
        displayMenu();

        cout << "What would you like to do? ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = 0;
        }

        // Load course data from file
        if (choice == 1) {
            cout << "Enter the file name: ";

            cin.ignore();
            getline(cin, fileName);

            loadCourses(fileName, &bst);
        }
        // Print all courses in alphanumeric order
        else if (choice == 2) {
            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            bst.InOrder();
        }

        // Search and display a specific course
        else if (choice == 3) {
            cout << "What course do you want to know about? ";
            cin >> courseNumber;

            courseNumber = toUpperCase(courseNumber);

            Course course = bst.Search(courseNumber);
            printCourse(course);
        }

        // Exit the program
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }

        // Handle invalid menu choices
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}