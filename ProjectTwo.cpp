// Author     :   Christian Delegas
// Date       :   02/16/2023
// Class      :   CS-300-T3310 DSA: Analysis and Design 23EW3
// Assignment :   Project 2: ABCU Course planner

#include <iostream>
#include <vector>
#include "CSVparser.hpp"
#include <cstring>

using namespace std;

// Used in load course data function

// Defines the structure for course objects
struct Course {
    string courseName;
    string courseNumber;
    vector<string> prereqs;
    Course() : prereqs() {
        courseName = "dummy";
        courseNumber = "00000";
    }
};

// Defines the structure for tree nodes
struct Node {
    Course course;
    Node* left;      // left child pointer
    Node* right;     // right child pointer

    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Constructor with data
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};



class BinarySearchTree {
private:
    // Private declarations for variables and helper functions
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void destroyBST(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseName);
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    destroyBST(root);
}

// Helper function for inOrder(), targets the root for the first node
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// This is a helper function for LoadCourses
void BinarySearchTree::Insert(Course course) {
    // If empty tree, new node is set to root
    if (root == nullptr) {
        root = new Node(course);
    }
    // Calls addNode to handle insert location
    else {
        this->addNode(root, course);
    }
}

Course BinarySearchTree::Search(string bidId) {
    Node* cur = root;
    // while there are still nodes to visit
    while (cur != nullptr) {
        // if node found, return node
        if (cur->course.courseName.compare(bidId) == 0) {
            return cur->course;
        }
        // if node key is higher than course name
        else if (cur->course.courseName.compare(bidId) > 0) {
            // traverse to left child
            cur = cur->left;
        }
        // if node key is lower than course name
        else if (cur->course.courseName.compare(bidId) < 0) {
            // traverse to right child
            cur = cur->right;
        }
    }
    // if nothing found, return a default course
    Course course;
    return course;
}

// Helper function for insert
void BinarySearchTree::addNode(Node* node, Course course) {

    // This if statement covers left child assignment and downward tree traversal
    if (node != nullptr && node->course.courseNumber.compare(course.courseNumber) > 0) {
        // adds node to left node if empty
        if (node->left == nullptr) {
            node->left = new Node(course);
            return;
        }
        // if not empty, recursively call addNode on left child
        else {
            this->addNode(node->left, course);
        }
    }
    // This else if statement covers right child assignment and downward tree traversal
    else if (node != nullptr && node->course.courseNumber.compare(course.courseNumber) < 0) {
        if (node->right == nullptr) {
            node->right = new Node(course);
            return;
        }
        // if not empty, recursively call addNode on right child
        else {
            this->addNode(node->right, course);
        }
    }
}

void displayCourse(Course course) {
    cout << course.courseNumber << " : " << course.courseName;
    for (int i = 0; i < course.prereqs.size(); ++i) {
        cout << " - " << course.prereqs.at(i);
    }
    cout << endl;
    return;
}

void BinarySearchTree::inOrder(Node* node) {
    // if the tree isn't empty
    if (node != nullptr) {
        // this traverses down the left tree side of the tree
        inOrder(node->left);
        displayCourse(node->course);
        inOrder(node->right);
    }
}

// This is used by the destructor
void BinarySearchTree::destroyBST(Node* node) {
    // if the node exists
    if (node != nullptr) {
        // this recursively seeks the the deepest left node
        destroyBST(node->left);
        // this checks the right subtree
        destroyBST(node->right);
        delete node;
    }
}



// *****NOTE***** To Professor
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);
    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Creates a data structure and add to the collection of courses
            Course course;

            // Validation step
            if (file[i].size() > 0) {
                course.courseNumber = file[i][0];
            }

            // Validation step
            if (file[i].size() > 1) {
                course.courseName = file[i][1];
            }

            // Loop to include prerequisites
            for (int j = 2; j < file[i].size(); j++) {
                course.prereqs.push_back(file[i][j]);
            }

            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void mainMenu() {
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    int choice = 0;
    string csvpath;
    string searchKey;

    cout << "Please specify the file" << endl;
    cin >> csvpath;
    while (choice != 4) {
        cout << "* * * M E N U * * *" << endl;
        cout << "Please select from the following options:" << endl;
        cout << "1. Load course data" << endl;
        cout << "2. Print alphanumeric list of all courses" << endl;
        cout << "3. Print specified course information" << endl;
        cout << "4. Exit" << endl;
        cin >> choice;

        switch (choice) {

        case 1:
            loadCourses(csvpath, bst);
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            cout << "Please enter the course name." << endl;
            std::getline(std::cin >> std::ws, searchKey);
            course = bst->Search(searchKey);
            if (course.courseName.compare("dummy") == 0) {
                cout << "Course not found" << endl;
                break;
            }
            else {                
                displayCourse(course);
            }
            break;

        case 4:

            return;
        }
    }
};

//********************** START OF THE MAIN METHOD **************************
int main() {
    // Menu
    mainMenu();
}
