#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ----------------------------- Utility: String Helpers -----------------------------
namespace strutil {
    static inline string trim(const string &s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == string::npos) return "";
        size_t end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }

    static inline string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
        return s;
    }

    static inline string replaceAll(string s, const string &from, const string &to) {
        if (from.empty()) return s;
        size_t pos = 0;
        while ((pos = s.find(from, pos)) != string::npos) {
            s.replace(pos, from.length(), to);
            pos += to.length();
        }
        return s;
    }
}

// ----------------------------- Utility: Console Colors -----------------------------
enum class ConsoleColor {
    DEFAULT,
    RED,
    GREEN,
    YELLOW,
    CYAN,
    MAGENTA,
    BLUE,
    WHITE,
    BRIGHT_WHITE
};

struct Console {
#ifdef _WIN32
    static void setColor(ConsoleColor color) {
        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD attributes = 7; // default gray
        switch (color) {
            case ConsoleColor::RED: attributes = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case ConsoleColor::GREEN: attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case ConsoleColor::YELLOW: attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case ConsoleColor::CYAN: attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case ConsoleColor::MAGENTA: attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case ConsoleColor::BLUE: attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case ConsoleColor::WHITE: attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            case ConsoleColor::BRIGHT_WHITE: attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
            case ConsoleColor::DEFAULT:
            default: attributes = 7; break;
        }
        SetConsoleTextAttribute(hConsole, attributes);
    }

    static void reset() { setColor(ConsoleColor::DEFAULT); }
#else
    static const char* code(ConsoleColor color) {
        switch (color) {
            case ConsoleColor::RED: return "\033[31;1m";
            case ConsoleColor::GREEN: return "\033[32;1m";
            case ConsoleColor::YELLOW: return "\033[33;1m";
            case ConsoleColor::CYAN: return "\033[36;1m";
            case ConsoleColor::MAGENTA: return "\033[35;1m";
            case ConsoleColor::BLUE: return "\033[34;1m";
            case ConsoleColor::WHITE: return "\033[37m";
            case ConsoleColor::BRIGHT_WHITE: return "\033[37;1m";
            case ConsoleColor::DEFAULT:
            default: return "\033[0m";
        }
    }

    static void setColor(ConsoleColor color) { cout << code(color); }
    static void reset() { cout << "\033[0m"; }
#endif
};

// ----------------------------- Domain: Enums and Conversions -----------------------------
enum class Priority { HIGH = 0, MEDIUM = 1, LOW = 2 };
enum class Category { STUDY = 0, WORK = 1, PERSONAL = 2 };
enum class Status { PENDING = 0, COMPLETED = 1 };

static inline string priorityToString(Priority p) {
    switch (p) {
        case Priority::HIGH: return "High";
        case Priority::MEDIUM: return "Medium";
        case Priority::LOW: return "Low";
    }
    return "";
}

static inline string categoryToString(Category c) {
    switch (c) {
        case Category::STUDY: return "Study";
        case Category::WORK: return "Work";
        case Category::PERSONAL: return "Personal";
    }
    return "";
}

static inline string statusToString(Status s) {
    switch (s) {
        case Status::PENDING: return "Pending";
        case Status::COMPLETED: return "Completed";
    }
    return "";
}

static inline bool tryParsePriority(const string &s, Priority &out) {
    string v = strutil::toLower(strutil::trim(s));
    if (v == "high" || v == "h") { out = Priority::HIGH; return true; }
    if (v == "medium" || v == "m") { out = Priority::MEDIUM; return true; }
    if (v == "low" || v == "l") { out = Priority::LOW; return true; }
    return false;
}

static inline bool tryParseCategory(const string &s, Category &out) {
    string v = strutil::toLower(strutil::trim(s));
    if (v == "study" || v == "s") { out = Category::STUDY; return true; }
    if (v == "work" || v == "w") { out = Category::WORK; return true; }
    if (v == "personal" || v == "p") { out = Category::PERSONAL; return true; }
    return false;
}

