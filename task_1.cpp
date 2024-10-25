#include <bits/stdc++.h>
using namespace std;

// Structure to represent an item
struct Item {
    int weight;
    int value;
    int shelf_life;

    // Constructor to initialize an item
    Item(int w, int v, int l) : weight(w), value(v), shelf_life(l) {}

    // Function to calculate value-to-weight ratio
    double valuePerWeight() const {
        return (double)value / weight;
    }
};

// Comparison function to sort items based on shelf life first and then by value-to-weight ratio
bool compare(const Item &a, const Item &b) {
    if (a.shelf_life != b.shelf_life)
        return a.shelf_life < b.shelf_life;  // Sort by ascending shelf life
    return a.valuePerWeight() > b.valuePerWeight();  // If same shelf life, sort by value/weight ratio
}

// Function to validate the input items and capacity
string isValidInput(const vector<Item> &items, int capacity) {
    if (capacity < 0) {
        return "Capacity cannot be negative.";
    }

    bool allZeroValues = true;
    for (const auto &item : items) {
        if (item.weight <= 0) {
            return "Item weight cannot be zero or negative.";
        }
        if (item.value < 0) {
            return "Item value cannot be negative.";
        }
        if (item.shelf_life < 0) {
            return "Item shelf life cannot be negative.";
        }
        if (item.value > 0) {
            allZeroValues = false;
        }
    }

    if (allZeroValues) {
        return "All item values are zero.";
    }

    return "Valid";  // If all conditions pass
}

// Function to calculate the maximum value of items that can be carried using the greedy fractional knapsack approach
double fractionalKnapsackGreedy(vector<Item> &items, int capacity) {
    // Sort the items based on shelf life and value-to-weight ratio
    sort(items.begin(), items.end(), compare);

    double total_value = 0.0;  // Variable to store the maximum value

    for (const auto &item : items) {
        if (capacity - item.weight >= 0) {
            // If we can fit the whole item
            capacity -= item.weight;
            total_value += item.value;
        } else {
            // If we can only take a fraction of the item
            total_value += item.valuePerWeight() * capacity;
            break;  // Knapsack is full
        }
    }
    return total_value;
}

// Function to run a test case
void runTestCase(vector<Item> &items, int capacity) {
    string validation_message = isValidInput(items, capacity);
    if (validation_message != "Valid") {
        cout << "Invalid input: " << validation_message << endl;
        return;
    }

    // Calculate the maximum value using the greedy fractional knapsack approach
    double max_value_greedy = fractionalKnapsackGreedy(items, capacity);
    cout << "Maximum value in Knapsack = " << max_value_greedy << endl;
}

// Main function to test all cases
int main() {
    // -------------------- Negative Test Cases --------------------

    // Test case 1: All values are 0
    vector<Item> items1 = {
        Item(10, 0, 1), 
        Item(20, 0, 2),
        Item(30, 0, 3)
    };
    int capacity1 = 40;
    cout << "Test Case 1:" << endl;
    runTestCase(items1, capacity1);

    // Test case 2: One item with negative value
    vector<Item> items2 = {
        Item(5, 50, 1),
        Item(10, -40, 2),  // Negative value
        Item(25, 80, 3)
    };
    int capacity2 = 30;
    cout << "\nTest Case 2:" << endl;
    runTestCase(items2, capacity2);

    // Test case 3: One item with negative shelf life
    vector<Item> items3 = {
        Item(8, 50, -2),   // Negative shelf life
        Item(12, 100, 1),
        Item(7, 80, 3)
    };
    int capacity3 = 20;
    cout << "\nTest Case 3:" << endl;
    runTestCase(items3, capacity3);

    // Test case 4: All items with weight 0
    vector<Item> items4 = {
        Item(0, 70, 1),    // Weight 0
        Item(0, 60, 2),    // Weight 0
        Item(0, 80, 3)     // Weight 0
    };
    int capacity4 = 15;
    cout << "\nTest Case 4:" << endl;
    runTestCase(items4, capacity4);

    // Test case 5: Non-numeric weight simulation (handled externally)
    cout << "\nTest Case 5 (Non-numeric weight simulation):" << endl;
    cout << "Invalid input: Non-numeric weights are not allowed." << endl;

    // -------------------- Positive Test Cases --------------------

    // Test case 6: Knapsack can exactly fit a few items
    vector<Item> items6 = {
        Item(10, 60, 1), 
        Item(20, 100, 2),
        Item(30, 120, 3),
        Item(5, 80, 1),
        Item(15, 90, 2)
    };
    int capacity6 = 40;
    cout << "\nTest Case 6:" << endl;
    runTestCase(items6, capacity6);

    // Test case 7: Small capacity, can't take many items
    vector<Item> items7 = {
        Item(5, 50, 1),
        Item(10, 40, 2),
        Item(25, 80, 3),
        Item(15, 90, 1),
        Item(20, 60, 2)
    };
    int capacity7 = 30;
    cout << "\nTest Case 7:" << endl;
    runTestCase(items7, capacity7);

    // Test case 8: Exact fit for capacity
    vector<Item> items8 = {
        Item(8, 50, 2),
        Item(12, 100, 1),
        Item(7, 80, 3),
        Item(25, 90, 1),
        Item(10, 70, 2)
    };
    int capacity8 = 20;
    cout << "\nTest Case 8:" << endl;
    runTestCase(items8, capacity8);

    // Test case 9: Higher shelf life sorting matters
    vector<Item> items9 = {
        Item(6, 70, 1),
        Item(14, 60, 3),
        Item(12, 80, 2),
        Item(4, 40, 1),
        Item(9, 90, 2)
    };
    int capacity9 = 15;
    cout << "\nTest Case 9:" << endl;
    runTestCase(items9, capacity9);

    // Test case 10: Low capacity test
    vector<Item> items10 = {
        Item(3, 40, 1),
        Item(9, 30, 2),
        Item(5, 70, 3),
        Item(10, 60, 2),
        Item(6, 80, 1)
    };
    int capacity10 = 12;
    cout << "\nTest Case 10:" << endl;
    runTestCase(items10, capacity10);

    return 0;
}
