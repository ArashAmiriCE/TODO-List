# 📝 TODO List

> A modern terminal-based TODO List application written in **C**, featuring an interactive Text User Interface powered by **ncurses** and persistent JSON storage.

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux-green.svg)
![UI](https://img.shields.io/badge/UI-ncurses-orange.svg)
![Storage](https://img.shields.io/badge/storage-JSON-yellow.svg)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

## 📖 Overview

TODO List is a lightweight terminal application for organizing daily tasks without leaving the command line.

Unlike simple console programs, this project provides a multi-window Text User Interface (TUI) where tasks, descriptions, categories, subtasks, and deadlines are displayed simultaneously for a better user experience.

All data is automatically stored in JSON format, allowing tasks to persist between executions.

---

## ✨ Features

- ✅ Interactive terminal interface using **ncurses**
- 📋 Create and manage tasks
- 📝 Edit task descriptions
- 📂 Assign categories
- ✔️ Create subtasks
- 📅 Manage deadlines
- ☑️ Mark tasks and subtasks as completed
- 🎯 Keyboard-driven navigation
- 💾 Persistent JSON storage using **cJSON**
- 🧩 Modular C codebase

---

## 🖥️ Interface

The application is divided into multiple windows:

| Window | Purpose |
|---------|----------|
| Tasks | Displays all tasks |
| Description | Shows the selected task description |
| Categories | Lists task categories |
| Subtasks | Displays subtasks |
| Deadline | Shows task deadline |

---

## 📂 Project Structure

```text
TODO-List/
│
├── src/
│   ├── main.c
│   ├── task.c
│   ├── tui.c
│   ├── keys.c
│   ├── globals.c
│   └── cJSON.c
│
├── include/
│   ├── task.h
│   ├── tui.h
│   ├── globals.h
│   ├── keys.h
│   ├── constants.h
│   └── cJSON.h
│
├── data/
│   └── tasks.json
│
└── README.md
```

---

## ⚙️ Technologies

- C
- ncurses
- cJSON
- JSON
- GCC

---

## 🚀 Build

Compile the project using GCC.

```bash
gcc src/*.c -lncurses -o todo
```

Run:

```bash
./todo
```

---

## 💾 Data Storage

All tasks are stored locally in:

```text
data/tasks.json
```

The application automatically loads saved tasks when it starts.

---

## 🎯 Learning Objectives

This project was developed to practice:

- Modular programming in C
- Dynamic memory management
- Terminal User Interface development
- File handling
- JSON serialization/deserialization
- Keyboard event handling
- Clean project organization

---

## 📌 Future Improvements

- [ ] Task priorities
- [ ] Search functionality
- [ ] Sorting
- [ ] Color themes
- [ ] Mouse support
- [ ] Recurring tasks
- [ ] Notifications

---

## 🤝 Contributing

Contributions are always welcome.

Feel free to fork the repository, open an issue, or submit a pull request.

---

## 📄 License

This project is released under the MIT License.

---

## ⭐ If you like this project

Please consider giving it a **Star** on GitHub!
