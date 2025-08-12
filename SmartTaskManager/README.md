# Smart Task Manager

A console-based C++ application to manage tasks with persistence, colorful UI, and helpful utilities like search, filtering, sorting, progress tracking, reminders, and CSV export.

## Features
- Add, edit, delete tasks (auto-generated ID)
- Fields: Name, Deadline (dd/mm/yyyy), Priority (High/Medium/Low), Category (Study/Work/Personal), Status (Pending/Completed)
- Data persistence in `tasks.txt`
- View tasks in a boxed ASCII table with color highlighting (red if <2 days remaining, magenta if overdue, green if completed)
- Search by name or category
- Filter by priority or status
- Sort by deadline (earliest first)
- Progress tracking (% completed)
- Suggest next task by priority and deadline
- Password protection via `password.txt` (default `admin123` created on first run)
- Export all tasks to `report.csv`

## Build (Linux)
```bash
cd SmartTaskManager
make
```

This produces the binary `smart_task_manager`.

## Run
```bash
./smart_task_manager
```
The app will prompt for a password. Default is `admin123` unless you edit `password.txt`.

## Build (Windows - MinGW)
```bash
mingw32-make
smart_task_manager.exe
```
On Windows, the app uses `SetConsoleTextAttribute` for colors.

## Files
- `tasks.txt`: persistent storage (auto-created)
- `password.txt`: password storage (auto-created with default `admin123`)
- `report.csv`: exported CSV

## Notes
- Input is validated. Dates must be `dd/mm/yyyy`.
- Names cannot contain the `|` character; it will be replaced by `/` when saving.
- Colors use ANSI codes on Unix-like systems.