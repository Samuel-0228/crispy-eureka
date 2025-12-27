#include <iostream>
#include <string>

using namespace std;

const int MAX_STUDENTS = 100;
const int MAX_COURSES = 10;
const int MAX_ENROLL = 100;
const int MAX_GRADES = 300;
const int MAX_ATTEND = 300;

// ----------------- CLASSES -----------------
class Student
{
public:
    string id;
    string name;
    string department;
    int age;
    string phonenumber;

    // reads everything EXCEPT id
    void inputRestFields()
    {
        cout << "Enter student name: ";
        getline(cin, name);

        cout << "Enter department: ";
        getline(cin, department);
        cin.ignore();

        cout << "Age: ";
        while (!(cin >> age))
        {                            // Loops until successful read
            cin.clear();             // Reset error state
            cin.ignore(10000, '\n'); // Flush bad input
            cout << "Invalid Age! Enter an integer: ";
        }
        cin.ignore();

        cout << "Phone number: ";
        getline(cin, phonenumber);
    }

    void updateStudent()
    {
        cout << "Enter new name (or - to keep): ";
        string newName;
        getline(cin, newName);
        if (newName != "-")
            name = newName;

        cout << "Enter new department (or - to keep): ";
        string newDept;
        getline(cin, newDept);
        if (newDept != "-")
            department = newDept;

        cout << "Enter new age (or -1 to keep): ";
        int newAge;
        cin >> newAge;
        cin.ignore();
        if (newAge != -1)
            age = newAge;

        cout << "Enter new phone (or - to keep): ";
        string newPhone;
        getline(cin, newPhone);
        if (newPhone != "-")
            phonenumber = newPhone;
    }

    void displayStudent()
    {
        cout << "ID: " << id
             << ", Name: " << name
             << ", Dept: " << department
             << ", Age: " << age
             << ", Phone: " << phonenumber << endl;
    }
};

class Course
{
public:
    string courseCode;
    string courseName;
    int creditHours;

    void inputCourse()
    {
        cout << "Enter course code: ";
        cin >> courseCode;
        cin.ignore();

        cout << "Enter course name: ";
        getline(cin, courseName);

        cout << "Enter credit hours: ";
        cin >> creditHours;
        cin.ignore();
    }

    void updateCourse()
    {
        cout << "Enter new course name (or - to keep): ";
        string newName;
        getline(cin, newName);
        if (newName != "-")
            courseName = newName;

        cout << "Enter new credit hours (or -1 to keep): ";
        int ch;
        cin >> ch;
        cin.ignore();
        if (ch != -1)
            creditHours = ch;
    }

    void displayCourse()
    {
        cout << "Code: " << courseCode
             << ", Name: " << courseName
             << ", Credit hours: " << creditHours << endl;
    }
};

class Enrollment
{
public:
    string studentId;
    string courseCode;
};

class GradeRecord
{
public:
    string studentId;
    string courseCode;
    double numericGrade;
    char letterGrade;
};

class AttendanceRecord
{
public:
    string studentId;
    string courseCode;
    int presents;
    int absents;
};

// ----------------- GLOBAL ARRAYS -----------------
Student students[MAX_STUDENTS];
Course courses[MAX_COURSES];
Enrollment enrollments[MAX_ENROLL];
GradeRecord grades[MAX_GRADES];
AttendanceRecord attendanceArr[MAX_ATTEND];

int studentCount = 0;
int courseCount = 0;
int enrollCount = 0;
int gradeCount = 0;
int attendCount = 0;

// ----------------- HELPERS -----------------
int findStudentIndexById(string id)
{
    for (int i = 0; i < studentCount; i++)
        if (students[i].id == id)
            return i;
    return -1;
}

int findCourseIndexByCode(string code)
{
    for (int i = 0; i < courseCount; i++)
        if (courses[i].courseCode == code)
            return i;
    return -1;
}

bool isStudentEnrolledInCourse(const string &sid, const string &ccode)
{
    for (int i = 0; i < enrollCount; i++)
    {
        if (enrollments[i].studentId == sid &&
            enrollments[i].courseCode == ccode)
            return true;
    }
    return false;
}

int findAttendanceIndex(const string &sid, const string &ccode)
{
    for (int i = 0; i < attendCount; i++)
    {
        if (attendanceArr[i].studentId == sid &&
            attendanceArr[i].courseCode == ccode)
            return i;
    }
    return -1;
}

