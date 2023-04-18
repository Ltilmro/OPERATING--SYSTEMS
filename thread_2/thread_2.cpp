
#include <iostream>
#include <windows.h>
using namespace std;
int* arr;
int sizee;
int sum;
int avg;
int mini{ 0 };
int maxi{ 0 };
DWORD WINAPI minmax(LPVOID par){
    mini = 0;
    maxi = 0;
    for (int i = 0; i < sizee; i++)
    {
        if (arr[mini] > arr[i])
            mini = i;
        Sleep(7);
        if (arr[maxi] < arr[i])
            maxi = i;
        Sleep(7);
    }
    cout << arr[mini] << " " << arr[maxi] <<endl;
    return 0;
}
DWORD WINAPI average(LPVOID par)
{
    sum = 0;
    for (int i = 0; i < sizee; i++)
    {
        sum += arr[i];
        Sleep(12);
    }
    avg = sum / sizee;
    cout << avg << endl;
    return 0;
}
int main()
{
    cin >> sizee;
    arr = new int[sizee];
    for (int i = 0; i < sizee; i++)
    {
        cin >> arr[i];
    }
    HANDLE mm = CreateThread(NULL, 0, minmax, NULL, 0, NULL);
    HANDLE avrg = CreateThread(NULL, 0, average, NULL, 0, NULL);
    WaitForSingleObject(mm,INFINITE);
    WaitForSingleObject(avrg, INFINITE);
    CloseHandle(mm);
    CloseHandle(avrg);
    arr[mini] = avg;
    arr[maxi] = avg;
    for (int i = 0; i < sizee; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}
