// Parking Management System
// Assignment 4
// Project by Muizz Khan (F2023376059) and Aliza Zainab (F2023376320)
// QUESTION 1


#include <iostream>
using namespace std;

// Class to manage the entry and exit queue for the cars
class entryExit {
private:
    int* queue;  // Array to represent the entry queue
    int capacity;  // Maximum capacity of the entry queue
    int front, rear, count;  // Pointers for front and rear, and the current count of cars in the queue

public:
    // Constructor to initialize the queue with a given capacity
    entryExit(int cap) : capacity(cap), front(0), rear(-1), count(0) {
        queue = new int[capacity];  // Allocate memory for the queue
    }

    // Destructor to free the allocated memory for the queue
    ~entryExit() {
        delete[] queue;
    }

    // Check if the entry queue is empty
    bool isEmpty() {
        return count == 0;
    }

    // Check if the entry queue is full
    bool isFull() {
        return count == capacity;
    }

    // Add a car to the entry queue
    void enqueue(int value) {
        if (isFull()) {
            cout << "Entry queue is full! Cannot add car " << value << "." << endl;
        } else {
            rear = (rear + 1) % capacity;  // Circular increment of rear pointer
            queue[rear] = value;  // Add car to the queue
            count++;  // Increment the count
            cout << "Car " << value << " added to the entry queue." << endl;
        }
    }

    // Remove a car from the front of the entry queue
    int dequeue() {
        if (isEmpty()) {
            cout << "Entry queue is empty!" << endl;
            return -1;
        } else {
            int car = queue[front];  // Get the car at the front
            front = (front + 1) % capacity;  // Circular increment of front pointer
            count--;  // Decrease the count
            return car;
        }
    }

    // Display the status of the entry queue
    void display() {
        cout << "Entry Queue: ";
        for (int i = 0; i < count; i++) {
            cout << queue[(front + i) % capacity] << " ";  // Display each car in the queue
        }
        cout << endl;
    }
};

// Class to manage a floor stack (using stack behavior for parking)
class floorStack {
private:
    int* stack;  // Array to represent the floor's stack
    int capacity;  // Maximum capacity of the floor
    int top;  // Pointer to the top of the stack

public:
    // Constructor to initialize the stack with a given capacity
    floorStack(int cap) : capacity(cap), top(-1) {
        stack = new int[capacity];  // Allocate memory for the stack
    }

    // Destructor to free the allocated memory for the stack
    ~floorStack() {
        delete[] stack;
    }

    // Check if the floor stack is empty
    bool isEmpty() {
        return top == -1;
    }

    // Check if the floor stack is full
    bool isFull() {
        return top == capacity - 1;
    }

    // Add a car to the top of the floor stack
    void push(int car) {
        if (isFull()) {
            cout << "Floor is full! Cannot park car " << car << "." << endl;
        } else {
            stack[++top] = car;  // Increment top and add the car to the stack
        }
    }

    // Remove a car from the top of the floor stack
    int pop() {
        if (isEmpty()) {
            cout << "Floor is empty!" << endl;
            return -1;
        } else {
            return stack[top--];  // Return the top car and decrement top pointer
        }
    }

    // Function to remove a car from anywhere in the stack
    bool removeCar(int carNumber) {
        for (int i = 0; i <= top; i++) {
            if (stack[i] == carNumber) {
                // Shift cars to the left to fill the gap
                for (int j = i; j < top; j++) {
                    stack[j] = stack[j + 1];  // Shift each car left
                }
                top--;  // Decrease the top pointer
                cout << "Car " << carNumber << " removed from the floor." << endl;
                return true; // Car found and removed
            }
        }
        cout << "Car " << carNumber << " not found on this floor!" << endl;
        return false; // Car not found
    }

    // Display the status of the cars on the floor
    void display(int floorNumber) {
        cout << "Floor " << floorNumber << " Cars: ";
        for (int i = 0; i <= top; i++) {
            cout << stack[i] << " ";  // Display each car on the floor
        }
        cout << endl;
    }
};

