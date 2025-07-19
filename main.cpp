// Library imports
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;
// Color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[38;2;139;27;58m"   
#define COLOR_YELLOW  "\033[38;2;220;188;65m"  
#define COLOR_GREEN   "\033[38;2;111;224;193m" 

// Incident structure 
struct Incident {
    string area;
    string type;
};

// Function to trim whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Read incidents from file
vector<Incident> read_file(const string& filename) {
    vector<Incident> incidents;
    ifstream fin(filename);
    string line;
    while (getline(fin, line)) {
        size_t sep = line.find('|');
        if (sep != string::npos) {
            Incident inc;
            inc.area = trim(line.substr(0, sep));
            inc.type = trim(line.substr(sep + 1));
            incidents.push_back(inc);
        }
    }
    return incidents;
}

// Write a new incident to file
void write_file(const string& filename, const Incident& incident) {
    ofstream fout(filename, ios::app);
    fout << incident.area << "|" << incident.type << endl;
}

// Display all incidents
void display_incidents(const vector<Incident>& incidents) {
    cout << "\n" << COLOR_CYAN << "Reported Incidents:" << COLOR_RESET << endl;
    int idx = 1;
    for (const auto& inc : incidents) {
        cout << idx++ << ". " << COLOR_YELLOW << inc.area << COLOR_RESET << " - "
             << COLOR_GREEN << inc.type << COLOR_RESET << endl;
    }
    if (incidents.empty()) {
        cout << "No incidents found." << endl;
    }
}

// Filter incidents by area or type
void filter_incidents(const vector<Incident>& incidents) {
    cout << "\nFilter by: 1. Area  2. Incident Type\nChoose option: ";
    int opt;
    cin >> opt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query;
    if (opt == 1) {
        cout << "Enter area to filter: ";
        getline(cin, query);
    } else if (opt == 2) {
        cout << "Enter incident type to filter: ";
        getline(cin, query);
    } else {
        cout << "Invalid option." << endl;
        return;
    }
    query = trim(query);
    int idx = 1;
    bool found = false;
    for (const auto& inc : incidents) {
        if ((opt == 1 && inc.area.find(query) != string::npos) ||
            (opt == 2 && inc.type.find(query) != string::npos)) {
            cout << idx++ << ". " << COLOR_YELLOW << inc.area << COLOR_RESET << " - "
                 << COLOR_GREEN << inc.type << COLOR_RESET << endl;
            found = true;
        }
    }
    if (!found) cout << "No matching incidents found." << endl;
}

// Input validation for non-empty string
string get_valid_input(const string& prompt) {
    string input;
    do {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (input.empty()) cout << "Input cannot be empty. Please try again.\n";
    } while (input.empty());
    return input;
}

// Add a new incident
Incident add_incident() {
    cout << "\n" << COLOR_CYAN << "Add New Incident" << COLOR_RESET << endl;
    string area = get_valid_input("Area where the incident occurred (e.g., Vasile Alecsandri Street): ");
    string type = get_valid_input("Type of incident (e.g., Non-functional streetlight): ");
    return Incident{area, type};
}

// Main menu for incident reporting
void menu(const string& filename) {
    while (true) {
        cout << "\n" << COLOR_CYAN << "Incident Reporting Menu" << COLOR_RESET << endl;
        cout << "1. View all incidents\n2. Add new incident\n3. Filter incidents\n4. Exit\nChoose option: ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 1) {
            auto incidents = read_file(filename);
            display_incidents(incidents);
        } else if (choice == 2) {
            Incident inc = add_incident();
            write_file(filename, inc);
            cout << COLOR_GREEN << "Incident added successfully!" << COLOR_RESET << endl;
        } else if (choice == 3) {
            auto incidents = read_file(filename);
            filter_incidents(incidents);
        } else if (choice == 4) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

// Main function
int main() {
    string filename = "incidents.txt";
    menu(filename);
    return 0;
}