static inline bool tryParseStatus(const string &s, Status &out) {
    string v = strutil::toLower(strutil::trim(s));
    if (v == "pending" || v == "p") { out = Status::PENDING; return true; }
    if (v == "completed" || v == "c" || v == "done") { out = Status::COMPLETED; return true; }
    return false;
}

// ----------------------------- Utility: Date Handling -----------------------------
struct Date {
    int day{1}, month{1}, year{1970};
};

static inline bool isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static inline bool isValidDate(const Date &d) {
    if (d.year < 1900 || d.year > 3000) return false;
    if (d.month < 1 || d.month > 12) return false;
    static const int mdays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int days = mdays[d.month];
    if (d.month == 2 && isLeap(d.year)) days = 29;
    return d.day >= 1 && d.day <= days;
}

static inline bool parseDate(const string &s, Date &out) {
    // Expecting dd/mm/yyyy
    int d=0,m=0,y=0;
    char c1='\0', c2='\0';
    std::istringstream iss(s);
    iss >> d >> c1 >> m >> c2 >> y;
    if (!iss.fail() && c1 == '/' && c2 == '/' ) {
        out.day = d; out.month = m; out.year = y;
        return isValidDate(out);
    }
    return false;
}

static inline string dateToString(const Date &d) {
    std::ostringstream oss;
    oss << setw(2) << setfill('0') << d.day << "/"
        << setw(2) << setfill('0') << d.month << "/"
        << setw(4) << setfill('0') << d.year;
    return oss.str();
}

static inline time_t toTimeT(const Date &d) {
    std::tm t = {};
    t.tm_mday = d.day;
    t.tm_mon = d.month - 1;
    t.tm_year = d.year - 1900;
    t.tm_hour = 0; t.tm_min = 0; t.tm_sec = 0;
    t.tm_isdst = -1;
    return mktime(&t);
}

static inline long daysBetween(time_t a, time_t b) {
    const long sec_per_day = 24 * 60 * 60;
    long diff = static_cast<long>(difftime(b, a));
    return diff / sec_per_day;
}

static inline long daysFromToday(const Date &d) {
    time_t now = time(nullptr);
    std::tm *ptm = localtime(&now);
    std::tm todayTm = *ptm;
    todayTm.tm_hour = 0; todayTm.tm_min = 0; todayTm.tm_sec = 0;
    time_t today = mktime(&todayTm);
    return daysBetween(today, toTimeT(d));
}

// ----------------------------- Data Model: Task -----------------------------
class Task {
public:
    int id;
    string name;
    Date deadline;
    Priority priority;
    Category category;
    Status status;

    Task() : id(0), name("") , deadline{1,1,1970}, priority(Priority::MEDIUM), category(Category::PERSONAL), status(Status::PENDING) {}

    Task(int id_, string name_, Date dl, Priority p, Category c, Status s)
        : id(id_), name(std::move(name_)), deadline(dl), priority(p), category(c), status(s) {}

    // Serialize to tasks.txt (pipe-delimited, escape '|' in name)
    string serialize() const {
        string safeName = strutil::replaceAll(name, "|", "/");
        ostringstream oss;
        oss << id << "|" << safeName << "|" << dateToString(deadline) << "|"
            << priorityToString(priority) << "|"
            << categoryToString(category) << "|"
            << statusToString(status);
        return oss.str();
    }

    static bool deserialize(const string &line, Task &out) {
        vector<string> parts;
        string cur;
        for (char ch : line) {
            if (ch == '|') { parts.push_back(cur); cur.clear(); }
            else cur.push_back(ch);
        }
        parts.push_back(cur);
        if (parts.size() != 6) return false;
        Task t;
        try {
            t.id = stoi(parts[0]);
        } catch (...) { return false; }
        t.name = strutil::trim(parts[1]);
        Date d; if (!parseDate(strutil::trim(parts[2]), d)) return false; t.deadline = d;
        Priority p; if (!tryParsePriority(parts[3], p)) return false; t.priority = p;
        Category c; if (!tryParseCategory(parts[4], c)) return false; t.category = c;
        Status s; if (!tryParseStatus(parts[5], s)) return false; t.status = s;
        out = t;
        return true;
    }
};