// Class to manage the overflow waitlist for cars that cannot be parked immediately
class overflowWaitlist {
private:
    struct Node {  // Struct to represent a car in the waitlist
        int carNumber;
        Node* next;
        Node(int num) : carNumber(num), next(nullptr) {}
    };

    Node* head;  // Head of the waitlist (linked list)
    Node* tail;  // Tail of the waitlist (linked list)

public:
    // Constructor to initialize the waitlist
    overflowWaitlist() : head(nullptr), tail(nullptr) {}

    // Destructor to free the memory of the waitlist
    ~overflowWaitlist() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Check if the waitlist is empty
    bool isEmpty() {
        return head == nullptr;
    }

    // Add a car to the overflow waitlist
    void addToWaitlist(int carNumber) {
        Node* newNode = new Node(carNumber);  // Create a new node for the car
        if (tail == nullptr) {
            head = tail = newNode;  // If the list is empty, head and tail both point to the new node
        } else {
            tail->next = newNode;  // Add the new node to the end of the list
            tail = newNode;  // Update the tail pointer
        }
        cout << "Car " << carNumber << " added to the overflow waitlist." << endl;
    }

    // Remove a car from the waitlist
    int removeFromWaitlist() {
        if (head == nullptr) {
            cout << "Waitlist is empty!" << endl;
            return -1;
        }
        int carNumber = head->carNumber;  // Get the car at the front of the waitlist
        Node* temp = head;
        head = head->next;  // Move the head pointer to the next car
        if (head == nullptr) {
            tail = nullptr;  // If the list becomes empty, set tail to null
        }
        delete temp;  // Delete the old head node
        return carNumber;
    }

    // Display the cars in the waitlist
    void display() {
        cout << "Overflow Waitlist: ";
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->carNumber << " ";  // Display each car in the waitlist
            temp = temp->next;
        }
        cout << endl;
    }
};

// Main class to manage the entire parking management system
class ParkingManagementSystem {
private:
    floorStack** floors;  // Array of floor stacks representing each floor
    entryExit entryQueue;  // The entry queue for cars
    overflowWaitlist waitlist;  // The overflow waitlist for cars that cannot be parked immediately
    int floorCapacity;  // The maximum capacity for each floor
    int numFloors;  // The number of floors in the parking lot

public:
    // Constructor to initialize the parking management system
    ParkingManagementSystem(int numFloors, int floorCap, int queueCap)
        : entryQueue(queueCap), floorCapacity(floorCap), numFloors(numFloors) {
        floors = new floorStack*[numFloors];  // Allocate memory for the floors
        for (int i = 0; i < numFloors; ++i) {
            floors[i] = new floorStack(floorCap);  // Create each floor stack
        }
    }

    // Destructor to free the memory of the floors
    ~ParkingManagementSystem() {
        for (int i = 0; i < numFloors; ++i) {
            delete floors[i];  // Delete each floor stack
        }
        delete[] floors;  // Delete the array of floor pointers
    }

    // Function to add a car to the parking lot
    void addCar(int carNumber) {
        bool parked = false;

        // Try to park the car in the available floors
        for (int i = 0; i < numFloors; ++i) {
            if (!floors[i]->isFull()) {
                floors[i]->push(carNumber);  // Park the car on the current floor
                cout << "Car " << carNumber << " parked on Floor " << i + 1 << "." << endl;
                parked = true;
                break;
            }
        }

        // If the parking lot is full, add to the entry queue or waitlist
        if (!parked) {
            if (!entryQueue.isFull()) {
                entryQueue.enqueue(carNumber);  // Add to entry queue if space available
            } else {
                waitlist.addToWaitlist(carNumber);  // Otherwise, add to waitlist
            }
        }

        // After adding a car to any floor, move cars from entry queue if space is available
        moveCarsFromEntryQueue();
    }

