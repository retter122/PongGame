#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
using namespace std;

#define WinSizeX 160 // Cosole window size in symbols
#define WinSizeY 45 // Console window size in symbols
#define BallMaxSpeed 5 // Ball max speed
#define RocketsSize 10 // Rockets size

int BallX = WinSizeX / 2;
int BallY = 1 + (WinSizeY - 1) / 2;

int BallSx = 1 + rand() % (BallMaxSpeed / 2);
int BallSy = BallMaxSpeed - BallSx;

int Rock1Y = 1 + (WinSizeY - 1) / 2 - RocketsSize / 2;
int Rock2Y = 1 + (WinSizeY - 1) / 2 - RocketsSize / 2;

int Sc1, Sc2;

int main() {
	// Set resolution of console
	_SMALL_RECT Rect = {0, 0, WinSizeX - 1, WinSizeY - 1 };
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, _COORD{ WinSizeX, WinSizeY });
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
	// Get console buffer
	wchar_t* screen = new wchar_t[WinSizeX * WinSizeY + 1];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	while (1) {
		// Clear buffer
		for (unsigned int i = 0; i < WinSizeX * WinSizeY; ++i) screen[i] = ' ';
		// Draw interface elements
		screen[WinSizeX] = '+'; screen[WinSizeX * 2 - 1] = '+'; screen[WinSizeX * WinSizeY - 1] = '+'; screen[WinSizeX * (WinSizeY - 1)] = '+';
		for (unsigned int i = WinSizeX + 1; i < WinSizeX * 2 - 1; ++i) {
			screen[i] = '=';
			screen[i + (WinSizeX * (WinSizeY - 2))] = '=';
		}
		screen[WinSizeX + WinSizeX / 2] = '+'; screen[WinSizeX * (WinSizeY - 1) + WinSizeX / 2] = '+';
		for (unsigned int i = 2; i < WinSizeY - 1; ++i) {
			screen[i * WinSizeX] = '|';
			screen[i * WinSizeX + WinSizeX - 1] = '|';
			screen[i * WinSizeX] = '|';
			screen[i * WinSizeX + WinSizeX / 2] = '|';
		}
		// Draw Game
		screen[BallX + BallY * WinSizeX] = 'o';
		for (unsigned int i = 0; i < RocketsSize; ++i) {
			screen[WinSizeX * (Rock1Y + i) + 1] = '#';
			screen[WinSizeX * (Rock2Y + i) + WinSizeX - 2] = '#';
		}
		int p = 0;
		for (auto i : to_wstring(Sc1)) {
			screen[p] = i;
			++p;
		}
		p = WinSizeX / 2 + 1;
		for (auto i : to_wstring(Sc2)) {
			screen[p] = i;
			++p;
		}
		// Game logic
		BallX = min(max(1, BallX + BallSx), WinSizeX - 2); BallY = min(max(2, BallY + BallSy), WinSizeY - 2);
		if (BallY == WinSizeY - 2 || BallY == 2) BallSy = -BallSy;
		if (_kbhit()) {
			switch (_getch()) {
				case ('w'): Rock1Y = max(2, Rock1Y - 3); break;
				case ('s'): Rock1Y = min(WinSizeY - 1 - RocketsSize, Rock1Y + 3); break;
				case ('W'): Rock1Y = max(2, Rock1Y - 3); break;
				case ('S'): Rock1Y = min(WinSizeY - 1 - RocketsSize, Rock1Y + 3); break;
				case ('o'): Rock2Y = max(2, Rock2Y - 3); break;
				case ('l'): Rock2Y = min(WinSizeY - 1 - RocketsSize, Rock2Y + 3); break;
				case ('O'): Rock2Y = max(2, Rock2Y - 3); break;
				case ('L'): Rock2Y = min(WinSizeY - 1 - RocketsSize, Rock2Y + 3); break;
				default: break;
			}
		}
		if (BallX == WinSizeX - 2 && !(BallY > Rock2Y && BallY < Rock2Y + RocketsSize)) {
			BallX = WinSizeX / 2;
			BallY = 1 + (WinSizeY - 1) / 2;
			BallSx = 1 + rand() % (BallMaxSpeed / 2);
			BallSy = BallMaxSpeed - BallSx;
			++Sc1;
		}
		else if (BallX == WinSizeX - 2) {
			BallX = WinSizeX - 3;
			BallSx = -BallSx;
		}
		if (BallX == 1 && !(BallY > Rock1Y && BallY < Rock1Y + RocketsSize)) {
			BallX = WinSizeX / 2;
			BallY = 1 + (WinSizeY - 1) / 2;
			BallSx = 1 + rand() % (BallMaxSpeed / 2);
			BallSy = BallMaxSpeed - BallSx;
			++Sc2;
		}
		else if (BallX == 1) {
			BallX = 2;
			BallSx = -BallSx;
		}
		// Copy image to console
		screen[WinSizeX * WinSizeY] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, WinSizeX * WinSizeY, { 0, 0 }, &dwBytesWritten);
		Sleep(100);
	}
}