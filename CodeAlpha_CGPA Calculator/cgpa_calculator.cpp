/*
 * ================================================================
 *   CGPA Calculator — Multi-Semester Grade Point Average Tracker
 *   Author : Claude (Anthropic)
 *   Compile: g++ -std=c++17 -O2 -o cgpa_calculator cgpa_calculator.cpp
 *   Run    : ./cgpa_calculator
 * ================================================================
 *
 *  Features
 *  ────────
 *  · Input number of courses per semester
 *  · Input grade (letter or numeric) + credit hours per course
 *  · Calculate GPA for each semester
 *  · Accumulate and compute overall CGPA across semesters
 *  · Display detailed grade table with letter grades and grade points
 *  · Grade scale: A+/A/A- ... F  (4.0 scale)
 * ================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <limits>

// ─── Color / style helpers (ANSI — works on Linux / macOS terminal) ──────────
namespace Clr {
    const std::string RESET  = "\033[0m";
    const std::string BOLD   = "\033[1m";
    const std::string DIM    = "\033[2m";
    const std::string CYAN   = "\033[96m";
    const std::string GREEN  = "\033[92m";
    const std::string YELLOW = "\033[93m";
    const std::string RED    = "\033[91m";
    const std::string BLUE   = "\033[94m";
    const std::string MAG    = "\033[95m";
    const std::string WHITE  = "\033[97m";
}

// ═══════════════════════════════════════════════════════════
//  1.  GRADE SCALE
// ═══════════════════════════════════════════════════════════

struct GradeInfo {
    double  points;      // grade points on 4.0 scale
    std::string letter;  // canonical letter grade
    std::string remark;
};

// Maps a letter-grade string → GradeInfo
const std::map<std::string, GradeInfo, std::less<>> GRADE_MAP = {
    {"A+", {4.0, "A+", "Outstanding" }},
    {"A",  {4.0, "A",  "Excellent"   }},
    {"A-", {3.7, "A-", "Very Good"   }},
    {"B+", {3.3, "B+", "Good"        }},
    {"B",  {3.0, "B",  "Above Avg"   }},
    {"B-", {2.7, "B-", "Average"     }},
    {"C+", {2.3, "C+", "Below Avg"   }},
    {"C",  {2.0, "C",  "Satisfactory"}},
    {"C-", {1.7, "C-", "Marginal"    }},
    {"D+", {1.3, "D+", "Poor"        }},
    {"D",  {1.0, "D",  "Very Poor"   }},
    {"F",  {0.0, "F",  "Fail"        }},
};

// Convert percentage → letter grade
std::string percentToLetter(double pct)
{
    if (pct >= 97) return "A+";
    if (pct >= 93) return "A";
    if (pct >= 90) return "A-";
    if (pct >= 87) return "B+";
    if (pct >= 83) return "B";
    if (pct >= 80) return "B-";
    if (pct >= 77) return "C+";
    if (pct >= 73) return "C";
    if (pct >= 70) return "C-";
    if (pct >= 67) return "D+";
    if (pct >= 60) return "D";
    return "F";
}

// Normalize input to uppercase and trim whitespace
std::string normalize(std::string s)
{
    for (auto& ch : s) ch = toupper(ch);
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);
    return s;
}

// ═══════════════════════════════════════════════════════════
//  2.  DATA STRUCTURES
// ═══════════════════════════════════════════════════════════

struct Course {
    std::string name;
    std::string letterGrade;
    double      gradePoints;   // 0.0 – 4.0
    double      creditHours;
    double      qualityPoints; // gradePoints × creditHours
};

struct Semester {
    int                  number;
    std::string          label;
    std::vector<Course>  courses;
    double               totalCredits;
    double               totalQualityPoints;
    double               gpa;
};

// ═══════════════════════════════════════════════════════════
//  3.  INPUT HELPERS
// ═══════════════════════════════════════════════════════════

// Safe integer input with bounds checking
int getInt(const std::string& prompt, int lo, int hi)
{
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= lo && v <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Clr::RED << "  ✗ Enter a number between "
                  << lo << " and " << hi << ".\n" << Clr::RESET;
    }
}

// Safe double input
double getDouble(const std::string& prompt, double lo, double hi)
{
    double v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= lo && v <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Clr::RED << "  ✗ Enter a value between "
                  << lo << " and " << hi << ".\n" << Clr::RESET;
    }
}

// Grade input: accepts letter grade OR percentage
GradeInfo getGrade()
{
    while (true) {
        std::string raw;
        std::cout << "  Grade (letter A+/A.../F  or  percentage 0-100): ";
        std::getline(std::cin, raw);
        std::string s = normalize(raw);

        // Try letter grade lookup
        auto it = GRADE_MAP.find(s);
        if (it != GRADE_MAP.end()) return it->second;

        // Try percentage
        try {
            double pct = std::stod(s);
            if (pct < 0 || pct > 100) throw std::out_of_range("");
            std::string letter = percentToLetter(pct);
            return GRADE_MAP.at(letter);
        } catch (...) {}

        std::cout << Clr::RED << "  ✗ Unrecognized grade. Try: A+, A, A-, B+, B, … F  or 0–100.\n"
                  << Clr::RESET;
    }
}

// ═══════════════════════════════════════════════════════════
//  4.  DISPLAY HELPERS
// ═══════════════════════════════════════════════════════════

void banner()
{
    std::cout << Clr::CYAN << Clr::BOLD;
    std::cout << "\n";
    std::cout << "  ██████╗ ██████╗ ██████╗  █████╗ \n";
    std::cout << " ██╔════╝██╔════╝ ██╔══██╗██╔══██╗\n";
    std::cout << " ██║     ██║  ███╗██████╔╝███████║\n";
    std::cout << " ██║     ██║   ██║██╔═══╝ ██╔══██║\n";
    std::cout << " ╚██████╗╚██████╔╝██║     ██║  ██║\n";
    std::cout << "  ╚═════╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝\n";
    std::cout << Clr::RESET;
    std::cout << Clr::WHITE << "       C A L C U L A T O R   v2.0\n\n" << Clr::RESET;
}

void separator(char ch = '─', int width = 68)
{
    std::cout << std::string(width, ch) << "\n";
}

// Color-code a GPA value
std::string gpaColor(double gpa)
{
    if (gpa >= 3.7) return Clr::GREEN;
    if (gpa >= 3.0) return Clr::CYAN;
    if (gpa >= 2.0) return Clr::YELLOW;
    return Clr::RED;
}

// Visual GPA bar (40 chars wide)
std::string gpaBar(double gpa)
{
    int filled = static_cast<int>((gpa / 4.0) * 30);
    std::string bar = "[";
    for (int i = 0; i < 30; i++) bar += (i < filled ? "█" : "░");
    bar += "] ";
    bar += std::to_string(gpa).substr(0, 4);
    bar += " / 4.00";
    return bar;
}

// Print a detailed course table for a semester
void printSemesterTable(const Semester& sem)
{
    std::cout << Clr::BOLD << Clr::BLUE;
    std::cout << "\n  ┌─ " << sem.label << " ───────────────────────────────────────────────────\n";
    std::cout << Clr::RESET;

    // Header
    std::cout << "  │ " << Clr::BOLD
              << std::left  << std::setw(28) << "Course"
              << std::right << std::setw(7)  << "Grade"
              << std::setw(8) << "Points"
              << std::setw(8) << "Credits"
              << std::setw(12) << "Quality Pts"
              << Clr::RESET << "\n";

    std::cout << "  │ " << std::string(63, '·') << "\n";

    for (const auto& c : sem.courses) {
        // Color by grade
        std::string gc;
        if (c.gradePoints >= 3.7)      gc = Clr::GREEN;
        else if (c.gradePoints >= 3.0) gc = Clr::CYAN;
        else if (c.gradePoints >= 2.0) gc = Clr::YELLOW;
        else                            gc = Clr::RED;

        std::cout << "  │ "
                  << std::left  << std::setw(28) << c.name
                  << gc << std::right << std::setw(5) << c.letterGrade << Clr::RESET
                  << std::fixed << std::setprecision(1)
                  << std::setw(8) << c.gradePoints
                  << std::setw(8) << c.creditHours
                  << std::setw(12) << c.qualityPoints
                  << "\n";
    }

    std::cout << "  │ " << std::string(63, '─') << "\n";
    std::cout << "  │ "
              << std::left  << std::setw(28) << "TOTALS"
              << std::right << std::setw(5)  << ""
              << std::setw(8)  << ""
              << std::fixed << std::setprecision(1)
              << std::setw(8)  << sem.totalCredits
              << std::setw(12) << sem.totalQualityPoints
              << "\n";

    std::cout << "  │\n";
    std::cout << "  │  GPA  " << gpaColor(sem.gpa) << Clr::BOLD
              << gpaBar(sem.gpa) << Clr::RESET << "\n";
    std::cout << Clr::BLUE << "  └───────────────────────────────────────────────────────────\n"
              << Clr::RESET;
}

// Final CGPA summary across all semesters
void printCGPASummary(const std::vector<Semester>& semesters,
                      double cgpa,
                      double totalCredits,
                      double totalQP)
{
    std::cout << "\n";
    separator('═');
    std::cout << Clr::BOLD << Clr::WHITE << "  CUMULATIVE GPA SUMMARY\n" << Clr::RESET;
    separator('═');

    // Per-semester summary table
    std::cout << Clr::BOLD
              << std::left  << std::setw(22) << "  Semester"
              << std::right << std::setw(10) << "Credits"
              << std::setw(12) << "Quality Pts"
              << std::setw(10) << "GPA"
              << Clr::RESET << "\n";
    separator();

    for (const auto& s : semesters) {
        std::cout << std::left  << std::setw(22) << ("  " + s.label)
                  << std::fixed << std::setprecision(2)
                  << std::right << std::setw(10) << s.totalCredits
                  << std::setw(12) << s.totalQualityPoints
                  << gpaColor(s.gpa) << std::setw(10) << s.gpa
                  << Clr::RESET << "\n";
    }

    separator();
    std::cout << Clr::BOLD
              << std::left  << std::setw(22) << "  OVERALL"
              << std::fixed << std::setprecision(2)
              << std::right << std::setw(10) << totalCredits
              << std::setw(12) << totalQP
              << gpaColor(cgpa) << std::setw(10) << cgpa
              << Clr::RESET << "\n";
    separator('═');

    // Big CGPA display
    std::cout << "\n  " << Clr::BOLD << Clr::WHITE << "Your CGPA: "
              << gpaColor(cgpa) << Clr::BOLD
              << std::fixed << std::setprecision(2) << cgpa
              << " / 4.00\n" << Clr::RESET;

    // Remark
    std::string remark, badge;
    if      (cgpa >= 3.9) { remark = "Summa Cum Laude";  badge = Clr::GREEN;  }
    else if (cgpa >= 3.7) { remark = "Magna Cum Laude";  badge = Clr::GREEN;  }
    else if (cgpa >= 3.5) { remark = "Cum Laude";        badge = Clr::CYAN;   }
    else if (cgpa >= 3.0) { remark = "Good Standing";    badge = Clr::CYAN;   }
    else if (cgpa >= 2.0) { remark = "Satisfactory";     badge = Clr::YELLOW; }
    else                   { remark = "Academic Warning"; badge = Clr::RED;    }

    std::cout << "  Standing : " << badge << Clr::BOLD << remark << Clr::RESET << "\n\n";

    // Progress bar
    std::cout << "  " << gpaColor(cgpa) << gpaBar(cgpa) << Clr::RESET << "\n\n";

    // Grade scale legend
    std::cout << Clr::DIM << "  Grade Scale Reference\n";
    std::cout << "  A+/A=4.0  A-=3.7  B+=3.3  B=3.0  B-=2.7\n";
    std::cout << "  C+=2.3  C=2.0  C-=1.7  D+=1.3  D=1.0  F=0.0\n" << Clr::RESET;
    separator('─');
}

// ═══════════════════════════════════════════════════════════
//  5.  SEMESTER INPUT
// ═══════════════════════════════════════════════════════════

Semester inputSemester(int semNum)
{
    Semester sem;
    sem.number = semNum;
    sem.label  = "Semester " + std::to_string(semNum);
    sem.totalCredits       = 0;
    sem.totalQualityPoints = 0;

    std::cout << "\n" << Clr::MAG << Clr::BOLD
              << "  ── " << sem.label << " ──────────────────────────────\n"
              << Clr::RESET;

    int n = getInt("  Number of courses (1-15): ", 1, 15);

    for (int i = 0; i < n; i++) {
        Course c;
        std::cout << "\n  Course " << (i + 1) << " of " << n << "\n";

        // Course name
        std::cout << "  Name (or press Enter for 'Course " << (i+1) << "'): ";
        std::getline(std::cin, c.name);
        if (c.name.empty()) c.name = "Course " + std::to_string(i + 1);

        // Grade
        GradeInfo gi = getGrade();
        c.letterGrade  = gi.letter;
        c.gradePoints  = gi.points;

        // Credit hours
        c.creditHours  = getDouble("  Credit hours (0.5-6): ", 0.5, 6.0);

        // Compute quality points
        c.qualityPoints = c.gradePoints * c.creditHours;

        sem.courses.push_back(c);
        sem.totalCredits       += c.creditHours;
        sem.totalQualityPoints += c.qualityPoints;
    }

    // GPA for this semester
    sem.gpa = (sem.totalCredits > 0)
              ? sem.totalQualityPoints / sem.totalCredits
              : 0.0;

    return sem;
}

// ═══════════════════════════════════════════════════════════
//  6.  MAIN
// ═══════════════════════════════════════════════════════════

int main()
{
    banner();

    int numSemesters = getInt("  How many semesters? (1-12): ", 1, 12);

    std::vector<Semester> semesters;
    semesters.reserve(numSemesters);

    double cumulativeCredits = 0;
    double cumulativeQP      = 0;

    // ── Collect all semester data ──────────────────────────
    for (int s = 1; s <= numSemesters; s++)
        semesters.push_back(inputSemester(s));

    // ── Display results ────────────────────────────────────
    std::cout << "\n\n";
    separator('═');
    std::cout << Clr::BOLD << Clr::WHITE << "  DETAILED RESULTS\n" << Clr::RESET;
    separator('═');

    for (const auto& sem : semesters) {
        printSemesterTable(sem);
        cumulativeCredits += sem.totalCredits;
        cumulativeQP      += sem.totalQualityPoints;
    }

    // ── Compute CGPA ───────────────────────────────────────
    double cgpa = (cumulativeCredits > 0) ? cumulativeQP / cumulativeCredits : 0.0;

    printCGPASummary(semesters, cgpa, cumulativeCredits, cumulativeQP);

    return 0;
}
