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
	cout << "����� ���� �� ���������?" << endl;
	cout << "K - ������" << endl;
	cout << "S - ������" << endl;
	cout << "��� ����� : ";
	cin >> s;
	switch (s)
	{
	case 'K':
	{
		// ������������ ���������� + �������� ������ ���������� WinSock2.h
		WSADATA wsaData;
		ADDRINFO hints;
		ADDRINFO* addrResult = NULL;
		// ������� �����
		SOCKET ConnectSocket = INVALID_SOCKET; // ������� ��� ����� ���� ��� �� ����������, ��� �������������� ������
		char recvBuffer[300];


		// �������� ���������� �� ������
		int result = NULL;
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0)
		{
			cout << "WSAStartUp, result = " << result << endl; system("pause"); return 1;
		}
		// ������� ���������(������� ��� OS Windows)
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;


		// ����� ��������� ����������
		result = getaddrinfo("localhost", "623", &hints, &addrResult);
		if (result != 0) {
			cout << "GetAddInfo failed with error" << result << endl; WSACleanup(); system("pause"); return 1;
		}
		// �������� ������
		ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

		// ��������� �������� ������
		if (ConnectSocket == INVALID_SOCKET) {
			cout << "Socket clreate failed"; freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}
		// ��������� ���������� 
		result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			cout << "fail connect to server" << endl; closesocket(ConnectSocket); ConnectSocket = INVALID_SOCKET; freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}
		// ����������� �� ������ ���-��
		const char* sendBuffer = "Hello from client";
		result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
		if (result == SOCKET_ERROR)
		{
			cout << "send failed, error: " << result << endl; closesocket(ConnectSocket); freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}
		// ��� �� ����������
		cout << "Send: " << result << "butes" << endl;

		// ��������� ����� �� �������� ������, �� �� ����� �������
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
		// ������������ ���������� + �������� ������ ���������� WinSock2.h
		WSADATA wsaData;
		ADDRINFO hints;
		ADDRINFO* addrResult = NULL;
		// ������� �����
		SOCKET ClientSocket = INVALID_SOCKET; // ������� ��� ����� ���� ��� �� ����������, ��� �������������� ������
		SOCKET ListenSocket = INVALID_SOCKET;
		char recvBuffer[300];


		// �������� ���������� �� ������
		int result = NULL;
		result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0)
		{
			cout << "WSAStartUp, result = " << result << endl; system("pause"); return 1;
		}
		// ������� ���������(������� ��� OS Windows)
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;


		// ����� ��������� ����������
		result = getaddrinfo(NULL, "623", &hints, &addrResult);
		if (result != 0) {
			cout << "GetAddInfo failed with error" << result << endl; WSACleanup(); system("pause"); return 1;
		}
		// �������� ������
		ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

		// ��������� �������� ������
		if (ListenSocket == INVALID_SOCKET) {
			cout << "Socket clreate failed"; freeaddrinfo(addrResult); WSACleanup(); system("pause"); return 1;
		}

		// ��������� ���������� 
		result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			cout << "Binding socket failed:(" << endl; closesocket(ListenSocket); ListenSocket = INVALID_SOCKET; freeaddrinfo(addrResult); WSACleanup(); return 1;
		}
		// ����������� �� ������ ���-��
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