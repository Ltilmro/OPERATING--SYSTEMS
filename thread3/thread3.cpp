#include <iostream>
#include <windows.h>
using namespace std;
CRITICAL_SECTION in;
CRITICAL_SECTION ar;
int* arr;
int asize;
int rcnt = 0;
HANDLE* mm;
HANDLE* start;
HANDLE* stop;
HANDLE* impossible;
DWORD WINAPI marker(LPVOID par) {
    WaitForSingleObject(start[(int)par], INFINITE);
    int cnt = 0;
    DWORD e = 0;
    HANDLE flags[]{ start[(int)par], stop[(int)par] };
    srand((int)par);
    int r;
    while (true)
    {
        r = rand() % asize;
        if (arr[r] == 0)
        {
            EnterCriticalSection(&ar);
            Sleep(5);
            arr[r] = (int)par + 1;
            cnt++;
            Sleep(5);
            LeaveCriticalSection(&ar);
            continue;
        }
        else
        {
            EnterCriticalSection(&ar);
            cout << (int)par + 1 << " " << cnt << " " << r << endl;
            LeaveCriticalSection(&ar);
            SetEvent(impossible[(int)par]);
            ResetEvent(start[(int)par]);
            e = WaitForMultipleObjects(2, flags, FALSE, INFINITE);
            if (e == WAIT_OBJECT_0 + 1)
            {
                EnterCriticalSection(&ar);
                for (int x = 0; x < asize; x++)
                {

                    if (arr[x] == int(par) + 1)
                        arr[x] = 0;
                }
                LeaveCriticalSection(&ar);
                ExitThread(0);
            }
            else
            {
                ResetEvent(impossible[(int)par]);
                continue;
            }
        }
    }
}
int main()
{
    bool* running;
    int t = 0;
    InitializeCriticalSection(&ar);
    InitializeCriticalSection(&in);
    int threadcount;
    EnterCriticalSection(&in);
    cout << "Enter array size\n";
    cin >> asize;
    arr = new int[asize];
    running = new bool[asize];
    for (int i = 0; i < asize; i++)
    {
        arr[i] = 0;
    }
    cout << "Enter thread count\n";
    cin >> threadcount;
    for (int i = 0; i < threadcount; i++)
    {
        running[i] = true;
        rcnt++;
    }
    LeaveCriticalSection(&in);
    mm = new HANDLE[threadcount];
    start = new HANDLE[threadcount];
    stop = new HANDLE[threadcount];
    impossible = new HANDLE[threadcount];
    for (int i = 0; i < threadcount; i++) {
        mm[i] = CreateThread(NULL, 1, marker, (LPVOID)(i), NULL, NULL);
        start[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        stop[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        impossible[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    while (rcnt != 0)
    {
        for (int i = 0; i < threadcount; i++) {
            if (running[i])
            {
                ResetEvent(impossible[i]);
                SetEvent(start[i]);
            }
        }
        WaitForMultipleObjects(threadcount, impossible, TRUE, INFINITE);
        EnterCriticalSection(&ar);
        for (int i = 0; i < asize; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
        LeaveCriticalSection(&ar);
        while (true)
        {
            EnterCriticalSection(&in);
            cout << "Enter thread to terminate ";
            cin >> t;
            LeaveCriticalSection(&in);
            if (running[t - 1])
            {
                running[t - 1] = 0;
                rcnt--;
                SetEvent(stop[t - 1]);
                WaitForSingleObject(mm[t - 1], INFINITE);
                CloseHandle(mm[t - 1]);
                CloseHandle(stop[t - 1]);
                CloseHandle(start[t - 1]);
                break;
            }
        }
        EnterCriticalSection(&ar);
        for (int i = 0; i < asize; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
        LeaveCriticalSection(&ar);
    }
}
