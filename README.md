# ChessProject

### Overview

This project is a simple yet functional **command-line chess game** written entirely in **C**.  
The main goal of the project was to explore how the complex logic of a classic game like chess can be implemented using basic programming concepts such as arrays, loops, and conditionals.

It serves as an educational project for learning structured programming and understanding how game state management, board representation, and move logic can be handled at a low level without external libraries or frameworks.

---

### The Game

When you run the program, you’ll get a chessboard displayed directly in your terminal.  
Players can make moves by entering coordinates, and the game updates accordingly.  

The current version includes basic piece movement and turn handling. The main focus was to get the **core board mechanics** working — piece positions, move rules, and alternating turns — while keeping the code as clean and readable as possible.

Future updates aim to expand these features into a more complete chess experience with check/checkmate detection, AI opponents, and perhaps even a graphical interface.

---

### The Code

The entire logic of the game resides in the `main.c` file inside the `chess/` directory.  
There’s also a `doc.txt` file explaining the design choices, move logic, and ideas for extending the project.

The structure is intentionally minimal, making it easy to read and modify — perfect for beginners looking to understand how games can be built from scratch in C.

---

### Requirements

- **C Compiler** (e.g., GCC, Clang, or MSVC)  
- **Terminal / Command Prompt**  
- (Optional) **Visual Studio Code** with the C/C++ extension  

---

### How to Build and Run

#### Using GCC (Linux / macOS / Windows with MinGW)

```bash
cd chess
gcc main.c -o chess
./chess
````

#### Using Visual Studio Code

1. Open the project folder in **VS Code**
2. Ensure the **C/C++ extension** is installed
3. Open `chess/main.c`
4. Press **Ctrl + F5** (Run Without Debugging)

---

### Documentation

The `chess/doc.txt` file contains information about:

* How the chessboard and pieces are represented
* Move logic and control flow
* Design notes and future development plans

---

### Future Enhancements

* Implement full move legality (check / checkmate detection)
* Add save/load game functionality
* Develop a simple AI opponent
* Introduce a graphical interface using **SDL** or **ncurses**
* Improve input validation and user feedback

---

### License

This project is made for **educational and learning purposes**.
Feel free to use, modify, and distribute the code.
(Consider adding a formal open-source license such as **MIT** or **GPL**.)

---

### Contributors

* [Alefittase](https://github.com/Alefittase)
* [Raven444x](https://github.com/Raven444x)

---

### Acknowledgements

* Inspired by the timeless game of **Chess ♟️**
* Built entirely in **C** for simplicity and deeper understanding of game logic
* Thanks to everyone who contributed ideas, feedback, and testing!
