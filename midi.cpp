#include <iostream>
#include <windows.h>
#include <mmeapi.h>
#include <mmsystem.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#pragma comment(lib,"Winmm.lib")

using namespace std;

enum Scale//每一个音阶的十进制对应表
{
	Rest = 0, C8 = 108, B7 = 107, A7s = 106, A7 = 105, G7s = 104, G7 = 103, F7s = 102, F7 = 101, E7 = 100,
	D7s = 99, D7 = 98, C7s = 97, C7 = 96, B6 = 95, A6s = 94, A6 = 93, G6s = 92, G6 = 91, F6s = 90, F6 = 89,
	E6 = 88, D6s = 87, D6 = 86, C6s = 85, C6 = 84, B5 = 83, A5s = 82, A5 = 81, G5s = 80, G5 = 79, F5s = 78,
	F5 = 77, E5 = 76, D5s = 75, D5 = 74, C5s = 73, C5 = 72, B4 = 71, A4s = 70, A4 = 69, G4s = 68, G4 = 67,
	F4s = 66, F4 = 65, E4 = 64, D4s = 63, D4 = 62, C4s = 61, C4 = 60, B3 = 59, A3s = 58, A3 = 57, G3s = 56,
	G3 = 55, F3s = 54, F3 = 53, E3 = 52, D3s = 51, D3 = 50, C3s = 49, C3 = 48, B2 = 47, A2s = 46, A2 = 45,
	G2s = 44, G2 = 43, F2s = 42, F2 = 41, E2 = 40, D2s = 39, D2 = 38, C2s = 37, C2 = 36, B1 = 35, A1s = 34,
	A1 = 33, G1s = 32, G1 = 31, F1s = 30, F1 = 29, E1 = 28, D1s = 27, D1 = 26, C1s = 25, C1 = 24, B0 = 23,
	A0s = 22, A0 = 21
};
enum noteTime
{
	Whole_Note = 4000, Half_Note = 2000, Quater_Note = 1000, Eighth_Note = 500
};

vector<int> Note, Voice, Duratin;

HMIDIOUT handle;
const BYTE Note_On = 0x90;
BYTE key[256];

const char K[25] = { 'Z','X','C','V','B','N','M','A','S','D','F','G', 'H','J','Q','W','E','R','T','Y','U' };
const int N[25] = { C3,D3,E3,F3,G3,A3,B3,C4,D4,E4,F4,G4,A4,B4,C5,D5,E5,F5,G5,A5,B5 };
bool keyIfPress [25] = {};


void PlayNote(int note,int Vo,int Duration) {

	int sleep = Duration;

	DWORD msg = Note_On | (note << 8) | (Vo << 16);

	midiOutShortMsg(handle, msg);
	Sleep(sleep);

}

void pri() {

	system("cls");

	cout << "成功打开钢琴！\n"
		<< "下面是键盘与琴键的对应表\n"
		<< "高音区：\n"
		<< "Q:Do W:Re E:Mi R:Fa T:SO Y:La U:Si\n"
		<< "中音区：\n"
		<< "A:Do S:Re D:Mi F:Fa G:SO H:La J:Si\n"
		<< "低音区：\n"
		<< "Z:Do X:Re C:Mi V:Fa B:SO N:La M:Si\n"
		<< "=============================================================================\n"
		<< "想要改变音色吗，请按下Shift吧！\n"
		<< "=============================================================================\n"
		<< "请按+号键退出\n"
		<< "\n\n"
		<< "tips:数字指的是小键盘上的数字\n"
		<< endl;

	return;

}

void pri2() {
	system("cls");
	cout << "1:小星星mini版\n" 
		 << "再次按+号退出" << endl;
}

void read_score() {
	ifstream openMusic;
	openMusic.open("D:\\code\\c++\\midi\\midipiano\\midipiano\\music_score.txt", ios::in);
	string str;
	if (openMusic.is_open()) {
		while (openMusic >> str) {
			if (str == "N") {
				while (1) {
					openMusic >> str;
					if (str == "*") {
						break;
					}
					int i = stoi(str);
					Note.push_back(i);
				}
			}
			if (str == "V") {
				while (1) {
					openMusic >> str;
					if (str == "*") {
						break;
					}
					int i = stoi(str);
					Voice.push_back(i);
				}
			}
			if (str == "D") {
				while (1) {
					openMusic >> str;
					if (str == "*") {
						break;
					}
					int i = stoi(str);
					Duratin.push_back(i);
				}
			}
		}
	}
}

