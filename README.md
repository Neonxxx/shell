Windows Terminal C++ Qt

A custom terminal/shell built using C++ and Qt, featuring autocompletion, file navigation, and a customizable UI.

Features

Autocomplete with TAB: Supports cycling through file names and commands.

Customizable UI: Resizable window, color themes, and font settings.

Command Execution: Run system commands and scripts.

File Navigation: Browse directories and open files.

Cross-Platform Support: Works on Windows, and can be adapted for Linux/macOS.

Installation

Prerequisites

Qt Framework (Qt 6 recommended)

C++ Compiler (MinGW or MSVC for Windows)

CMake (if using CMake for builds)

Build Instructions

Using Qt Creator

Open the project in Qt Creator.

Configure the project with the desired Qt version.

Click Build & Run.

Using CMake & CLI

mkdir build && cd build
cmake .. -G "MinGW Makefiles"
make
./terminal_app

Usage

Launch the application.

Type commands and press Enter to execute.

Press TAB for autocompletion of filenames and commands.

Use cd <directory> to navigate directories.

Customize appearance via settings (if implemented).

Roadmap / Future Improvements



Contributing

Contributions are welcome! Please submit issues or pull requests via GitHub.

License

This project is licensed under the MIT License. See LICENSE for details.

Author

Developed by [Your Name]