// ----------------------------- Repository: File Storage -----------------------------
class TaskRepository {
private:
    string tasksFilePath;
public:
    explicit TaskRepository(string path) : tasksFilePath(std::move(path)) {}

    vector<Task> loadAll() {
        vector<Task> tasks;
        ifstream fin(tasksFilePath);
        if (!fin.is_open()) {
            return tasks; // No file yet; return empty
        }
        string line;
        while (getline(fin, line)) {
            line = strutil::trim(line);
            if (line.empty()) continue;
            Task t;
            if (Task::deserialize(line, t)) tasks.push_back(t);
        }
        return tasks;
    }

    bool saveAll(const vector<Task> &tasks) {
        ofstream fout(tasksFilePath, ios::trunc);
        if (!fout.is_open()) return false;
        for (const Task &t : tasks) {
            fout << t.serialize() << "\n";
        }
        return true;
    }
};

// ----------------------------- Business Logic: TaskManager -----------------------------
class TaskManager {
private:
    vector<Task> tasks;
    TaskRepository repo;
    int nextId;

    int computeNextId() const {
        int maxId = 0;
        for (const Task &t : tasks) maxId = max(maxId, t.id);
        return maxId + 1;
    }

public:
    explicit TaskManager(const string &filePath) : repo(filePath), nextId(1) {
        tasks = repo.loadAll();
        nextId = computeNextId();
    }

    bool persist() { return repo.saveAll(tasks); }

    const vector<Task>& getAll() const { return tasks; }

    // CRUD
    Task addTask(const string &name, const Date &deadline, Priority pr, Category cat, Status st) {
        Task t(nextId++, name, deadline, pr, cat, st);
        tasks.push_back(t);
        persist();
        return t;
    }

    bool deleteTask(int id) {
        auto it = remove_if(tasks.begin(), tasks.end(), [&](const Task &t){ return t.id == id; });
        bool removed = it != tasks.end();
        if (removed) tasks.erase(it, tasks.end());
        if (removed) persist();
        return removed;
    }

    Task* findById(int id) {
        for (auto &t : tasks) if (t.id == id) return &t;
        return nullptr;
    }

    bool editTask(int id, const string &newName, const Date &newDeadline, Priority newPr, Category newCat, Status newSt) {
        Task* t = findById(id);
        if (!t) return false;
        t->name = newName;
        t->deadline = newDeadline;
        t->priority = newPr;
        t->category = newCat;
        t->status = newSt;
        persist();
        return true;
    }

    // Search by name substring or category exact
    vector<Task> searchByName(const string &query) const {
        string q = strutil::toLower(strutil::trim(query));
        vector<Task> result;
        for (const Task &t : tasks) {
            string ln = strutil::toLower(t.name);
            if (ln.find(q) != string::npos) result.push_back(t);
        }
        return result;
    }

    vector<Task> searchByCategory(Category cat) const {
        vector<Task> result;
        copy_if(tasks.begin(), tasks.end(), back_inserter(result), [&](const Task &t){ return t.category == cat; });
        return result;
    }

    // Filter
    vector<Task> filterByPriority(Priority p) const {
        vector<Task> result;
        copy_if(tasks.begin(), tasks.end(), back_inserter(result), [&](const Task &t){ return t.priority == p; });
        return result;
    }

    vector<Task> filterByStatus(Status s) const {
        vector<Task> result;
        copy_if(tasks.begin(), tasks.end(), back_inserter(result), [&](const Task &t){ return t.status == s; });
        return result;
    }

    // Sort by deadline ascending
    vector<Task> sortByDeadline(vector<Task> in) const {
        sort(in.begin(), in.end(), [&](const Task &a, const Task &b){
            return toTimeT(a.deadline) < toTimeT(b.deadline);
        });
        return in;
    }

    // Progress percentage
    double completionPercentage() const {
        if (tasks.empty()) return 0.0;
        int completed = 0;
        for (const Task &t : tasks) if (t.status == Status::COMPLETED) ++completed;
        return 100.0 * static_cast<double>(completed) / static_cast<double>(tasks.size());
    }

