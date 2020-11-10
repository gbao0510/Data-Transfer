// Client.cpp : Defines the entry point for the console application.
//
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
#include "string.h"
#include <conio.h>
#include <fstream> 
#include <iostream>
#include <string>
#include <stdlib.h>
#include <thread>
#include <assert.h>
#include <stdio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CWinApp theApp;
CFile myFile;

using namespace std;

void split_path_file(char *fn, char *pf) {
	char *part1 = (char *)malloc(strlen(pf));
	strcpy(part1, pf);
	char *pos = strrchr(part1, '/');
	*pos = '\0';
	fn = strdup(pos + 1);
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		CSocket client;
		char sAdd[1000];
		unsigned int port = 1234; //Cung port voi server
		AfxSocketInit(NULL);


		//Tao socket
		client.Create();
		//Nhap dia chi IP cua server
		printf("\nNhap dia chi IP cua server: ");
		gets_s(sAdd);

		int sizelen, x;
		int chon;
		char username[50], password[50],chon2[5] = "1";
		char *thongbao, *kt, *listFile;

		char lenh[50], fileName[50], filePath[100];;
		
		if (client.Connect(CA2W(sAdd), port))
		{
			while (true)
			{
				cout << "\n\n1.Dang ky\n2.Dang nhap\n3.Logout\nChon chuong trinh: " << endl;
				cin >> chon;

				client.Send((char*)&chon, sizeof(int), 0);

				if (chon == 1)
				{
					cout << "\n----------Dang Ky----------\n";
					cout << "Nhap username:";
					cin >> username;
					cout << "password:";
					cin >> password;
	
					sizelen = strlen(username);
					client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
					client.Send(username, sizelen, 0);			//Goi username cho server
	
					sizelen = strlen(password);
					client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
					client.Send(password, sizelen, 0);			//Goi password cho server

					int sizelen;

					client.Receive((char*)&sizelen, sizeof(int), 0); //Nhan do dai
					thongbao = new char[sizelen];
					client.Receive((char*)thongbao, sizelen, 0); //Nhan thong bao

					do
					{
						if (thongbao[0] == 't')
						{

							cout << "\nusername bi trung " << endl;
							cout << "\nban co muon dang ky lai ?(1 = co | 0 = khong) : ";
							cin >> chon2;

							sizelen = strlen(chon2);
							client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
							client.Send(&chon2, sizelen, 0);              //Goi lai luachon
							if (strcmp(chon2, "0") == 0)
								break;
							strcpy(chon2, "2");


							//Tao lai username, password
							cout << "username:";
							cin >> username;
							cout << "password:";
							cin >> password;

							sizelen = strlen(username);
							client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
							client.Send(username, sizelen, 0);              //Goi lai username

							sizelen = strlen(password);
							client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
							client.Send(password, sizelen, 0);              //Goi lai password
						}
						else
						{
							cout << "Dang ky username thanh cong";
							break;
						}

						delete thongbao;
						thongbao = 0;

						client.Receive((char*)&sizelen, sizeof(int), 0); //Nhan do dai
						thongbao = new char[sizelen];
						client.Receive((char*)thongbao, sizelen, 0); //Nhan thong bao

						if (thongbao[0] != 't')
						{
							cout << "Dang ky username thanh cong";
							{
								delete thongbao;
								thongbao = 0;
								break;
							}
						}
					} while (thongbao[0] == 't');
				}



				if (chon == 2)
				{
					int countlogin = 1;
					cout << "\n-----------Dang Nhap-----------\n";
					cout << "username:";
					cin >> username;
					cout << "password:";
					cin >> password;
					getchar();

					sizelen = strlen(username);
					client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
					client.Send(username, sizelen, 0);			//Goi username
					

					sizelen = strlen(password);
					client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
					client.Send(password, sizelen, 0);			//Goi password

					client.Receive((char*)&sizelen, sizeof(int), 0); //Nhan do dai
					thongbao = new char[sizelen];
					client.Receive((char*)thongbao, sizelen, 0); //Nhan thong bao

					do
					{

						if (thongbao[0] == 'c')
						{
							if (countlogin == 3)
							{
								delete thongbao;
								thongbao = 0;
								cout << "dang nhap sai qua 3 lan. Vui long dang nhap lai sau.";
								break;
							}
							countlogin++;// dem so lan con lai login

							cout << "\nusername hoac password sai.  " << endl;
							cout << "\nban co muon dang nhap lai ?(1 = co | 0 = khong) : ";
							//gui lua chon
							cin >> chon2;
							sizelen = strlen(chon2);
							client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
							client.Send(&chon2, sizelen, 0);           //Goi lai luachon
							if (strcmp(chon2, "0") == 0)
								break;
							strcpy(chon2,"2");

							//Tao lai username, password
							cout << "username:";
							cin >> username;
							cout << "password:";
							cin >> password;

							sizelen = strlen(username);
							client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
							client.Send(username, sizelen, 0);              //Goi lai username

							sizelen = strlen(password);
							client.Send(&sizelen, sizeof(sizelen), 0); //Goi do dai
							client.Send(password, sizelen, 0);              //Goi lai password
						}
						else
						{
							cout << "Dang nhap thanh cong";
							delete thongbao;
							thongbao = 0;
							break;
						}

						delete thongbao;
						thongbao = 0;
						client.Receive((char*)&sizelen, sizeof(int), 0); //Nhan do dai
						thongbao = new char[sizelen];
						client.Receive((char*)thongbao, sizelen, 0); //Nhan thong bao

						if (thongbao[0] == 't')
						{
							cout << "Dang nhap thanh cong !" << endl;
							delete thongbao;
							thongbao = 0;
							break;
						}
					} while (thongbao[0] == 'c');
					//--------------------------------------------------------------------------------------------
					//------------------------------------FILE TRANSFER-------------------------------------------
					//--------------------------------------------------------------------------------------------
					if (strcmp(chon2,"0") != 0)
					{
						do
						{
							getchar();
							x = 0;
							cout << endl << "Nhap lenh: ";
							gets_s(lenh);
							/*getchar();*/

							//Goi lenh cho Server
							sizelen = strlen(lenh);
							client.Send(&sizelen, sizeof(sizelen), 0);
							client.Send(lenh, sizelen, 0);

							if (strcmp(lenh, "QUIT") == 0 || strcmp(lenh, "quit") == 0) //Thoat
							{
								printf("Da thoat nhap lenh !\n");
								break;
							}
							else if (strcmp(lenh, "DIR") == 0 || strcmp(lenh, "dir") == 0) //Liet ke danh sach
							{
								cout << "Danh sach tap tin tren server:\n";

								//Nhan danh sach tap tin
								client.Receive((char*)&sizelen, sizeof(int), 0);
								listFile = new char[sizelen + 1];
								client.Receive((char*)listFile, sizelen, 0);
								listFile[sizelen] = '\0';

								cout << listFile << endl;
							}
							else if (strcmp(lenh, "GET") == 0 || strcmp(lenh, "get") == 0) //Download
							{
								/*getchar();*/
								printf("Nhap ten file can download: ");
								gets_s(fileName);

								//Goi ten file can download qua cho server
								sizelen = strlen(fileName);
								client.Send(&sizelen, sizeof(sizelen), 0);
								client.Send(fileName, sizelen, 0);

								//Nhan xac thuc file name tu server
								delete thongbao;
								thongbao = 0;
								client.Receive((char*)&sizelen, sizeof(int), 0);
								thongbao = new char[sizelen];
								client.Receive((char*)thongbao, sizelen, 0);

								if (thongbao[0] == 't')
								{
									//Nhan du lieu tu server
									delete thongbao;
									thongbao = 0;
									client.Receive((char*)&sizelen, sizeof(int), 0);
									thongbao = new char[sizelen];
									client.Receive((char*)thongbao, sizelen, 0);
									thongbao[sizelen] = '\0';

									//Luu du lieu
									FILE *fi;
									fi = fopen(fileName, "a");
									if (fi)
									{
										for (int i = 0; i < strlen(thongbao); i++)
											putc(thongbao[i], fi);
									}
									fclose(fi);
									cout << endl << "Download " << fileName << " thanh cong !" << endl;
								}
								if (thongbao[0] == 'f')
								{
									cout << endl << "File " << fileName << " chua upload len server" << endl;
								}

								////if (strcmp(kt, "true") == 0)
								////{
								//	//Nhan file tu server
								//int dataLength;
								//client.Receive(&dataLength, 4); //Nhan kich thuoc
								//byte* data = new byte[dataLength];
								//client.Receive(data, dataLength); //Nhan du lieu

								//CFile destFile(_T("%s", fileName), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
								//destFile.Write(data, dataLength); // Xuat ra file
								//destFile.Close();
								//}
								//else
								//{
								//	printf("File ban nhap chua upload len Server");
								//}
							}
							else if (strcmp(lenh, "PUT") == 0 || strcmp(lenh, "put") == 0) //Upload
							{
								printf("\nNhap duong dan thu muc can upload: ");
								gets_s(filePath);

								//Goi duong dan cho Server
								sizelen = strlen(filePath);
								client.Send(&sizelen, sizeof(sizelen), 0);
								client.Send(filePath, sizelen, 0);

								printf("\nDa upload len server\n");

								/*x = 1;*/
							}
							else
							{
								delete thongbao;
								thongbao = 0;
								client.Receive((char*)&sizelen, sizeof(int), 0); //Nhan do dai
								thongbao = new char[sizelen];
								client.Receive((char*)thongbao, sizelen, 0); //Nhan thong bao
								if (thongbao[0] == 'K')
								{
									cout << "\nLenh nhap khong hop le\n";
								}
								delete thongbao;
								thongbao = 0;

								x = 0;
							}

							//if (x == 1)
							//{
							//	//Goi file data
							//	myFile.Open(_T("%s", filePath), CFile::modeRead | CFile::typeBinary);
							//	int myFileLength = myFile.GetLength();
							//	client.Send(&myFileLength, 4); // Goi do dai
							//	byte* data = new byte[myFileLength];
							//	myFile.Read(data, myFileLength);
							//	client.Send(data, myFileLength); //Goi data
							//	myFile.Close();
							//	delete data;
							//}
						} while (1);
					}
				}

				if (chon == 3)
				{
					client.Close();
					break;
				}
			}
		}
		if (client.Connect(CA2W(sAdd), port) == NULL)
			printf("Da ngat ket noi voi server....\n");
		//cout << "\nkiem tra noi phat sinh loi";
	}
	getchar();
	return nRetCode;
}