    // Function to remove a car from the parking lot
    void removeCar(int carNumber) {
        bool found = false;

        // Try to remove a car from any floor
        for (int i = numFloors - 1; i >= 0; --i) {
            if (!floors[i]->isEmpty()) {
                found = floors[i]->removeCar(carNumber);  // Try to remove the car from the floor
                if (found) {
                    break;
                }
            }
        }

        if (!found) {
            cout << "Car " << carNumber << " not found in the parking lot." << endl;
        }

        // After removing a car, move cars from the entry queue if space is available
        moveCarsFromEntryQueue();

        // After removing a car, if space is available, move a car from the waitlist to the entry queue
        if (!waitlist.isEmpty()) {
            int carFromWaitlist = waitlist.removeFromWaitlist();
            entryQueue.enqueue(carFromWaitlist);  // Add car from waitlist to the entry queue
            cout << "Car " << carFromWaitlist << " moved from waitlist to entry queue." << endl;
        }
    }

    // Function to move cars from the entry queue to the floors when space is available
    void moveCarsFromEntryQueue() {
        // Check if any floor has space and move cars from the entry queue
        for (int i = 0; i < numFloors; ++i) {
            while (!floors[i]->isFull() && !entryQueue.isEmpty()) {
                int carToPark = entryQueue.dequeue();  // Remove car from entry queue
                floors[i]->push(carToPark);  // Park car on the floor
                cout << "Car " << carToPark << " moved from entry queue to Floor " << i + 1 << "." << endl;
            }
        }
    }

    // Function to expand the parking lot by adding new floors
    void expandParking(int newFloors) {
        floorStack** newArr = new floorStack*[numFloors + newFloors];  // Create a new array of floor pointers
        for (int i = 0; i < numFloors; ++i) {
            newArr[i] = floors[i];  // Copy the old floors into the new array
        }
        for (int i = 0; i < newFloors; ++i) {
            newArr[numFloors + i] = new floorStack(floorCapacity);  // Add new floor stacks
            cout << "Floor " << numFloors + i + 1 << " added to the parking lot." << endl;
        }
        delete[] floors;  // Delete the old array of floors
        floors = newArr;  // Point to the new array
        numFloors += newFloors;  // Update the total number of floors
    }

    // Function to display the current status of the parking lot
    void displayStatus() {
        cout << "Parking Lot Status:" << endl;
        for (int i = 0; i < numFloors; ++i) {
            floors[i]->display(i + 1);  // Display each floor's car status
        }
        entryQueue.display();  // Display entry queue
        waitlist.display();  // Display waitlist
    }
};

// Main function to interact with the system
int main() {
    int numFloors, floorCap, queueCap;

    // User input for the initial configuration
    cout << "Enter the number of floors in the parking lot: ";
    cin >> numFloors;

    cout << "Enter the capacity of each floor: ";
    cin >> floorCap;

    cout << "Enter the capacity of the entry queue: ";
    cin >> queueCap;

    // Create the parking management system
    ParkingManagementSystem parking(numFloors, floorCap, queueCap);

    int choice, carNumber, newFloors;
    while (true) {
        // Display the menu options
        cout << "\nParking Management System Menu:\n";
        cout << "1. Add Car\n";
        cout << "2. Remove Car\n";
        cout << "3. Expand Parking\n";
        cout << "4. Display Status\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter car number to add: ";
                cin >> carNumber;
                parking.addCar(carNumber);  // Add car to parking
                break;

            case 2:
                cout << "Enter car number to remove: ";
                cin >> carNumber;
                parking.removeCar(carNumber);  // Remove car from parking
                break;

            case 3:
                cout << "Enter number of new floors to add: ";
                cin >> newFloors;
                parking.expandParking(newFloors);  // Expand parking by adding new floors
                break;

            case 4:
                parking.displayStatus();  // Display the status of the parking system
                break;

            case 5:
                cout << "Exiting the system.\n";
                return 0;  // Exit the system

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