void music() {

	midiOutOpen(&handle, 0, 0, 0, 0);

	pri2();

	bool p = true;

	while (p) {
		if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
			cout << "Start" << endl;
			read_score();
			//cout << "开始播放" << endl;
			//for (int i = 0; i < 7; i++) {
			//	PlayNote(Star[i][0], Star[i][1], Star[i][2]);
			//}
			//cout << "播放完成,即将退出:D" << endl;
			//Sleep(2000);
			//pri2();
			int len = Note.size();
			for (int i = 0; i < len; i++) {
				PlayNote(Note[i],Voice[i],Duratin[i]);
			}
			cout << "播放完成" << endl;
		}
		if (GetAsyncKeyState(VK_ADD) & 0x8000) {
			p = false;
		}
	}

	midiOutClose(handle);

	return;

}

void change(int mes) {

	BYTE byt = 0xC0; // 程序变化消息
	BYTE message = mes; // 要选择的乐器
	DWORD msg = byt | (message << 8);
	midiOutShortMsg(handle, msg);

	cout << "音色切换成功！" << endl;

	Sleep(2000);

	return;
	

	//待优化
}

void piano() {

	midiOutOpen(&handle, 0, 0, 0, 0);

	pri();
	bool p = true;

	while (p) {
		
		for (int i = 0; i < 21; i++) {
			if (GetAsyncKeyState(K[i]) & 0x8000) {
				keyIfPress[i] = true;
			}
		}

		for (int i = 0; i < 21; i++) {
			if (keyIfPress[i]) {
				PlayNote(N[i], 40, 0);
				keyIfPress[i] = false;
			}
		}

		/*
		//Medium
		if (GetAsyncKeyState('A') & 0x8000) {
			midiOutShortMsg(handle, 0x00403c90);
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			midiOutShortMsg(handle, 0x00403e90);
		}
		if (GetAsyncKeyState('D') & 0x8000) {
			midiOutShortMsg(handle, 0x00404090);
		}
		if (GetAsyncKeyState('F') & 0x8000) {
			midiOutShortMsg(handle, 0x00404190);
		}
		if (GetAsyncKeyState('G') & 0x8000) {
			midiOutShortMsg(handle, 0x00404390);
		}
		if (GetAsyncKeyState('H') & 0x8000) {
			midiOutShortMsg(handle, 0x00404590);
		}
		if (GetAsyncKeyState('J') & 0x8000) {
			midiOutShortMsg(handle, 0x00404790);
		}

		//High
		if (GetAsyncKeyState('Q') & 0x8000) {
			midiOutShortMsg(handle, 0x00404890);
		}
		if (GetAsyncKeyState('W') & 0x8000) {
			midiOutShortMsg(handle, 0x00404a90);
		}
		if (GetAsyncKeyState('E') & 0x8000) {
			midiOutShortMsg(handle, 0x00404c90);
		}
		if (GetAsyncKeyState('R') & 0x8000) {
			midiOutShortMsg(handle, 0x00404d90);
		}
		if (GetAsyncKeyState('T') & 0x8000) {
			midiOutShortMsg(handle, 0x00404f90);
		}
		if (GetAsyncKeyState('Y') & 0x8000) {
			midiOutShortMsg(handle, 0x00405190);
		}
		if (GetAsyncKeyState('U') & 0x8000) {
			midiOutShortMsg(handle, 0x00405390);
		}

		//Low
		if (GetAsyncKeyState('Z') & 0x8000) {
			midiOutShortMsg(handle, 0x00403090);
		}
		if (GetAsyncKeyState('X') & 0x8000) {
			midiOutShortMsg(handle, 0x00403290);
		}
		if (GetAsyncKeyState('C') & 0x8000) {
			midiOutShortMsg(handle, 0x00403490);
		}
		if (GetAsyncKeyState('V') & 0x8000) {
			midiOutShortMsg(handle, 0x00403590);
		}
		if (GetAsyncKeyState('B') & 0x8000) {
			midiOutShortMsg(handle, 0x00403790);
		}
		if (GetAsyncKeyState('N') & 0x8000) {
			midiOutShortMsg(handle, 0x00403990);
		}
		if (GetAsyncKeyState('M') & 0x8000) {
			midiOutShortMsg(handle, 0x00403b90);
		}

		*/

		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {

			system("cls");

			cout << "1:经典大钢琴" << endl;
			cout << "2:电钢琴" << endl;
			cout << "3:钟琴" << endl;

			while (1) {
				if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
					change(0);
					break;
				}
				if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
					change(2);
					break;
				}
				if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
					change(10);
					break;
				}
			}

			pri();
			
		}
		
		if (GetAsyncKeyState(VK_ADD) & 0x8000) {
			p = false;
		}

	}



	midiOutClose(handle);

	return;

}

int main() {

	cout << "输入1打开钢琴，输入2打开曲库:D" << endl;
	
	int k = 0;

	cin >> k;

	if (k == 1) {
		piano();
	}
	if (k == 2) {
		music();
	}

	return 0;

}

