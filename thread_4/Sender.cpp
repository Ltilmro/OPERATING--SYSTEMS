#include <iostream> 
#include <fstream> 
#include <string> 
#include "Windows.h" 
using namespace std;

int main(int argc, char* argv[]) {
    ofstream binOutputStream;
    string filename{ argv[1] };
    string message;
    char messageCharset[20]{ };
    int option{ 0 };
    int proccessID{ stoi(argv[2]) };
    HANDLE hReadAccessSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Read Access Semaphore");
    HANDLE hWriteAccessSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Write Access Semaphore");
    HANDLE senderStartEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)("START PROCESS" + proccessID));

    SetEvent(senderStartEvent);
    cout << "Sender process number " << proccessID << endl;

    while (option != 2) {
        WaitForSingleObject(hWriteAccessSemaphore, INFINITE);
        cout << "1. Write message\n" << "2. Exit\n";
        cin >> option;
        switch (option) {
        case 1:
            option = 0;
            cout << "Input message: ";
            cin >> message;
            for (int i = 0; i < 20; i++)
                messageCharset[i] = ' ';
            message.copy(messageCharset, message.length());
            binOutputStream.open(filename, ios::binary | ios::app);
            binOutputStream.write((char*)&messageCharset, sizeof(messageCharset));
            binOutputStream.close();
            ReleaseSemaphore(hReadAccessSemaphore, 1, NULL);
            break;
        case 2:
            cout << "Process stopped";
            Sleep(500);
            break;
        }
    }
    return 0;
}