double letterToPoint(char g)
{
    switch (g)
    {
    case 'A':
    case 'a':
        return 4.0;
    case 'B':
    case 'b':
        return 3.0;
    case 'C':
    case 'c':
        return 2.0;
    case 'D':
    case 'd':
        return 1.0;
    default:
        return 0.0;
    }
}

// ----------------- STUDENT FUNCTIONS -----------------
void addStudent()
{
    if (studentCount >= MAX_STUDENTS)
    {
        cout << "Student list full!\n";
        return;
    }

    cout << "\n--- Add Student ---\n";

    Student s;

    // read ID first
    cout << "Enter student ID: ";
    cin >> s.id;
    cin.ignore();

    // check duplicate ID
    if (findStudentIndexById(s.id) != -1)
    {
        cout << "Student with this ID already registered.\n";
        cout << "Please use 'Update Student' option instead.\n";
        return; // STOP here, do not read more data
    }

    // only if ID is new, read rest of fields
    s.inputRestFields();

    students[studentCount] = s;
    studentCount++;
    cout << "Student registered successfully.\n";
}

void listStudents()
{
    cout << "\n--- All Students ---\n";
    if (studentCount == 0)
    {
        cout << "No students.\n";
        return;
    }
    for (int i = 0; i < studentCount; i++)
        students[i].displayStudent();
}

void searchStudent()
{
    string id;
    cout << "Enter student ID to search: ";
    cin >> id;
    int idx = findStudentIndexById(id);
    if (idx == -1)
        cout << "Student not found.\n";
    else
        students[idx].displayStudent();
}

void updateStudentById()
{
    string id;
    cout << "Enter student ID to update: ";
    cin >> id;
    cin.ignore();
    int idx = findStudentIndexById(id);
    if (idx == -1)
    {
        cout << "Student not found.\n";
        return;
    }
    students[idx].updateStudent();
    cout << "Student updated.\n";
}

void deleteStudentById()
{
    string id;
    cout << "Enter student ID to delete: ";
    cin >> id;
    int idx = findStudentIndexById(id);
    if (idx == -1)
    {
        cout << "Student not found.\n";
        return;
    }

    // remove enrollments of this student
    for (int i = 0; i < enrollCount;)
    {
        if (enrollments[i].studentId == id)
        {
            enrollments[i] = enrollments[enrollCount - 1];
            enrollCount--;
        }
        else
            i++;
    }

    students[idx] = students[studentCount - 1];
    studentCount--;
    cout << "Student deleted.\n";
}

// ----------------- COURSE FUNCTIONS -----------------
void addCourse()
{
    if (courseCount >= MAX_COURSES)
    {
        cout << "Course list full!\n";
        return;
    }
    cout << "\n--- Add Course ---\n";
    courses[courseCount].inputCourse();
    courseCount++;
}

void listCourses()
{
    cout << "\n--- All Courses ---\n";
    if (courseCount == 0)
    {
        cout << "No courses.\n";
        return;
    }
    for (int i = 0; i < courseCount; i++)
        courses[i].displayCourse();
}

void searchCourse()
{
    string code;
    cout << "Enter course code: ";
    cin >> code;
    int idx = findCourseIndexByCode(code);
    if (idx == -1)
        cout << "Course not found.\n";
    else
        courses[idx].displayCourse();
}

void updateCourseByCode()
{
    string code;
    cout << "Enter course code to update: ";
    cin >> code;
    cin.ignore();
    int idx = findCourseIndexByCode(code);
    if (idx == -1)
    {
        cout << "Course not found.\n";
        return;
    }
    courses[idx].updateCourse();
    cout << "Course updated.\n";
}

void deleteCourseByCode()
{
    string code;
    cout << "Enter course code to delete: ";
    cin >> code;
    int idx = findCourseIndexByCode(code);
    if (idx == -1)
    {
        cout << "Course not found.\n";
        return;
    }

    // remove enrollments of this course
    for (int i = 0; i < enrollCount;)
    {
        if (enrollments[i].courseCode == code)
        {
            enrollments[i] = enrollments[enrollCount - 1];
            enrollCount--;
        }
        else
            i++;
    }

    courses[idx] = courses[courseCount - 1];
    courseCount--;
    cout << "Course deleted.\n";
}

