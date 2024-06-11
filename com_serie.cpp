#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm> // For max_element and transform
#include <numeric>   // For accumulate

using namespace std;

void send_command(HANDLE& hSerial, const string& command) {
    string full_command = command + "\r\n";
    DWORD bytes_written;
    WriteFile(hSerial, full_command.c_str(), full_command.length(), &bytes_written, NULL);
    this_thread::sleep_for(chrono::milliseconds(100)); // Wait for the command to be processed
}

string read_response(HANDLE& hSerial) {
    char buffer[256];
    DWORD bytes_read;
    string response;
    while (ReadFile(hSerial, buffer, 1, &bytes_read, NULL) && bytes_read > 0) {
        if (buffer[0] == '\n') break;
        response += buffer[0];
    }
    return response;
}

int move(HANDLE& hSerial, const string& coordinates, int pre_nn_time) {
    int previous = pre_nn_time;
    stringstream ss(coordinates);
    vector<string> coordinates_mm;
    string coord;
    while (ss >> coord) {
        coordinates_mm.push_back(coord);
    }
    vector<string> coordinates_um;
    for (const auto& coord_mm : coordinates_mm) {
        coordinates_um.push_back(to_string(stof(coord_mm) * 1000));
    }
    int nn_time = abs(stoi(*max_element(coordinates_mm.begin(), coordinates_mm.end())) - previous);
    cout << "le nn_time = " << nn_time << endl;
    string command = accumulate(next(coordinates_um.begin()), coordinates_um.end(), coordinates_um[0], [](string a, string b) { return a + " " + b; }) + " move";
    send_command(hSerial, command);
    string response = read_response(hSerial);
    cout << "Response: " << response << endl;
    this_thread::sleep_for(chrono::milliseconds(nn_time));
    return nn_time;
}

int main() {
    // Open the serial port
    HANDLE hSerial = CreateFileA(
        "\\\\.\\COM1", // Adjust the COM port number as needed
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        cerr << "Error opening serial port" << endl;
        return 1;
    }

    // Configure the serial port
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Error getting serial port state" << endl;
        return 1;
    }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        cerr << "Error setting serial port state" << endl;
        return 1;
    }

    this_thread::sleep_for(chrono::seconds(2)); // Wait for the serial connection to initialize
    int last_nn = 0;

    try {
        while (true) {
            cout << "Enter a Venus-1 command name (s to stop): ";
            string command;
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "move") {
                while (true) {
                    cout << "Enter 'move' command coordinates in mm (space-separated) or type 'exit' to quit: ";
                    string move_coor;
                    getline(cin, move_coor);
                    if (move_coor == "exit") break;
                    last_nn = move(hSerial, move_coor, last_nn);
                }
            } else if (command == "s") {
                break;
            } else {
                send_command(hSerial, command);
                string response = read_response(hSerial);
                cout << "Response: " << response << endl;
            }
        }
    } catch (const std::exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    }

    CloseHandle(hSerial);
    return 0;
}
