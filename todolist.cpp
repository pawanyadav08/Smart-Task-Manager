#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <limits>
using namespace std;

struct Task {
    string description;
    string deadline; // format: DD/MM/YYYY
    bool isDone;
};

// Parse "DD/MM/YYYY" into day, month, year with validation
void parseDate(string date, int &d, int &m, int &y) {
    if (date.size() != 10 || date[2] != '/' || date[5] != '/') {
        d = m = y = 0;
        return;
    }
    d = stoi(date.substr(0, 2));
    m = stoi(date.substr(3, 2));
    y = stoi(date.substr(6, 4));
}

bool compareByDeadline(Task a, Task b) {
    int d1, m1, y1, d2, m2, y2;
    parseDate(a.deadline, d1, m1, y1);
    parseDate(b.deadline, d2, m2, y2);
    if (y1 != y2) return y1 < y2;
    if (m1 != m2) return m1 < m2;
    return d1 < d2;
}

string checkDeadlineStatus(string deadline) {
    time_t t = time(0);
    tm* now = localtime(&t);
    int todayD = now->tm_mday;
    int todayM = now->tm_mon + 1;
    int todayY = now->tm_year + 1900;

    int d, m, y;
    parseDate(deadline, d, m, y);

    if (d == 0 || m == 0 || y == 0) return "Invalid date";
    if (y < todayY || (y == todayY && m < todayM) || (y == todayY && m == todayM && d < todayD)) {
        return "❌ Overdue";
    } else if (d == todayD && m == todayM && y == todayY) {
        return "⚠ Reminder: Due Today";
    } else {
        return " Upcoming";
    }
}

void addTask(vector<Task>& todoList) {
    Task t;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter task description: ";
    getline(cin, t.description);
    cout << "Enter deadline (DD/MM/YYYY): ";
    getline(cin, t.deadline);
    int d, m, y;
    parseDate(t.deadline, d, m, y);
    if (d == 0 || m == 0 || y == 0) {
        cout << "Invalid date format. Task not added.\n";
        return;
    }
    t.isDone = false;
    todoList.push_back(t);
    cout << "Task added successfully!\n";
}

void viewTasks(vector<Task>& todoList) {
    cout << "\n--- Your Tasks ---\n";
    if (todoList.empty()) {
        cout << "No tasks available.\n";
    } else {
        for (int i = 0; i < todoList.size(); i++) {
            cout << i + 1 << ". " << todoList[i].description
                 << " | Due: " << todoList[i].deadline
                 << (todoList[i].isDone ? " [Done]" : " [Pending]")
                 << " → " << checkDeadlineStatus(todoList[i].deadline)
                 << "\n";
        }
    }
}

void markTaskDone(vector<Task>& todoList) {
    int index;
    cout << "Enter task number to mark as done: ";
    cin >> index;
    if (index > 0 && index <= todoList.size()) {
        todoList[index - 1].isDone = true;
        cout << "Task marked as done!\n";
    } else {
        cout << "Invalid task number.\n";
    }
}

void deleteTask(vector<Task>& todoList) {
    int index;
    cout << "Enter task number to delete: ";
    cin >> index;
    if (index > 0 && index <= todoList.size()) {
        todoList.erase(todoList.begin() + index - 1);
        cout << "Task deleted successfully!\n";
    } else {
        cout << "Invalid task number.\n";
    }
}

void searchTask(vector<Task>& todoList) {
    string keyword;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter keyword to search: ";
    getline(cin, keyword);
    cout << "\n--- Search Results ---\n";
    bool found = false;
    for (int i = 0; i < todoList.size(); i++) {
        if (todoList[i].description.find(keyword) != string::npos) {
            cout << i + 1 << ". " << todoList[i].description 
                 << " | Due: " << todoList[i].deadline
                 << (todoList[i].isDone ? " [Done]" : " [Pending]")
                 << " → " << checkDeadlineStatus(todoList[i].deadline)
                 << "\n";
            found = true;
        }
    }
    if (!found) cout << "No tasks found with keyword: " << keyword << "\n";
}

void sortAlphabetically(vector<Task>& todoList) {
    sort(todoList.begin(), todoList.end(),
         [](Task a, Task b) { return a.description < b.description; });
    cout << "Tasks sorted alphabetically!\n";
}

void sortByDeadline(vector<Task>& todoList) {
    sort(todoList.begin(), todoList.end(), compareByDeadline);
    cout << "Tasks sorted by deadline!\n";
}

void saveTasks(vector<Task>& todoList) {
    ofstream file("tasks.txt");
    for (auto &t : todoList) {
        file << t.description << "|" << t.deadline << "|" << (t.isDone ? "1" : "0") << "\n";
    }
    file.close();
    cout << "Tasks saved successfully!\n";
}

void loadTasks(vector<Task>& todoList) {
    ifstream file("tasks.txt");
    string line;
    todoList.clear();
    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        if (pos1 == string::npos || pos2 == string::npos) continue;
        string desc = line.substr(0, pos1);
        string deadline = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string doneStr = line.substr(pos2 + 1);
        bool done = (doneStr == "1");
        Task t = {desc, deadline, done};
        todoList.push_back(t);
    }
    file.close();
    cout << "Tasks loaded successfully!\n";
}

int main() {
    vector<Task> todoList;
    int choice;

    loadTasks(todoList); // Load tasks on start

    do {
        cout << "\n===== ADVANCED TO-DO LIST MENU =====\n";
        cout << "1. Add Task\n2. View Tasks\n3. Mark Task as Done\n4. Delete Task\n";
        cout << "5. Search Task\n6. Sort Tasks Alphabetically\n7. Sort Tasks by Due Date\n";
        cout << "8. Save Tasks\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addTask(todoList); break;
            case 2: viewTasks(todoList); break;
            case 3: markTaskDone(todoList); break;
            case 4: deleteTask(todoList); break;
            case 5: searchTask(todoList); break;
            case 6: sortAlphabetically(todoList); break;
            case 7: sortByDeadline(todoList); break;
            case 8: saveTasks(todoList); break;
            case 9: saveTasks(todoList); cout << "Exiting... Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 9);

    return 0;

}
