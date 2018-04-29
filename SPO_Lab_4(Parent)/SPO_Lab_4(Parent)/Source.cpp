#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <thread>
#define CountOfThreads 10
int number = 0;
using namespace std;
CRITICAL_SECTION cs;
wchar_t* cw(const char *arry)
{
	wchar_t *w_s = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, arry, -1, w_s, 4096);
	return w_s;
}

void ThreadProcedure(int id)
{
	try{
		while (id<number) {
			EnterCriticalSection(&cs);
			for (size_t i = 0; i < 5; i++)
			{
				cout << "thread " << id << endl;
				Sleep(1000);
			}
			LeaveCriticalSection(&cs);
			Sleep(2000);
		}
	}
	catch (...)
	{
		cout << "error thread " << id << endl;
	}
	ExitThread(0);
}

 
void main()
{
	InitializeCriticalSection(&cs);
	thread threadArray[CountOfThreads];
	char ch;
 	while (1)
	{
		ch = _getch();
		if (ch == 'q')
		{
			DeleteCriticalSection(&cs);
			for (size_t i = 0; i < number; i++)
			{
				threadArray[number - 1].~thread();
			}
			exit(0);
		}
		if (ch == '+')
		{
			if (number < CountOfThreads)
			{
				number++;
				threadArray[number-1]=thread(ThreadProcedure,number-1);
				threadArray[number - 1].detach();
			}
		}
		if (ch == '-')
		{
			if (number > 0)
			{
				number--;
				threadArray[number].~thread();;
			}
	    }
	}
	_getch();
}


