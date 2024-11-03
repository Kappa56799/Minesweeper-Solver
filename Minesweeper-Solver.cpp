#include <windows.h>
#include <iostream>
#include <vector>
#include <iomanip>

// Minesweeper memory addresses gotten from Cheat Engine and IDA Pro
#define start 0x1005340
#define end 0x10056A0

DWORD pid = 0;

int main() {
    // Find Minesweeper window
    HWND hwnd = FindWindowA(0, "Minesweeper");
    if (!hwnd) {
        std::cerr << "Minesweeper window not found!" << std::endl;
        return 1;
    }

    // Get Minesweeper process ID
    GetWindowThreadProcessId(hwnd, &pid);
    if (!pid) {
        std::cerr << "Could not get process ID." << std::endl;
        return 1;
    }

    // Open Minesweeper process
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!pHandle) {
        std::cerr << "Could not open process." << std::endl;
        return 1;
    }

    // We use WriteProcessMemory to write a new instruction to the target address, the target ip is 0x010036FA which is where
    // the mines are placed when the game starts. Instead of the instruction that places mines that we cannot see, we replace it
    // with shellcode that shows us where the game would place the mines. The shellcode is mov byte ptr [eax], 0x8A.
    LPCVOID targetAddress = (LPCVOID)0x010036FA; // Address gotten from Cheat Engine
    BYTE newInstruction[] = {0xC6, 0x00, 0x8A};         
    SIZE_T bytesWritten;

    // Write new instruction to target address
    if (WriteProcessMemory(pHandle, (LPVOID)targetAddress, newInstruction, sizeof(newInstruction), &bytesWritten)) {
        std::cout << "Successfully wrote to process memory!" << std::endl;
    } else {
        std::cerr << "Failed to write to process memory." << std::endl;
    }

    // Calculate the game board memory size with the start and end addresses gotten from cheat engine and Ida Pro
    DWORD size = end - start;
    std::vector<BYTE> buffer(size); // Buffer to store memory data
    SIZE_T bytesRead;

    // Read game board memory
    if (ReadProcessMemory(pHandle, (LPCVOID)start, buffer.data(), size, &bytesRead)) {
        std::cout << "Successfully read " << bytesRead << " bytes from process memory:" << std::endl;

        SIZE_T bytesWritten;

        // Replace 0x8A (mine we highlighted earlier) with 0x0E (flags) in the game board memory making it so the user wins the game by clicking 1 non mine tile.
        for (SIZE_T i = 0; i < bytesRead; ++i) {
            // if mine in minesweeper game board
            if (buffer[i] == 0x8A) {
                BYTE newValue = 0x0E;
                
                LPVOID targetAddress = (LPVOID)(start + i);
                // replace mine with flag
                if (WriteProcessMemory(pHandle, targetAddress, &newValue, sizeof(newValue), &bytesWritten)) {
                    std::cout << "Replaced 0x8A (mine) with 0x0E (flag) at address " << std::hex << targetAddress << std::endl;
                } else {
                    std::cerr << "Failed to write to process memory at address " << std::hex << targetAddress << std::endl;
                }
            }
        }
    } else {
        std::cerr << "Failed to read process memory." << std::endl;
    }

    // Close Minesweeper process
    CloseHandle(pHandle);

    return 0;
}
