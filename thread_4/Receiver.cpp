#include <iostream> 
#include <fstream> 
#include <string> 
#include "Windows.h" 

using namespace std;
int main() {
    STARTUPINFO si{ 0 };
    PROCESS_INFORMATION pi{ 0 };
    ifstream binInputStream;
    string filename;
    string strCmdLineRequest;
    wstring wstrCmdLineRequest;
    LPWSTR lpwCmdLineRequest{ nullptr };
    HANDLE* hProcesses{ nullptr };
    HANDLE* hStartEvents{ nullptr };
    HANDLE hReadAccessSemaphore{ 0 };
    HANDLE hWriteAccessSemaphore{ 0 };
    DWORD response{ 0 };
    const short DELAY{ 25'000 };
    const short messageLength{ 20 };
    char message[20]{ "" };
    int numberOfProcesses{ 0 };
    int numberOfEntries{ 0 };
    int bytesRead{ 0 };
    int option{ 0 };


    cout << "Input .bin file name: ";
    cin >> filename;
    cout << "Input number of entries: ";
    cin >> numberOfEntries;
    cout << "Input number of Sender processes: ";
    cin >> numberOfProcesses;
    hReadAccessSemaphore = CreateSemaphore(NULL, 0, numberOfEntries, L"Read Access Semaphore");
    hWriteAccessSemaphore = CreateSemaphore(NULL, numberOfEntries, numberOfEntries, L"Write Access Semaphore");

    hProcesses = new HANDLE[numberOfProcesses];
    hStartEvents = new HANDLE[numberOfProcesses];
    for (int i = 0; i < numberOfProcesses; i++)
        hStartEvents[i] = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)("START PROCESS" + i));

    for (int i = 0; i < numberOfProcesses; i++) {
        strCmdLineRequest = string("Sender.exe") + " " + filename + " " + to_string(i);
        wstrCmdLineRequest = wstring(strCmdLineRequest.begin(), strCmdLineRequest.end());
        lpwCmdLineRequest = &wstrCmdLineRequest[0];
        if (!CreateProcess(NULL, lpwCmdLineRequest, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
            cout << "Unable to create \"Sender.exe\" process" << endl;
        CloseHandle(pi.hThread);
        hProcesses[i] = pi.hProcess;
    }
    WaitForMultipleObjects(numberOfProcesses, hStartEvents, TRUE, INFINITE);
    while (option != 2) {
        cout << "1. Read message \n" << "2. Exit\n";
        cin >> option;
        switch (option) {
        case 1:
            option = 0;
            binInputStream.open(filename, ios::binary);
            binInputStream.seekg(bytesRead);
            binInputStream.read((char*)&message, sizeof(message));
            cout << "Message: ";
            for (int i = 0; i < messageLength; i++)
                cout << message[i];
            cout << endl;
            binInputStream.close();
            bytesRead += messageLength;
            ReleaseSemaphore(hWriteAccessSemaphore, 1, NULL);
            break;
        case 2:
            cout << "Process stopped";
            Sleep(500);
            break;
        }
    }
    CloseHandle(hReadAccessSemaphore);
    CloseHandle(hWriteAccessSemaphore);
    for (int i = 0; i < numberOfProcesses; i++)
    {
        CloseHandle(hStartEvents[i]);
        CloseHandle(hProcesses[i]);
    }
    delete[] hStartEvents; 
    delete[] hProcesses; 
    hStartEvents = nullptr;
    hProcesses = nullptr;

    return 0;
}
