#include <windows.h>
#include <stdio.h>
#include <conio.h>
wchar_t* cw(const char *arry)
{
	wchar_t *w_s = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, arry, -1, w_s, 4096);
	return w_s;
}
DWORD main(int argc, char *argv[])
{
	/*HANDLE c;
	c = OpenEvent(EVENT_ALL_ACCESS, true, cw("asd"));
	SetEvent(c);*/
	// ������������� ������ Pipe
	HANDLE hNamedPipe;

	// ���������� ����, ���������� ����� �����
	DWORD  cbWritten;

	// ���������� ����, �������� ����� �����
	DWORD  cbRead;

	// ����� ��� �������� ������
	char   szBuf[256];

	// ����� ��� ����� ������ Pipe
	char   szPipeName[256];


	printf("Syntax: pipec [servername]\n");

	// ���� ��� ������� PIPEC ���� ������� ��� ������,
	// ��������� ��� � ����� ������ Pipe
	if (argc > 1)
		sprintf_s(szPipeName, "\\\\%s\\pipe\\$MyPipe$",
			argv[1]);

	// ���� ��� ������� ������ �� ����, ������� �����
	// � ��������� ���������
	else
		strcpy_s(szPipeName, "\\\\.\\pipe\\$MyPipe$");

	// ������� ����� � ��������� PIPES
	hNamedPipe = CreateFile(
		cw(szPipeName), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);

	// ���� �������� ������, ������� �� ��� � 
	// ��������� ������ ����������
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		fprintf(stdout, "CreateFile: Error %ld\n",
			GetLastError());
		_getch();
		return 0;
	}

	// ������� ��������� � �������� ������
	printf_s( "\nConnected. Type 'exit' to terminate\n");

	// ���� ������ ������� � ��������� ���������
	while (1)
	{
		// ������� ����������� ��� ����� �������
		printf("cmd>");

		// ������ ��������� ������
		gets_s(szBuf);

		// �������� ��������� ������ ���������� ��������
		// � �������� �������
		if (!WriteFile(hNamedPipe, szBuf, strlen(szBuf) + 1,
			&cbWritten, NULL))
			break;
		//ResetEvent(c);

		// �������� ��� �� ������� ������� �� �������
		if (ReadFile(hNamedPipe, szBuf, 512, &cbRead, NULL))
			printf("Received back: <%s>\n", szBuf);

		// ���� ��������� ������, ������� �� ��� �
		// ��������� ������ ����������
		else
		{
			fprintf(stdout, "ReadFile: Error %ld\n",
				GetLastError());
			_getch();
			break;
		}

		// � ����� �� ������� "exit" ��������� ����
		// ������ ������� � ��������� ���������
		if (!strcmp(szBuf, "exit"))
			break;
	}

	// ��������� ������������� ������
	CloseHandle(hNamedPipe);
	return 0;
}
