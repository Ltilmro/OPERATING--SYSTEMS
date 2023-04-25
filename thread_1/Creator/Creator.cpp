
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct Employee
{
    int num;
    char name[10];
    double hours;
};
int main(int argc, char* argv[])
{
    Employee emp;
    ofstream sout;
    string filename = argv[1];
    int entrynum = stoi(argv[2]);

    sout.open(filename, ios::app | ios::binary);
    if (sout.is_open()) {
        for (int i = 0; i < entrynum; i++) {
            cout << "Enter employee number\n";
            cin >> emp.num;
            cout << "Enter employee name\n";
            cin >> emp.name;
            cout << "Enter number of hours\n";
            cin>> emp.hours;
            sout.write((char*)&emp, sizeof(Employee));
        }
    }
    sout.close();
    return 0;
}