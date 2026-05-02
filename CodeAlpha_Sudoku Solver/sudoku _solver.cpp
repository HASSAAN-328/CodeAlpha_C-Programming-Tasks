/*
 * ============================================================
 *              Sudoku Solver (Backtracking)
 * ------------------------------------------------------------
 * A simple C++ program that solves a 9x9 Sudoku puzzle.
 *
 * - Empty cells are represented by 0
 * - Uses recursion + backtracking
 * - Checks row, column, and 3x3 box constraints
 *
 * Compile: g++ -std=c++17 -O2 sudoku.cpp -o sudoku
 * Run    : ./sudoku
 * ============================================================
 */

#include <iostream>
#include <array>
#include <chrono>

using namespace std;

// 9x9 Sudoku grid
using Grid = array<array<int, 9>, 9>;


// ------------------------------------------------------------
// 1. Constraint Checking Functions
// ------------------------------------------------------------

// Check if number already exists in row
bool rowSafe(const Grid& g, int row, int num) {
    for (int col = 0; col < 9; col++)
        if (g[row][col] == num)
            return false;
    return true;
}

// Check if number already exists in column
bool colSafe(const Grid& g, int col, int num) {
    for (int row = 0; row < 9; row++)
        if (g[row][col] == num)
            return false;
    return true;
}

// Check if number exists in 3x3 box
bool boxSafe(const Grid& g, int boxRow, int boxCol, int num) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (g[boxRow * 3 + i][boxCol * 3 + j] == num)
                return false;
    return true;
}

// Combined check
bool isSafe(const Grid& g, int row, int col, int num) {
    return rowSafe(g, row, num) &&
           colSafe(g, col, num) &&
           boxSafe(g, row / 3, col / 3, num);
}


// ------------------------------------------------------------
// 2. Find next empty cell
// ------------------------------------------------------------

bool findEmpty(const Grid& g, int& row, int& col) {
    for (row = 0; row < 9; row++)
        for (col = 0; col < 9; col++)
            if (g[row][col] == 0)
                return true;

    return false; // no empty cells left
}


// ------------------------------------------------------------
// 3. Backtracking Solver
// ------------------------------------------------------------

bool solve(Grid& g) {
    int row, col;

    // If no empty cell → solved
    if (!findEmpty(g, row, col))
        return true;

    // Try numbers 1–9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(g, row, col, num)) {
            g[row][col] = num;  // place number

            if (solve(g))       // recurse
                return true;

            g[row][col] = 0;    // undo (backtrack)
        }
    }

    return false; // no valid number found
}


// ------------------------------------------------------------
// 4. Print Sudoku Grid
// ------------------------------------------------------------

void printGrid(const Grid& g) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << (g[i][j] == 0 ? '.' : char(g[i][j] + '0')) << " ";
        }
        cout << endl;
    }
}


// ------------------------------------------------------------
// 5. Sample Puzzle
// ------------------------------------------------------------

Grid samplePuzzle() {
    return {{
        {5,3,0, 0,7,0, 0,0,0},
        {6,0,0, 1,9,5, 0,0,0},
        {0,9,8, 0,0,0, 0,6,0},

        {8,0,0, 0,6,0, 0,0,3},
        {4,0,0, 8,0,3, 0,0,1},
        {7,0,0, 0,2,0, 0,0,6},

        {0,6,0, 0,0,0, 2,8,0},
        {0,0,0, 4,1,9, 0,0,5},
        {0,0,0, 0,8,0, 0,7,9}
    }};
}


// ------------------------------------------------------------
// 6. Main Function
// ------------------------------------------------------------

int main() {
    Grid puzzle = samplePuzzle();

    cout << "Original Puzzle:\n";
    printGrid(puzzle);

    auto start = chrono::high_resolution_clock::now();

    if (solve(puzzle)) {
        auto end = chrono::high_resolution_clock::now();

        cout << "\nSolved Puzzle:\n";
        printGrid(puzzle);

        double time = chrono::duration<double, milli>(end - start).count();
        cout << "\nSolved in " << time << " ms\n";
    } else {
        cout << "No solution exists.\n";
    }

    return 0;
}