---

# 🎓 CGPA Calculator (C++)

A terminal-based **CGPA Calculator** built in C++ that helps students track their academic performance across multiple semesters. It supports both **letter grades and percentages**, calculates semester GPA, and provides a detailed cumulative CGPA summary.

---

## 📌 Features

* Input multiple semesters (up to 12)
* Add multiple courses per semester
* Accepts:

  * Letter grades (A+, A, B, etc.)
  * Percentage (0–100)
* Automatically converts percentage → grade
* Calculates:

  * GPA for each semester
  * Overall CGPA
* Displays:

  * Detailed course table
  * Credit hours & quality points
  * Visual GPA progress bar
  * Academic standing (e.g., Good, Warning)
* Color-coded terminal output (ANSI supported terminals)

---

## 🛠️ How It Works

The calculator follows this process:

1. User enters number of semesters
2. For each semester:

   * Enter number of courses
   * Input course name, grade, and credit hours
3. System converts grades into grade points (4.0 scale)
4. GPA is calculated:

   ```
   GPA = Total Quality Points / Total Credit Hours
   ```
5. CGPA is calculated across all semesters
6. Results are displayed with tables and visual indicators

---

## 📂 Project Structure

```id="y98vut"
cgpa_calculator.cpp   # Main program
README.md             # Documentation
```

---

## ▶️ How to Compile and Run

### Compile

```bash id="tzqj9x"
g++ -std=c++17 -O2 cgpa_calculator.cpp -o cgpa_calculator
```

### Run

```bash id="wlbqj6"
./cgpa_calculator
```

---

## 📊 Grade Scale (4.0 System)

| Grade | Points |
| ----- | ------ |
| A+, A | 4.0    |
| A-    | 3.7    |
| B+    | 3.3    |
| B     | 3.0    |
| B-    | 2.7    |
| C+    | 2.3    |
| C     | 2.0    |
| C-    | 1.7    |
| D+    | 1.3    |
| D     | 1.0    |
| F     | 0.0    |

---

## 🧪 Example Input

```id="y51g9u"
How many semesters? 2

Semester 1:
Courses: 3
Math       A     3.0
Physics    B+    4.0
English    85    3.0

Semester 2:
Courses: 2
Programming  A+   3.0
Data Struct  B    3.0
```

---

## ✅ Example Output

* Semester GPA displayed with tables
* Overall CGPA shown with:

  * Progress bar
  * Academic standing
  * Total credits and quality points

---

## 🚀 Future Improvements

* File save/load support
* GUI version (Qt or web-based)
* Export result as PDF
* Support for different grading systems

---

## 📖 Concepts Used

* Object-Oriented Programming (Structs)
* STL (`vector`, `map`)
* Input validation
* String handling
* Recursion-free calculations
* Terminal formatting (ANSI colors)

---

## 🤝 Contributing

Feel free to fork the project and improve it. Pull requests and suggestions are welcome.

---

## 📜 License

This project is open-source and free to use for educational purposes.

---

If you want, I can also:

* Add **GitHub badges (build, license, stars)**
* Create a **portfolio-ready version**
* Or make a **LinkedIn project description** for you