// ----------------- ENROLLMENT -----------------
void enrollStudentInCourse()
{
    if (enrollCount >= MAX_ENROLL)
    {
        cout << "Enrollment list full!\n";
        return;
    }

    string sid, ccode;
    cout << "Enter student ID: ";
    cin >> sid;
    cout << "Enter course code: ";
    cin >> ccode;

    int sIdx = findStudentIndexById(sid);
    int cIdx = findCourseIndexByCode(ccode);

    if (sIdx == -1)
    {
        cout << "Student not found.\n";
        return;
    }
    if (cIdx == -1)
    {
        cout << "Course not found.\n";
        return;
    }
    if (isStudentEnrolledInCourse(sid, ccode))
    {
        cout << "Already enrolled.\n";
        return;
    }

    enrollments[enrollCount].studentId = sid;
    enrollments[enrollCount].courseCode = ccode;
    enrollCount++;
    cout << "Enrollment successful.\n";
}

void listEnrollments()
{
    cout << "\n--- All Enrollments ---\n";
    if (enrollCount == 0)
    {
        cout << "No enrollments.\n";
        return;
    }
    for (int i = 0; i < enrollCount; i++)
    {
        cout << "Student ID: " << enrollments[i].studentId
             << " -> Course: " << enrollments[i].courseCode << endl;
    }
}

// ----------------- GRADES -----------------
void addOrUpdateGrade()
{
    if (gradeCount >= MAX_GRADES)
    {
        cout << "Grade list full!\n";
        return;
    }

    string sid, ccode;
    cout << "Enter student ID: ";
    cin >> sid;
    cout << "Enter course code: ";
    cin >> ccode;

    if (!isStudentEnrolledInCourse(sid, ccode))
    {
        cout << "Student is not enrolled in this course.\n";
        return;
    }

    int idx = -1;
    for (int i = 0; i < gradeCount; i++)
    {
        if (grades[i].studentId == sid &&
            grades[i].courseCode == ccode)
        {
            idx = i;
            break;
        }
    }

    double num;
    char let;
    cout << "Enter numeric grade (0-100): ";
    cin >> num;
    cout << "Enter letter grade (A-F): ";
    cin >> let;

    if (idx == -1)
    {
        grades[gradeCount].studentId = sid;
        grades[gradeCount].courseCode = ccode;
        grades[gradeCount].numericGrade = num;
        grades[gradeCount].letterGrade = let;
        gradeCount++;
    }
    else
    {
        grades[idx].numericGrade = num;
        grades[idx].letterGrade = let;
    }
    cout << "Grade saved.\n";
}

void viewTranscript()
{
    string sid;
    cout << "Enter student ID: ";
    cin >> sid;

    int sIdx = findStudentIndexById(sid);
    if (sIdx == -1)
    {
        cout << "Student not found.\n";
        return;
    }

    cout << "\n--- Transcript for " << students[sIdx].name << " ---\n";
    bool has = false;
    for (int i = 0; i < gradeCount; i++)
    {
        if (grades[i].studentId == sid)
        {
            int cIdx = findCourseIndexByCode(grades[i].courseCode);
            if (cIdx != -1)
            {
                cout << courses[cIdx].courseCode << " - "
                     << courses[cIdx].courseName
                     << " | Credit: " << courses[cIdx].creditHours
                     << " | Num: " << grades[i].numericGrade
                     << " | Letter: " << grades[i].letterGrade << endl;
                has = true;
            }
        }
    }
    if (!has)
        cout << "No grades recorded.\n";
}

// ----------------- ATTENDANCE -----------------
void recordAttendance()
{
    if (attendCount >= MAX_ATTEND)
    {
        cout << "Attendance list full!\n";
        return;
    }

    string sid, ccode;
    cout << "Enter student ID: ";
    cin >> sid;
    cout << "Enter course code: ";
    cin >> ccode;

    if (!isStudentEnrolledInCourse(sid, ccode))
    {
        cout << "Student is not enrolled in this course.\n";
        return;
    }

    char status;
    cout << "Enter status (P = present, A = absent): ";
    cin >> status;

    int idx = findAttendanceIndex(sid, ccode);
    if (idx == -1)
    {
        attendanceArr[attendCount].studentId = sid;
        attendanceArr[attendCount].courseCode = ccode;
        attendanceArr[attendCount].presents = 0;
        attendanceArr[attendCount].absents = 0;
        idx = attendCount;
        attendCount++;
    }

    if (status == 'P' || status == 'p')
        attendanceArr[idx].presents++;
    else
        attendanceArr[idx].absents++;

    cout << "Attendance recorded.\n";
}

void viewAttendanceForStudentCourse()
{
    string sid, ccode;
    cout << "Enter student ID: ";
    cin >> sid;
    cout << "Enter course code: ";
    cin >> ccode;

    int idx = findAttendanceIndex(sid, ccode);
    if (idx == -1)
    {
        cout << "No attendance record.\n";
        return;
    }
    cout << "Presents: " << attendanceArr[idx].presents
         << ", Absents: " << attendanceArr[idx].absents << endl;
}

