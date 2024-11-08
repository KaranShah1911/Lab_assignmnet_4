#include <bits/stdc++.h>

using namespace std;

// Brute force method to count inversions
int countInversionsBruteForce(const vector<int>& course_codes) {
    int inversions = 0;
    int n = course_codes.size();
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (course_codes[i] > course_codes[j]) {
                inversions++;
            }
        }
    }
    
    return inversions;
}

// Merge function used in merge sort to count inversions
int mergeAndCount(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
    int i = left;  // Starting index for left subarray
    int j = mid + 1;  // Starting index for right subarray
    int k = left;  // Starting index to be sorted
    int inv_count = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
            inv_count += (mid - i + 1);  // Increment inversion count
        }
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    for (i = left; i <= right; i++)
        arr[i] = temp[i];

    return inv_count;
}

// Divide and Conquer approach (Merge Sort) to count inversions
int mergeSortAndCount(vector<int>& arr, vector<int>& temp, int left, int right) {
    int mid, inv_count = 0;
    if (right > left) {
        mid = (right + left) / 2;

        inv_count += mergeSortAndCount(arr, temp, left, mid);
        inv_count += mergeSortAndCount(arr, temp, mid + 1, right);

        inv_count += mergeAndCount(arr, temp, left, mid, right);
    }
    return inv_count;
}

// Function to use divide and conquer to count inversions
int countInversionsOptimized(vector<int> course_codes) {
    vector<int> temp(course_codes.size());
    return mergeSortAndCount(course_codes, temp, 0, course_codes.size() - 1);
}

// Function to check if a course code is valid (positive integer)
bool isValidCourseCode(const string& code) {
    if (code.empty()) return false;
    // Ensure the course code is numeric and not negative
    for (char c : code) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to read the CSV file and parse student course choices
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> students;
    ifstream file(filename);
    string line;
    
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file " << filename << endl;
        return students;
    }
    
    // Skip the header
    getline(file, line);
    
    // Read each student's data
    int line_number = 2; // Start after the header
    
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> course_codes;
        string value;
        
        // Skip the first column (student ID)
        getline(ss, value, ',');

        if (value.empty()) {
            cout << "Error: Missing student ID at line " << line_number << ". Skipping this student's entry." << endl;
            line_number++;
            continue;
        }
        
        bool is_valid = true;
        
        // Read the course codes
        while (getline(ss, value, ',')) {
            // Trim leading/trailing spaces
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            course_codes.push_back(value);
        }
        
        if (course_codes.size() != 5) {  // Each student should have exactly 5 course codes
            cout << "Error: Inconsistent number of course codes at line " << line_number << ". Expected 5, but got " << course_codes.size() << ". Skipping this student's entry." << endl;
            is_valid = false;
        }
        
        if (is_valid) {
            students.push_back(course_codes);
        }
        
        line_number++;
    }
    
    file.close();
    return students;
}

int main() {
    // File path for the CSV file
    string file_path = "student_course_choices.csv";
    
    // Read the CSV file and load student course choices
    vector<vector<string>> students = readCSV(file_path);
    
    if (students.empty()) {
        cout << "No valid student course data available for processing." << endl;
        return 1;
    }

    // Map to store the count of students with a certain number of inversions (for both methods)
    map<int, int> inversion_count_map_brute_force;
    map<int, int> inversion_count_map_optimized;
    
    // Loop over each student and calculate inversions using both methods
    int student_number = 1;
    for (const auto& course_codes_str : students) {
        bool has_invalid_code = false;
        vector<int> course_codes;

        cout << "Student " << student_number << ": ";

        for (const string& code_str : course_codes_str) {
            if (!isValidCourseCode(code_str)) {
                cout << "Non-numeric or invalid course code '" << code_str << "' found. Skipping inversion calculation." << endl;
                has_invalid_code = true;
                break;
            }

            int code = stoi(code_str);
            if (code < 0) {
                cout << "Negative course code " << code << " found. Skipping inversion calculation." << endl;
                has_invalid_code = true;
                break;
            }

            course_codes.push_back(code);
        }

        if (!has_invalid_code) {
            int inversions_brute_force = countInversionsBruteForce(course_codes);
            int inversions_optimized = countInversionsOptimized(course_codes);

            // Print the inversion counts for each student (both methods)
            cout << "Brute-force inversions = " << inversions_brute_force
                 << ", Optimized inversions = " << inversions_optimized << endl;

            // Count inversions for brute-force method
            inversion_count_map_brute_force[inversions_brute_force]++;
            
            // Count inversions for optimized method
            inversion_count_map_optimized[inversions_optimized]++;
        }
        
        student_number++;
    }
    
    // Output the inversion count summary (brute force)
    cout << "\nSummary of Inversion Counts (Brute Force):" << endl;
    for (const auto& pair : inversion_count_map_brute_force) {
        cout << pair.first << " inversions: " << pair.second << " students" << endl;
    }

    // Output the inversion count summary (optimized approach)
    cout << "\nSummary of Inversion Counts (Optimized Divide-and-Conquer):" << endl;
    for (const auto& pair : inversion_count_map_optimized) {
        cout << pair.first << " inversions: " << pair.second << " students" << endl;
    }

    return 0;
}
