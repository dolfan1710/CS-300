//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Joe Silva
// Version     : 1.0
// Description : Project Two
//============================================================================

#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//Structure to hold course information
struct Course {
    string courseId; 
    string courseName;
    vector<string> prerequisites;
    
    //Default constructor
    Course() {
        courseId = "";
		courseName = "";
		prerequisites.clear();
	}
};

//Structure to hold node information
struct Node {
    Course course;
    Node* left;
    Node* right;

    //Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //Contructor with course
    Node(Course aCourse) :
        Node() {
        this->course = aCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;
    void inOrder(Node* node);
    void addNode(Node* node, Course course);

public:
    Node* getRoot();
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void DestroyTree(Node* node);
    void InOrder();
    void Insert(Course course);
    void clear();
    Course Search(string courseId);
};

//Default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

//Returns root
Node* BinarySearchTree::getRoot() {
	return root;
}   

//Destructor
BinarySearchTree::~BinarySearchTree() {
    DestroyTree(root);
    root = nullptr;
}

//Deletes binary search tree
void BinarySearchTree::DestroyTree(Node* node) {
    if (node == nullptr) {
		return;
    }

    //Recursively delete left and right nodes
    DestroyTree(node->left);
    DestroyTree(node->right);
    delete node;
}

//Inserts course into the binary search tree
void BinarySearchTree::Insert(Course course) {
    
    //Checks if root is empty, if it is, it creates a new node
    if (root == nullptr) {
        root = new Node(course);
    }

    //Otherwise, it calls the private addNode function
    else {
        this->addNode(root, course);
    }
}

//Prints the binary search tree
void BinarySearchTree::InOrder() {
    this->inOrder(root);
}

//Adds a new node to the binary search tree
void BinarySearchTree::addNode(Node* node, Course course) {
    if (node->course.courseId.compare(course.courseId) > 0) {

        //Check to see if there is a left node
        if (node->left == nullptr) {

            //If there is no left node, this node becomes left
            node->left = new Node(course);
        }
        else {
            //Else recurse down the left node and add the course to the left node
            this->addNode(node->left, course);
        }
    }

    //Otherwise the course is greater than the current node
    else {

        // If there is no right node, this node becomes right
        if (node->right == nullptr) {
            node->right = new Node(course);
        }

        //Else recurse down the right node and add the course to the right node
        else {
            this->addNode(node->right, course);
        }
    }
}

//Prints the binary search tree
void BinarySearchTree::inOrder(Node* node) {
    // The root node is passed in from the public InOrder function, that is where we start
    // If the node is not null
    if (node != nullptr) {

        //Traverse the left subtree
        inOrder(node->left);

        //Output the course
        cout << node->course.courseId << "\, " << node->course.courseName << endl;
        
        //Traverse the right subtree
        inOrder(node->right);
    }
}

//Searches for a course and returns it
Course BinarySearchTree::Search(string courseId) {

    //Create a pointer current node and set it to root
    Node* currentNode = root;

    //Create a loop to traverse the tree, if the node we are searching for is root it will be found  
    //and returned the first iteration
    while (currentNode != nullptr) {

        //Check to see if the current node matches the course we are seacrhing for and return
        //it if we have a match
        if (currentNode->course.courseId.compare(courseId) == 0) {
            return currentNode->course;
        }

		//Check to see if the course we are searching for is less than the current node
		//If it is, recurse down the left subtree
        if (currentNode->course.courseId.compare(courseId) > 0) {
            currentNode = currentNode->left;
        }

        //Otherwise recurse down the right subtree
        else {
            currentNode = currentNode->right;
        }
    }
    //If the course is not found, return an empty course
    if (currentNode == nullptr) {
        return Course();
    }
}

//Clears the binary search tree
void BinarySearchTree::clear() {
    DestroyTree(root);
    root = nullptr;
}

//Validates the course ID
bool validateCourseId(string courseId) {
    bool courseIdValid = false;

    //Check to see if the course id is 7 characters
    if (courseId.length() != 7) {
        return false;
    }

    //Check to see if the first 4 characters are letters
    for (int i = 0; i < 4; i++) {
        if (isalpha(courseId[i])) {
        }
        else {
            return false;
        }
    }

    //Check to see if the last 3 characters are numbers
    for (int j = 4; j < 7; j++) {
        if (isdigit(courseId[j])) {
            courseIdValid = true;
        }
        else {
            return false;
        }
    }
    return courseIdValid;
}

//Prints bad lines from the course list
void printBadLines(vector<string>& badlines) {
    int lineCount = 1;
    cout << "\nBad lines found, missing course id or course name, check the input file!\n" << endl;
    
    //Loops through the badLines vector and prints the bad lines
    for (int i = 0; i < badlines.size(); i++) {
        size_t pos = 0;

        //Replaces the commas with spaces
        while ((pos = badlines[i].find(",")) != std::string::npos) {
            badlines[i].replace(pos, 1, " ");
            pos += 1;
        }

        //displays the bad line
        cout << "Bad line " << lineCount << ": " << badlines[i] << endl;
        lineCount++;
    }
}

//Generate the course list
void generateCourseList(vector<string>& allPrerequisites, BinarySearchTree* tree, string filePath){
    vector<string> prerequisites;
    vector<string> badlines;
    vector<string> allPrerequisites;    

    string line;
    string prerequisite;
    string error;
    string noIdMesaage = "\033[31mCourse ID incorrect or Missing!\033[0m";
    string noNameMessage = "\033[31mCourse Name Missing!\033[0m";

    //Use the file path to open the file
    ifstream file(filePath);

    //Loops through the file and adds the course to the binary search tree
    if(file){
		while (getline(file, line)) {
            Course course;
            istringstream iss(line);
            getline(iss, course.courseId, ',');

			//Checks if the course id is empty and that it is valid (length 7, first 3 letters are letters, last 4 are numbers)
            if (course.courseId == "" || course.courseId.length() == 0 || !validateCourseId(course.courseId)) {
                error = line + " |" + noIdMesaage;
                //Pushes a bad line to the badlines vector
                badlines.push_back(error);
                continue;
            }

getline(iss, course.courseName, ',');

//Checks if the course name is empty
if (course.courseName == "" || course.courseName.length() == 0) {
    error = line + " |" + noNameMessage;

    //Pushes a bad line to the badlines vector
    badlines.push_back(error);
    continue;
}

//Loops through the prerequisites and adds them to the prerequisites vector
//This vector is used to verify that the prerequisites are in the course catalog
//We won't be able to handle the case where they are in the catalog but incorrect
while (getline(iss, prerequisite, ',')) {
    if (!prerequisite.empty()) {
        prerequisites.push_back(prerequisite);
        allPrerequisites.push_back(prerequisite);
    }
}

//Inserts the course into the binary search tree
course.prerequisites = prerequisites;
tree->Insert(course);

//Clears the prerequisites
prerequisites.clear();
        }
    }

    //Post an error if the file failed to open
    else {
        cerr << "Failed to open the file!" << endl;
    }

    //Prints the bad lines to the screen
    if (badlines.size() > 0) {
        printBadLines(badlines);
    }

    //Returns the vector of all prerequisites
    
}

//Validates the prerequisites, returns a vector of prerequisites that are not in the course catalog
void checkPrerequisites(vector<string>& missingPrerequisites, vector<string>& allPrerequisites, BinarySearchTree* tree) {

    //Loops through the prerequisites and checks if they are in the course catalog
    vector<string> missingPrerequisites;
    for (int i = 0; i < allPrerequisites.size(); i++) {

        //Validate the course ID numbers of the prerequisites
        if (!validateCourseId(allPrerequisites[i])) {
            missingPrerequisites.push_back(allPrerequisites[i]);
        }

        //If the course ID is valid, check if it is in the course catalog
        else {
            //Leverages the search function
            Course course = tree->Search(allPrerequisites[i]);
            //If the course is not found, add it to the missingPrerequisites vector
            if (course.courseId.empty()) {
                missingPrerequisites.push_back(allPrerequisites[i]);
            }
        }
    }

    //Returns the vector of missing prerequisites
    
}

void printMissingPrerequisites(vector<string>& missingPrerequisites) {

    //Prints the missing prerequisites, change color to red
    cout << "\n\033[31mInvalid prerequisites found!\033[0m\n" << endl;
    cout << "These Prerequisites are either invalid or not in the course catalog: ";

    //Loops through the missingPrerequisites vector and prints the missing prerequisites in red
    for (int i = 0; i < missingPrerequisites.size(); i++) {
        cout << "\033[31m" << missingPrerequisites[i] << "\033[0m";
        if (i < missingPrerequisites.size() - 1) {
            cout << ", ";
        }
    }

    //Prompts the user to try a different file
    cout << "\nCheck the input file and try again!\n" << endl;
}

//Prints the searched course to the console
void printSearchedCourse(string selectedCourse, BinarySearchTree* tree) {

    //Converts the selected course to uppercase
    for (int i = 0; i < selectedCourse.length(); i++) {
        selectedCourse[i] = toupper(selectedCourse[i]);
    }

    //Check to see if the selected course is valid
    if (!validateCourseId(selectedCourse)){
		cout << "\n\033[31mInvalid course ID!\033[0m\n" << endl;
		return;
    }

    //Search for the iputted course
    Course course = tree->Search(selectedCourse);

    //Handle the case where the course if not found
    if (course.courseId.empty()) {
		cout << "\n\033[31mCourse not found!!\033[0m\n" << endl;
		return;             
        }

    cout << "\n" << course.courseId << " " << course.courseName << endl;

    //Prints and formats the prerequisites
    if (course.prerequisites.size() > 0) {
        cout << "Prerequisites: ";
        for (int i = 0; i < course.prerequisites.size(); i++) {

            //Prerequisites are printed in green
            cout << "\33[32m" << course.prerequisites[i] << "\033[0m";
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << "\n" << endl;
    }

    //Prints a message if there are no prerequisites
    else {
        cout << "No prerequisites!!\n" << endl;
    }
}

int main() {
    BinarySearchTree* tree = new BinarySearchTree();
    
    vector<string> allPrerequisites;
    vector<string> missingPrerequisites;
    
    string filePath = "C:\\Testing\\courses.csv";
	string selectedCourse;
    string choice = "0";

    cout << "\nWelcome to the Course Planner!\n" << endl;

    //Starts the user menu, changed to if/else structure to avoid infinite loop with bad input
    while (choice != "9"){
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit\n" << endl;
        cout << "What would you like to do: ";
        cin >> choice;
        
        if (choice == "1") {

            //When loading courses we first check to see if the tree exists, if it does we clear it
            //This way the user can fix their input file and retry
            Node* root = tree->getRoot();
            if (root != nullptr) {
                cout << "\nThe data structure was already loaded!" << endl;
				cout << "Clearing all data!\n" << endl;
                tree->clear();
               
            }

            //Generates the course list and checks for missing prerequisites
            generateCourseList(allPrerequisites, tree, filePath);
            checkPrerequisites(missingPrerequisites, allPrerequisites, tree);
            if (missingPrerequisites.size() != 0) {
                printMissingPrerequisites(missingPrerequisites);
            }
        }

        //Prints the course list in alphanumeric order
        else if (choice == "2") {
            cout << "\nHere is a sample schedule:\n" << endl;
            tree->InOrder();
			cout << endl;
        }
        
        //Prints a specific course
        else if (choice == "3") {
            cout << "What course do you want to know about: ";
            cin >> selectedCourse;
            printSearchedCourse(selectedCourse, tree);
        }

        //Exits the program
        else if (choice == "9") {
			break;
		}

		else{
            cout << choice << " is not a valid option\n" << endl;
        }
    }

    cout << "Thank you for using Course Planner !" << endl;
   	return 0;
}
