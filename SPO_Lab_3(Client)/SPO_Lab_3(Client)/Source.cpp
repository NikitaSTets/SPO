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
	// Идентификатор канала Pipe
	HANDLE hNamedPipe;

	// Количество байт, переданных через канал
	DWORD  cbWritten;

	// Количество байт, принятых через канал
	DWORD  cbRead;

	// Буфер для передачи данных
	char   szBuf[256];

	// Буфер для имени канала Pipe
	char   szPipeName[256];


	printf("Syntax: pipec [servername]\n");

	// Если при запуске PIPEC было указано имя срвера,
	// указываем его в имени канала Pipe
	if (argc > 1)
		sprintf_s(szPipeName, "\\\\%s\\pipe\\$MyPipe$",
			argv[1]);

	// Если имя сервера задано не было, создаем канал
	// с локальным процессом
	else
		strcpy_s(szPipeName, "\\\\.\\pipe\\$MyPipe$");

	// Создаем канал с процессом PIPES
	hNamedPipe = CreateFile(
		cw(szPipeName), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);

	// Если возникла ошибка, выводим ее код и 
	// завершаем работу приложения
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		fprintf(stdout, "CreateFile: Error %ld\n",
			GetLastError());
		_getch();
		return 0;
	}

	// Выводим сообщение о создании канала
	printf_s( "\nConnected. Type 'exit' to terminate\n");

	// Цикл обмена данными с серверным процессом
	while (1)
	{
		// Выводим приглашение для ввода команды
		printf("cmd>");

		// Вводим текстовую строку
		gets_s(szBuf);

		// Передаем введенную строку серверному процессу
		// в качестве команды
		if (!WriteFile(hNamedPipe, szBuf, strlen(szBuf) + 1,
			&cbWritten, NULL))
			break;
		//ResetEvent(c);

		// Получаем эту же команду обратно от сервера
		if (ReadFile(hNamedPipe, szBuf, 512, &cbRead, NULL))
			printf("Received back: <%s>\n", szBuf);

		// Если произошла ошибка, выводим ее код и
		// завершаем работу приложения
		else
		{
			fprintf(stdout, "ReadFile: Error %ld\n",
				GetLastError());
			_getch();
			break;
		}

		// В ответ на команду "exit" завершаем цикл
		// обмена данными с серверным процессом
		if (!strcmp(szBuf, "exit"))
			break;
	}

	// Закрываем идентификатор канала
	CloseHandle(hNamedPipe);
	return 0;
}