    // Suggest next task: Highest priority, then earliest deadline, pending only
    optional<Task> suggestNext() const {
        vector<Task> candidates;
        copy_if(tasks.begin(), tasks.end(), back_inserter(candidates), [](const Task &t){ return t.status == Status::PENDING; });
        if (candidates.empty()) return nullopt;
        stable_sort(candidates.begin(), candidates.end(), [](const Task &a, const Task &b){
            // Priority order: HIGH < MEDIUM < LOW (so smaller is better)
            if (a.priority != b.priority) return static_cast<int>(a.priority) < static_cast<int>(b.priority);
            time_t ta = toTimeT(a.deadline), tb = toTimeT(b.deadline);
            if (ta != tb) return ta < tb;
            return a.id < b.id;
        });
        return candidates.front();
    }

    // Export to CSV
    bool exportCsv(const string &csvPath) const {
        ofstream fout(csvPath, ios::trunc);
        if (!fout.is_open()) return false;
        fout << "Task ID,Task Name,Deadline,Priority,Category,Status\n";
        for (const Task &t : tasks) {
            // Escape commas in name by wrapping in quotes and doubling quotes
            string nameEsc = t.name;
            bool needQuotes = nameEsc.find(',') != string::npos || nameEsc.find('"') != string::npos;
            if (nameEsc.find('"') != string::npos) nameEsc = strutil::replaceAll(nameEsc, "\"", "\"\"");
            if (needQuotes) nameEsc = string("\"") + nameEsc + string("\"");
            fout << t.id << "," << nameEsc << "," << dateToString(t.deadline) << ","
                 << priorityToString(t.priority) << ","
                 << categoryToString(t.category) << ","
                 << statusToString(t.status) << "\n";
        }
        return true;
    }
};

// ----------------------------- UI Helpers: Table Rendering -----------------------------
struct TableColumn {
    string header;
    int width;
};

static inline void drawHorizontalBorder(const vector<TableColumn> &cols) {
    cout << "+";
    for (const auto &col : cols) {
        cout << string(col.width + 2, '-') << "+"; // spaces around cell content
    }
    cout << "\n";
}

static inline string truncateOrPad(const string &s, int width) {
    if ((int)s.size() > width) return s.substr(0, width - 1) + "…";
    if ((int)s.size() < width) return s + string(width - (int)s.size(), ' ');
    return s;
}

static inline void printRow(const vector<TableColumn> &cols, const vector<string> &values, ConsoleColor color = ConsoleColor::DEFAULT) {
    Console::setColor(color);
    cout << "|";
    for (size_t i = 0; i < cols.size(); ++i) {
        string v = i < values.size() ? values[i] : "";
        cout << " " << truncateOrPad(v, cols[i].width) << " ";
        Console::reset(); // ensure borders not colored
        cout << "|";
        Console::setColor(color);
    }
    Console::reset();
    cout << "\n";
}

static inline void printHeader(const vector<TableColumn> &cols) {
    drawHorizontalBorder(cols);
    vector<string> headers;
    headers.reserve(cols.size());
    for (auto &c : cols) headers.push_back(c.header);
    printRow(cols, headers, ConsoleColor::BRIGHT_WHITE);
    drawHorizontalBorder(cols);
}

