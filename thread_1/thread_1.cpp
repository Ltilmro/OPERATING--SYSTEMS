
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
using namespace std;
struct Employee
{
    int num;
    char name[10];
    double hours;
};
void binOut(string fname)
{
    Employee emp;
    vector<Employee> e;
    ifstream in;
    in.open(fname,ios::binary);
    if (in.is_open()) {
        do {
            in.read((char*)&emp, sizeof(Employee));
            e.push_back(emp);
        } while (!in.eof());
        e.pop_back();
    }
    for (Employee i : e)
        cout << i.num << " " << i.name << " " << i.hours << " hours\n";
}
void repOut(string fname)
{
    ifstream sout;
    sout.open(fname);
    string s;
    while (getline(sout,s))
        cout << s<<endl;
}
int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    string filename;
    int entry_num;
    cout << "Enter binary file name\n";
    cin >> filename;
    filename += ".bin";
    cout << "Enter number of entries\n";
    cin >> entry_num;
    string creatorString = string(".\\Creator\\Debug\\Creator.exe") + " " + filename + " "+to_string(entry_num);
    wstring wCreator(creatorString.begin(), creatorString.end());
    LPWSTR lpwCreator = &wCreator[0];
    CreateProcess(NULL, lpwCreator,NULL, NULL,FALSE,CREATE_NEW_CONSOLE,NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hThread, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    binOut(filename);
    string reportname;
    int wage;
    cout << "Enter report file name\n";
    cin >> reportname;
    reportname += ".txt";
    cout << "Enter hourly wage\n";
    cin >> wage;
    string reporterString = string(".\\Reporter\\Debug\\Reporter.exe") + " " + filename + " " + reportname + " " + to_string(wage);
    wstring wReporter(reporterString.begin(), reporterString.end());
    LPWSTR lpwReporter = &wReporter[0];
    CreateProcess(NULL, lpwReporter, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hThread, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    repOut(reportname);
    return 0;

}
