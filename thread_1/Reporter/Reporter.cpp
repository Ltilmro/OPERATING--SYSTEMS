#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
    string reportname = argv[2];
    int wage= stoi(argv[3]);
    sout.open(reportname);
    sout << filename << " report:\n";
    vector<Employee> e;
    ifstream in;
    in.open(filename, ios::binary);
    if (in.is_open()) {
        do {
            in.read((char*)&emp, sizeof(Employee));
            e.push_back(emp);
        } while (!in.eof());
        e.pop_back();
    }
    for (Employee i : e)
        sout << to_string(i.num) << " " << i.name << " " << i.hours << " hours $" << i.hours * wage<<"\n";
    sout.close();
}