// ----------------------------- UI Layer: Menu and Interaction -----------------------------
static inline void pauseForEnter() {
    Console::setColor(ConsoleColor::CYAN);
    cout << "\nPress Enter to continue...";
    Console::reset();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static inline string readLine(const string &prompt) {
    Console::setColor(ConsoleColor::YELLOW);
    cout << prompt;
    Console::reset();
    string s;
    getline(cin, s);
    return strutil::trim(s);
}

static inline bool readDateInteractive(const string &prompt, Date &out) {
    string s = readLine(prompt + " (dd/mm/yyyy): ");
    Date d; if (!parseDate(s, d)) return false;
    if (!isValidDate(d)) return false;
    out = d; return true;
}

static inline Priority readPriorityInteractive() {
    while (true) {
        string s = readLine("Priority [High/Medium/Low]: ");
        Priority p; if (tryParsePriority(s, p)) return p;
        Console::setColor(ConsoleColor::RED); cout << "Invalid priority. Try again.\n"; Console::reset();
    }
}

static inline Category readCategoryInteractive() {
    while (true) {
        string s = readLine("Category [Study/Work/Personal]: ");
        Category c; if (tryParseCategory(s, c)) return c;
        Console::setColor(ConsoleColor::RED); cout << "Invalid category. Try again.\n"; Console::reset();
    }
}

static inline Status readStatusInteractive() {
    while (true) {
        string s = readLine("Status [Pending/Completed]: ");
        Status st; if (tryParseStatus(s, st)) return st;
        Console::setColor(ConsoleColor::RED); cout << "Invalid status. Try again.\n"; Console::reset();
    }
}

static inline void viewTasksTable(const vector<Task> &tasks) {
    vector<TableColumn> cols = {
        {"ID", 4}, {"Task Name", 28}, {"Deadline", 12}, {"Priority", 8}, {"Category", 10}, {"Status", 10}
    };
    printHeader(cols);
    for (const Task &t : tasks) {
        long daysLeft = daysFromToday(t.deadline);
        ConsoleColor rowColor = ConsoleColor::DEFAULT;
        if (t.status == Status::COMPLETED) rowColor = ConsoleColor::GREEN;
        else if (daysLeft < 0) rowColor = ConsoleColor::MAGENTA; // overdue
        else if (daysLeft <= 2) rowColor = ConsoleColor::RED; // reminder

        vector<string> vals = {
            to_string(t.id),
            t.name,
            dateToString(t.deadline),
            priorityToString(t.priority),
            categoryToString(t.category),
            statusToString(t.status)
        };
        printRow(cols, vals, rowColor);
    }
    drawHorizontalBorder(cols);
}

static inline void showProgress(const TaskManager &tm) {
    double pct = tm.completionPercentage();
    Console::setColor(ConsoleColor::CYAN);
    cout << fixed << setprecision(1) << "\nProgress: " << pct << "% completed\n";
    Console::reset();
}

static inline void showSuggestion(const TaskManager &tm) {
    auto opt = tm.suggestNext();
    if (!opt.has_value()) {
        Console::setColor(ConsoleColor::YELLOW);
        cout << "No pending tasks to suggest.\n";
        Console::reset();
        return;
    }
    const Task &t = *opt;
    Console::setColor(ConsoleColor::BRIGHT_WHITE);
    cout << "Suggested next task -> ";
    Console::reset();
    Console::setColor(ConsoleColor::GREEN);
    cout << "[" << t.id << "] " << t.name << " (" << priorityToString(t.priority) << ", due " << dateToString(t.deadline) << ")\n";
    Console::reset();
}

// ----------------------------- Password Protection -----------------------------
static inline string readPasswordFromFile(const string &path) {
    ifstream fin(path);
    if (!fin.is_open()) return "";
    string pw; getline(fin, pw);
    return strutil::trim(pw);
}

static inline void writePasswordToFile(const string &path, const string &pw) {
    ofstream fout(path, ios::trunc);
    if (fout.is_open()) fout << pw << "\n";
}

static inline bool authenticate(const string &passwordFilePath) {
    string stored = readPasswordFromFile(passwordFilePath);
    if (stored.empty()) {
        stored = "admin123"; // default
        writePasswordToFile(passwordFilePath, stored);
    }
    Console::setColor(ConsoleColor::BRIGHT_WHITE);
    cout << "Enter password: ";
    Console::reset();
    string input;
    getline(cin, input);
    if (input == stored) {
        Console::setColor(ConsoleColor::GREEN); cout << "Access granted!\n"; Console::reset();
        return true;
    }
    Console::setColor(ConsoleColor::RED); cout << "Incorrect password. Exiting.\n"; Console::reset();
    return false;
}

// ----------------------------- Menu -----------------------------
static inline void printMainMenu() {
    Console::setColor(ConsoleColor::BRIGHT_WHITE);
    cout << "\n===== Smart Task Manager =====\n";
    Console::reset();
    Console::setColor(ConsoleColor::CYAN);
    cout << "1. Add Task\n";
    cout << "2. Edit Task\n";
    cout << "3. Delete Task\n";
    cout << "4. View All Tasks\n";
    cout << "5. Search Tasks\n";
    cout << "6. Filter Tasks\n";
    cout << "7. Sort by Deadline\n";
    cout << "8. Show Progress\n";
    cout << "9. Suggest Next Task\n";
    cout << "10. Export to report.csv\n";
    cout << "0. Exit\n";
    Console::reset();
}

static inline void handleAdd(TaskManager &tm) {
    Console::setColor(ConsoleColor::BRIGHT_WHITE);
    cout << "\nAdd New Task\n";
    Console::reset();
    string name = readLine("Task Name: ");
    Date d; while (!readDateInteractive("Deadline", d)) { Console::setColor(ConsoleColor::RED); cout << "Invalid date. Try again.\n"; Console::reset(); }
    Priority p = readPriorityInteractive();
    Category c = readCategoryInteractive();
    Status s = readStatusInteractive();
    Task t = tm.addTask(name, d, p, c, s);
    Console::setColor(ConsoleColor::GREEN); cout << "Task added with ID: " << t.id << "\n"; Console::reset();
}

static inline void handleEdit(TaskManager &tm) {
    string sid = readLine("Enter Task ID to edit: ");
    int id = 0; try { id = stoi(sid); } catch (...) { Console::setColor(ConsoleColor::RED); cout << "Invalid ID.\n"; Console::reset(); return; }
    Task *t = tm.findById(id);
    if (!t) { Console::setColor(ConsoleColor::RED); cout << "Task not found.\n"; Console::reset(); return; }

    Console::setColor(ConsoleColor::BRIGHT_WHITE);
    cout << "Editing Task [" << t->id << "] " << t->name << "\n";
    Console::reset();

    string name = readLine("New Task Name (leave empty to keep current): ");
    if (!name.empty()) t->name = name;

    string dateStr = readLine("New Deadline dd/mm/yyyy (leave empty to keep current): ");
    if (!dateStr.empty()) {
        Date d; if (!parseDate(dateStr, d) || !isValidDate(d)) { Console::setColor(ConsoleColor::RED); cout << "Invalid date. Keeping old.\n"; Console::reset(); }
        else t->deadline = d;
    }

    string pStr = readLine("New Priority [High/Medium/Low] (leave empty to keep): ");
    if (!pStr.empty()) { Priority p; if (tryParsePriority(pStr, p)) t->priority = p; else { Console::setColor(ConsoleColor::RED); cout << "Invalid priority. Keeping old.\n"; Console::reset(); } }

    string cStr = readLine("New Category [Study/Work/Personal] (leave empty to keep): ");
    if (!cStr.empty()) { Category c; if (tryParseCategory(cStr, c)) t->category = c; else { Console::setColor(ConsoleColor::RED); cout << "Invalid category. Keeping old.\n"; Console::reset(); } }

    string sStr = readLine("New Status [Pending/Completed] (leave empty to keep): ");
    if (!sStr.empty()) { Status s; if (tryParseStatus(sStr, s)) t->status = s; else { Console::setColor(ConsoleColor::RED); cout << "Invalid status. Keeping old.\n"; Console::reset(); } }

    tm.persist();
    Console::setColor(ConsoleColor::GREEN); cout << "Task updated.\n"; Console::reset();
}

static inline void handleDelete(TaskManager &tm) {
    string sid = readLine("Enter Task ID to delete: ");
    int id = 0; try { id = stoi(sid); } catch (...) { Console::setColor(ConsoleColor::RED); cout << "Invalid ID.\n"; Console::reset(); return; }
    if (tm.deleteTask(id)) { Console::setColor(ConsoleColor::GREEN); cout << "Task deleted.\n"; Console::reset(); }
    else { Console::setColor(ConsoleColor::RED); cout << "Task not found.\n"; Console::reset(); }
}

static inline void handleViewAll(TaskManager &tm) {
    auto data = tm.getAll();
    if (data.empty()) { Console::setColor(ConsoleColor::YELLOW); cout << "No tasks to show.\n"; Console::reset(); return; }
    viewTasksTable(data);
}

static inline void handleSearch(TaskManager &tm) {
    Console::setColor(ConsoleColor::BRIGHT_WHITE);
    cout << "\nSearch Tasks\n";
    Console::reset();
    cout << "1) By Name  2) By Category\n";
    string c = readLine("Choice: ");
    if (c == "1") {
        string q = readLine("Enter part of task name: ");
        auto res = tm.searchByName(q);
        if (res.empty()) { Console::setColor(ConsoleColor::YELLOW); cout << "No matches.\n"; Console::reset(); }
        else viewTasksTable(res);
    } else if (c == "2") {
        Category cat = readCategoryInteractive();
        auto res = tm.searchByCategory(cat);
        if (res.empty()) { Console::setColor(ConsoleColor::YELLOW); cout << "No matches.\n"; Console::reset(); }
        else viewTasksTable(res);
    } else {
        Console::setColor(ConsoleColor::RED); cout << "Invalid choice.\n"; Console::reset();
    }
}

static inline void handleFilter(TaskManager &tm) {
    Console::setColor(ConsoleColor::BRIGHT_WHITE);
    cout << "\nFilter Tasks\n";
    Console::reset();
    cout << "1) By Priority  2) By Status\n";
    string c = readLine("Choice: ");
    if (c == "1") {
        Priority p = readPriorityInteractive();
        auto res = tm.filterByPriority(p);
        if (res.empty()) { Console::setColor(ConsoleColor::YELLOW); cout << "No tasks with this priority.\n"; Console::reset(); }
        else viewTasksTable(res);
    } else if (c == "2") {
        Status s = readStatusInteractive();
        auto res = tm.filterByStatus(s);
        if (res.empty()) { Console::setColor(ConsoleColor::YELLOW); cout << "No tasks with this status.\n"; Console::reset(); }
        else viewTasksTable(res);
    } else {
        Console::setColor(ConsoleColor::RED); cout << "Invalid choice.\n"; Console::reset();
    }
}

static inline void handleSortByDeadline(TaskManager &tm) {
    auto sorted = tm.sortByDeadline(vector<Task>(tm.getAll().begin(), tm.getAll().end()));
    if (sorted.empty()) { Console::setColor(ConsoleColor::YELLOW); cout << "No tasks.\n"; Console::reset(); return; }
    viewTasksTable(sorted);
}

static inline void handleExport(TaskManager &tm) {
    if (tm.exportCsv("report.csv")) { Console::setColor(ConsoleColor::GREEN); cout << "Exported to report.csv\n"; Console::reset(); }
    else { Console::setColor(ConsoleColor::RED); cout << "Failed to export.\n"; Console::reset(); }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const string tasksPath = "tasks.txt";
    const string passwordPath = "password.txt";

    // Password prompt
    Console::setColor(ConsoleColor::CYAN);
    cout << "Welcome to Smart Task Manager" << "\n";
    Console::reset();
    if (!authenticate(passwordPath)) return 1;

    TaskManager tm(tasksPath);

    while (true) {
        printMainMenu();
        string choice = readLine("Select an option: ");
        if (choice == "1") handleAdd(tm);
        else if (choice == "2") handleEdit(tm);
        else if (choice == "3") handleDelete(tm);
        else if (choice == "4") handleViewAll(tm);
        else if (choice == "5") handleSearch(tm);
        else if (choice == "6") handleFilter(tm);
        else if (choice == "7") handleSortByDeadline(tm);
        else if (choice == "8") showProgress(tm);
        else if (choice == "9") showSuggestion(tm);
        else if (choice == "10") handleExport(tm);
        else if (choice == "0") {
            Console::setColor(ConsoleColor::GREEN); cout << "Goodbye!\n"; Console::reset();
            break;
        } else {
            Console::setColor(ConsoleColor::RED); cout << "Invalid choice.\n"; Console::reset();
        }
        cout << "\n";
    }

    return 0;
}