#define WIN32_LEAN_AND_MEAN

#include<windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

int main()
{

	char s;

	setlocale(LC_ALL, "rus");
	cout << "Какую роль ты выбираешь?" << endl;
	cout << "K - клиент" << endl;
	cout << "S - сервер" << endl;
	cout << "Ваш выбор : ";
	cin >> s;
	switch (s)
	{
	case 'K':
	{
		// Инициализаци библиотеки + проверка версии библиотеки WinSock2.h
		WSADATA wsaData;
		ADDRINFO hints;
		ADDRINFO* addrResult = NULL;
		// Создаем сокет
		SOCKET ConnectSocket = INVALID_SOCKET; // Говорим что сокет пока что не правильный, для предотвращения ошибки
		char recvBuffer[300];


		// Проверка выполнения на ошибка
		int result = NULL;
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0)
		{
			cout << "WSAStartUp, result = " << result << endl; system("pause"); return 1;
		}
		// Очистка стурктуры(Вариант для OS Windows)
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;


		// Задаём параметры соеденения
		result = getaddrinfo("localhost", "623", &hints, &addrResult);
		if (result != 0) {
			cout << "GetAddInfo failed with error" << result << endl; WSACleanup(); system("pause"); return 1;
		}
		// Создание сокета
		ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

		// проверяем создание сокета
		if (ConnectSocket == INVALID_SOCKET) {
			cout << "Socket clreate failed"; freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}
		// Проверяем соединение 
		result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			cout << "fail connect to server" << endl; closesocket(ConnectSocket); ConnectSocket = INVALID_SOCKET; freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}
		// Отсправляем на сервер что-то
		const char* sendBuffer = "Hello from client";
		result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
		if (result == SOCKET_ERROR)
		{
			cout << "send failed, error: " << result << endl; closesocket(ConnectSocket); freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}
		// Что мы отправляли
		cout << "Send: " << result << "butes" << endl;

		// Отключаем сокет от передачи данных, но на прием активен
		result = shutdown(ConnectSocket, SD_SEND);
		if (result == SOCKET_ERROR)
		{
			cout << "ShotDown error:" << result << endl;
			closesocket(ConnectSocket); freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}

		do
		{
			ZeroMemory(recvBuffer, 300);
			result = recv(ConnectSocket, recvBuffer, 300, NULL);
			if (result > 0)
			{
				cout << "Received" << result << "bytes" << endl;
				cout << "Received data : " << recvBuffer << endl;
			}
			else if (result == 0)
			{
				cout << "Connection close" << endl;
			}
			else
			{
				cout << "recv failed with error" << endl;
			}
		} while (result > 0);
		closesocket(ConnectSocket);
		freeaddrinfo(addrResult);
		WSACleanup();
		system("pause");
		return 0;


	}
	break;
	case 'S':
	{
		// Инициализаци библиотеки + проверка версии библиотеки WinSock2.h
		WSADATA wsaData;
		ADDRINFO hints;
		ADDRINFO* addrResult = NULL;
		// Создаем сокет
		SOCKET ClientSocket = INVALID_SOCKET; // Говорим что сокет пока что не правильный, для предотвращения ошибки
		SOCKET ListenSocket = INVALID_SOCKET;
		char recvBuffer[300];


		// Проверка выполнения на ошибка
		int result = NULL;
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0)
		{
			cout << "WSAStartUp, result = " << result << endl; system("pause"); return 1;
		}
		// Очистка стурктуры(Вариант для OS Windows)
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;


		// Задаём параметры соеденения
		result = getaddrinfo(NULL, "623", &hints, &addrResult);
		if (result != 0) {
			cout << "GetAddInfo failed with error" << result << endl; WSACleanup(); system("pause"); return 1;
		}
		// Создание сокета
		ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

		// проверяем создание сокета
		if (ListenSocket == INVALID_SOCKET) {
			cout << "Socket clreate failed"; freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}

		// Проверяем соединение 
		result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			cout << "Binding socket failed:(" << endl; closesocket(ListenSocket); ListenSocket = INVALID_SOCKET; freeaddrinfo(addrResult); WSACleanup(); return 1;
		}
		// Отсправляем на сервер что-то
		const char* sendBuffer = "Hello from Server";
		result = listen(ListenSocket, SOMAXCONN);
		if (result == SOCKET_ERROR)
		{
			cout << "Listening socket failed" << endl;
			closesocket(ListenSocket);
			ListenSocket = INVALID_SOCKET;
			freeaddrinfo(addrResult);
			WSACleanup();
			system("pause");
			return 1;
		}

		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET)
		{
			cout << "Accepting socket failed" << endl;
			closesocket(ListenSocket);
			ListenSocket = INVALID_SOCKET;
			freeaddrinfo(addrResult);
			WSACleanup();
			system("pause");
			return 1;
		}

		closesocket(ListenSocket);



		do
		{
			ZeroMemory(recvBuffer, 300);
			result = recv(ClientSocket, recvBuffer, 300, NULL);
			if (result > 0)
			{
				cout << "Received" << result << "bytes" << endl;
				cout << "Received data : " << recvBuffer << endl;

				result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
				if (result == SOCKET_ERROR)
				{
					cout << "Failed tosend data back";
					closesocket(ClientSocket);
					ClientSocket = INVALID_SOCKET;
					freeaddrinfo(addrResult);
					WSACleanup();
					system("pause");
					return 1;
				}
			}
			else if (result == 0)
			{
				cout << "Connection closing..." << endl;
			}
			else
			{
				cout << "recv failed with error" << endl;
				closesocket(ClientSocket);
				freeaddrinfo(addrResult);
				WSACleanup();
				system("pause");
				return 1;
			}
		} while (result > 0);

		result = shutdown(ClientSocket, SD_SEND);
		if (result == SOCKET_ERROR)
		{
			cout << "ShutDown client sicket failed" << endl;
			closesocket(ClientSocket);
			freeaddrinfo(addrResult);
			WSACleanup();
			system("pause");
			return 1;
		}
		closesocket(ClientSocket);
		freeaddrinfo(addrResult);
		WSACleanup();

		system("pause");

		return 0;

	}
	break;
	}
	return 0;
}