---

# 🧩 Sudoku Solver (C++)

A simple and efficient Sudoku solver written in C++ using the **backtracking algorithm**. It solves standard 9×9 Sudoku puzzles by filling in missing values while respecting all Sudoku rules.

---

## 📌 Features

* Solves any valid 9×9 Sudoku puzzle
* Uses **recursive backtracking**
* Ensures all constraints:

  * No duplicate in any row
  * No duplicate in any column
  * No duplicate in any 3×3 sub-grid
* Measures execution time
* Clean and easy-to-understand code

---

## 🛠️ How It Works

The solver follows a straightforward approach:

1. Find an empty cell (marked as `0`)
2. Try placing numbers from **1 to 9**
3. Check if the number is valid:

   * Not in the same row
   * Not in the same column
   * Not in the same 3×3 box
4. If valid, place the number and continue recursively
5. If stuck, backtrack and try another number

---

## 📂 Project Structure

```
sudoku_solver.cpp   # Main source code
README.md           # Project documentation
```

---

## ▶️ How to Compile and Run

### Compile

```bash
g++ -std=c++17 -O2 sudoku_solver.cpp -o sudoku_solver
```

### Run

```bash
./sudoku_solver
```

---

## 🧪 Sample Input

The program includes a built-in puzzle like this:

```
5 3 0 | 0 7 0 | 0 0 0
6 0 0 | 1 9 5 | 0 0 0
0 9 8 | 0 0 0 | 0 6 0
```

(`0` represents empty cells)

---

## ✅ Sample Output

```
Solved Puzzle:
5 3 4 | 6 7 8 | 9 1 2
6 7 2 | 1 9 5 | 3 4 8
...
```

Execution time is also displayed in milliseconds.

---

## 🚀 Future Improvements

* Add user input support
* GUI-based Sudoku solver
* Implement faster solving techniques (e.g., MRV heuristic)
* Support for larger grids (e.g., 16×16)

---

## 📖 Concepts Used

* Recursion
* Backtracking
* Constraint checking
* 2D arrays

---

## 🤝 Contributing

Feel free to fork this repository and improve it. Suggestions and pull requests are welcome.

---

## 📜 License

This project is open-source and free to use for learning purposes.

---

If you want, I can also make a **more professional GitHub version with badges, screenshots, and animations** or tailor it for your portfolio.
