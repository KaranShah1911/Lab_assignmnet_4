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

// Comparison function to sort items based on value-to-weight ratio
bool compare(const Item &a, const Item &b) {
    return a.valuePerWeight() > b.valuePerWeight();
}

// Function to calculate the maximum value of items that can be carried using the greedy approach
double fractionalKnapsackGreedy(vector<Item> &items, int capacity) {
    // Sort the items based on value-to-weight ratio in descending order
    sort(items.begin(), items.end(), compare);

    double total_value = 0.0; // Variable to store the maximum value

    for (const auto &item : items) {
        if (capacity - item.weight >= 0) {
            // If we can fit the whole item
            capacity -= item.weight;
            total_value += item.value;
        } else {
            // If we can only take a fraction of the item
            total_value += item.valuePerWeight() * capacity;
            break; // Knapsack is full
        }
    }
    return total_value;
}

// Helper function to calculate maximum value using brute force
double knapsackBruteForce(const vector<Item> &items, int n, int capacity) {
    // Base case: no items or no remaining capacity
    if (n == 0 || capacity == 0)
        return 0;

    // If the weight of the nth item is more than the knapsack capacity, skip this item
    if (items[n-1].weight > capacity)
        return knapsackBruteForce(items, n-1, capacity);

    // Case 1: nth item is included (either fully or fractionally)
    double value_included;
    if (items[n-1].weight <= capacity)
        value_included = items[n-1].value + knapsackBruteForce(items, n-1, capacity - items[n-1].weight);
    else
        value_included = (double)items[n-1].value * capacity / items[n-1].weight;

    // Case 2: nth item is excluded
    double value_excluded = knapsackBruteForce(items, n-1, capacity);

    // Return the maximum of the two cases
    return max(value_included, value_excluded);
}

// Main function to test both algorithms
int main() {
    // Create a list of items (weight, value, shelf life)
    vector<Item> items = {
        Item(50, 100, 10), 
        Item(30, 120, 5),
        Item(60, 40, 20),
        Item(50, 70, 10),
        Item(90, 300, 1),
        Item(80, 200, 4)
    };

    int capacity = 200; // Vehicle capacity (in tons)

    // Calculate the maximum value using the greedy approach
    double max_value_greedy = fractionalKnapsackGreedy(items, capacity);
    cout << "Maximum value in Knapsack (Greedy) = " << max_value_greedy << endl;

    // Calculate the maximum value using the brute force approach
    double max_value_bruteforce = knapsackBruteForce(items, items.size(), capacity);
    cout << "Maximum value in Knapsack (Brute Force) = " << max_value_bruteforce << endl;

    return 0;
}
