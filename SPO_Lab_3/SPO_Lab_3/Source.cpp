#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
using namespace std;
HANDLE c;
wchar_t* cw(const char *arry)
{
	wchar_t *w_s = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, arry, -1, w_s, 4096);
	return w_s;
}
void main()
{
	char * name = "asd";
	c = CreateEvent(NULL, false, true, cw(name));

	bool resultOfConnectionWithClient;
	//  оличество байт данных, прин€тых через канал
	DWORD  cbRead;
	//  оличество байт данных, переданных через канал
	DWORD  cbWritten;
	HANDLE handle;
	char *str = "\\\\.\\pipe\\$MyPipe$";//"server";
	char bufer[256];
	handle=CreateNamedPipe(cw(str),PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_WAIT,PIPE_UNLIMITED_INSTANCES,512,512,1000,NULL);

	if (handle == INVALID_HANDLE_VALUE)
	{
		fprintf(stdout, "CreateNamedPipe: Error %ld\n",
			GetLastError());
		_getch();
		return ;
	}
	printf_s("Waiting fot connect...\n");//f
	resultOfConnectionWithClient = ConnectNamedPipe(handle,NULL);
	
	if (!resultOfConnectionWithClient)
	{
		switch (GetLastError())
		{
		case ERROR_NO_DATA:
			printf_s("ConnectNamedPipe: ERROR_NO_DATA");
			_getch();
			CloseHandle(handle);
			return ;
			break;

		case ERROR_PIPE_CONNECTED:
			printf_s(
				"ConnectNamedPipe: ERROR_PIPE_CONNECTED");
			_getch();
			CloseHandle(handle);
			return ;
			break;

		case ERROR_PIPE_LISTENING:
			printf_s(
				"ConnectNamedPipe: ERROR_PIPE_LISTENING");
			_getch();
			CloseHandle(handle);
			return ;
			break;

		case ERROR_CALL_NOT_IMPLEMENTED:
			printf_s(
				"ConnectNamedPipe: ERROR_CALL_NOT_IMPLEMENTED");
			_getch();
			CloseHandle(handle);
			return ;
			break;

		default:
			printf_s("ConnectNamedPipe: Error %ld\n",
				GetLastError());
			_getch();
			CloseHandle(handle);
			return ;
			break;

		}

		CloseHandle(handle);
		_getch();
		return;
	}

	// ¬ыводим сообщение об успешном создании канала
	printf_s("\nConnected. Waiting for command...\n");
	while (1)
	{
		if (ReadFile(handle, bufer, 512, &cbRead, NULL))
		{
			//if(WaitForSingleObject(c,10000))//ждем 1с,если не перешли в несигнальное состо€ни€,мб что client отправил exit
			if (!WriteFile(handle, bufer, strlen(bufer) + 1,
				&cbWritten, NULL))
				break;

			//Sleep(10000);
			printf("Received: <%s>\n", bufer);
			if (!strcmp(bufer, "exit"))
				break;
		}
		else
		{
			fprintf(stdout, "ReadFile: Error %ld\n",
				GetLastError());
			_getch();
			break;
		}
	}
}