// ----------------- GPA -----------------
double computeGPAForStudent(const string &sid)
{
    double totalPoints = 0;
    int totalCredits = 0;

    for (int i = 0; i < gradeCount; i++)
    {
        if (grades[i].studentId == sid)
        {
            int cIdx = findCourseIndexByCode(grades[i].courseCode);
            if (cIdx != -1)
            {
                double gp = letterToPoint(grades[i].letterGrade);
                totalPoints += gp * courses[cIdx].creditHours;
                totalCredits += courses[cIdx].creditHours;
            }
        }
    }

    if (totalCredits == 0)
        return 0.0;
    return totalPoints / totalCredits;
}

void showGPAForStudent()
{
    string sid;
    cout << "Enter student ID: ";
    cin >> sid;

    int sIdx = findStudentIndexById(sid);
    if (sIdx == -1)
    {
        cout << "Student not found.\n";
        return;
    }

    double gpa = computeGPAForStudent(sid);
    cout << "GPA for " << students[sIdx].name << " = " << gpa << endl;
}

// ----------------- SUB‑MENUS -----------------
void studentMenu()
{
    int ch;
    do
    {
        cout << "\n--- Student Menu ---\n";
        cout << "1. Add Student\n";
        cout << "2. List Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch)
        {
        case 1:
            addStudent();
            break;
        case 2:
            listStudents();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            updateStudentById();
            break;
        case 5:
            deleteStudentById();
            break;
        case 0:
            break;
        default:
            cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void courseMenu()
{
    int ch;
    do
    {
        cout << "\n--- Course Menu ---\n";
        cout << "1. Add Course\n";
        cout << "2. List Courses\n";
        cout << "3. Search Course\n";
        cout << "4. Update Course\n";
        cout << "5. Delete Course\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch)
        {
        case 1:
            addCourse();
            break;
        case 2:
            listCourses();
            break;
        case 3:
            searchCourse();
            break;
        case 4:
            updateCourseByCode();
            break;
        case 5:
            deleteCourseByCode();
            break;
        case 0:
            break;
        default:
            cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void enrollmentMenu()
{
    int ch;
    do
    {
        cout << "\n--- Enrollment Menu ---\n";
        cout << "1. Enroll Student in Course\n";
        cout << "2. List Enrollments\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch)
        {
        case 1:
            enrollStudentInCourse();
            break;
        case 2:
            listEnrollments();
            break;
        case 0:
            break;
        default:
            cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void gradeMenu()
{
    int ch;
    do
    {
        cout << "\n--- Grade Menu ---\n";
        cout << "1. Add/Update Grade\n";
        cout << "2. View Transcript\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch)
        {
        case 1:
            addOrUpdateGrade();
            break;
        case 2:
            viewTranscript();
            break;
        case 0:
            break;
        default:
            cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void attendanceMenu()
{
    int ch;
    do
    {
        cout << "\n--- Attendance Menu ---\n";
        cout << "1. Record Attendance\n";
        cout << "2. View Attendance\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch)
        {
        case 1:
            recordAttendance();
            break;
        case 2:
            viewAttendanceForStudentCourse();
            break;
        case 0:
            break;
        default:
            cout << "Invalid.\n";
        }
    } while (ch != 0);
}

void gpaMenu()
{
    int ch;
    do
    {
        cout << "\n--- GPA Menu ---\n";
        cout << "1. Show GPA for a Student\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> ch;
        cin.ignore();

        switch (ch)
        {
        case 1:
            showGPAForStudent();
            break;
        case 0:
            break;
        default:
            cout << "Invalid.\n";
        }
    } while (ch != 0);
}

// ----------------- MAIN MENU -----------------
void runMenu()
{
    int choice;
    do
    {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Student\n";
        cout << "2. Course\n";
        cout << "3. Enrollment\n";
        cout << "4. Grades\n";
        cout << "5. Attendance\n";
        cout << "6. GPA / CGPA\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            studentMenu();
            break;
        case 2:
            courseMenu();
            break;
        case 3:
            enrollmentMenu();
            break;
        case 4:
            gradeMenu();
            break;
        case 5:
            attendanceMenu();
            break;
        case 6:
            gpaMenu();
            break;
        case 0:
            cout << "Goodbye.\n";
            break;
        default:
            cout << "Invalid.\n";
        }
    } while (choice != 0);
}

int main()
{
    runMenu();
    return 0;
}
