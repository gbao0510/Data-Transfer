// Server.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Server.h"
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

// The one and only application object
CWinApp theApp;
CFile myFile;

using namespace std;

// bien toan cuc
CSocket server;

int sizeMessage, numbers, countClientNow = 0, onOrOffSever = 0, x, checklinelist = 0;
bool checkusname = true;
bool ktra = false;
string Sthongbao;
char thongbao[20];
char *lenhc, *filePathUp, *fileNameDown, *fileName, *luaChonTiepTucClient = "1", *usrname, *pasword;
char listF[999];

void AddFile(char *filename)
{
	int c;
	FILE *fp;
	fp = fopen("FilePath.txt", "a");
	if (fp)
	{
		for (int i = 0; i < strlen(filename); i++)
			putc(filename[i], fp);
		for (int i = strlen(filename); i > 0; i--)
		{
			putc('\n', fp);
			break;
		}
	}
	fclose(fp);
}

bool kiemmangchacover(char *s1, int size1, int size2, char *s2)
{
	int lens1 = 0, lens2 = 0;
	//cout << "\nsize: " << size1;
	for (int i = 0; i < size1; i++)
	{
		//cout << "\nky tu: " << s1[i];
		lens1++;
	}
	//cout << "\nlen1:  " << lens1;
	//cout << "\nlen2_1:  " << size2;
	for (int i = 0; i < size2; i++)
	{
		//cout << "\nky tu: " << int(s2[i]);
		if (int(s2[i]) == 10)
		{
			//cout << "\nket thuc chuoi2: " << i;
			break;
		}
		lens2++;
	}
	//cout << "\nlen2:  " << lens2;
	if (lens1 == lens2)
	{
		//cout << "\ngiong len chuoi1_: ";
		for (int i = 0; i < lens2 ; i++)
		{
			if (s2[i] != s1[i])
			{
				s1 = 0;
				s2 = 0;
				return false;
			}
		}
		//cout << "\ngiong chuoi hoang toan!!!!!!!11: ";
		s1 = 0;
		s2 = 0;
		return true;
	}
	else
	{
		s1 = 0;
		s2 = 0;
		return false;
	}
}

void Checkusername(char usname[50], char pasword[50])
{
	checkusname = true;
	FILE *us;
	us = fopen("username.txt", "a");
	fclose(us);
	FILE *file;
	file = fopen("username.txt", "a+");
	char line[50];
	int xacDinhFileRong=0;

	while (fgets(line, sizeof(line), file))
	{
		xacDinhFileRong = 1;
		if (kiemmangchacover(usname, strlen(usname), strlen(line), line))
		{
			//Tim thay chuoi trung
			checkusname = false;
			fclose(file);
			break;
		}
	}

	//Ghi acc vao file
	if (checkusname == true)
	{
		if(xacDinhFileRong ==1)
		fputs("\n", file);
		fputs(usname, file);
		fclose(file);
		//Ghi pasword
		FILE *pw;
		pw = fopen("password.txt", "a");
		fclose(pw);
		FILE *mypwfile;
		mypwfile = fopen("password.txt", "a+");

		if (mypwfile == NULL)
		{
			printf("Error!");
			exit(1);
		}
		
		if (xacDinhFileRong == 1)
		fputs("\n", mypwfile);
		fputs(pasword, mypwfile);
		fclose(mypwfile);
		mypwfile = 0;
	}
	fclose(file);
	file=0;
}

void Login(char name[50], char word[50])
{
	ktra = false;
	FILE *usname;
	usname = fopen("username.txt", "a");
	fclose(usname);
	int indexline_us =1 , indexline_pw = 1;
	FILE *usfile;
	usfile = fopen("username.txt", "r");

	if (usfile == NULL)
	{
		printf("Error!");
		exit(1);
	}

	char lineus[50];
	while (fgets(lineus, sizeof(lineus), usfile))
	{
		if (kiemmangchacover(name, strlen(name), strlen(lineus), lineus))
		{
			//cout << "\n = user ";
			FILE *mypwfile;
			mypwfile = fopen("password.txt", "r");

			if (mypwfile == NULL)
			{
				printf("Error!");
				exit(1);
			}

			char linepw[50];
			while (fgets(linepw, sizeof(linepw), mypwfile))
			{
				if (kiemmangchacover(word, strlen(word), strlen(linepw), linepw) && indexline_pw == indexline_us )
				{
					//cout << "\n = paword";
					ktra = true;
					break;
				}
				else
				{
					ktra = false;
				}
				indexline_pw++;
			}

			fclose(mypwfile);
			mypwfile=0;
			break;
		}
		else
		{
			ktra = false;
		}
		indexline_us++;
	}
	fclose(usfile);
	usfile=0;
	
}


//DWORD WINAPI khoiTaoTheardCountClientNow(LPVOID arg)
//{
//	SOCKET* conTroConnected = (SOCKET*)arg;
//	CSocket socketServer;
//	//Chuyen ve lai CSocket
//	socketServer.Attach(*conTroConnected);
//
//	//do 
//	//{
//	//	if (countClientNow == -2)
//	//	{
//	//		cout << "da dong server .";
//	//		socketServer.Close();
//	//		break;
//	//	}
//	//} while (1);
//	return 0;
//}

DWORD WINAPI khoiTaoTheardClient(LPVOID arg)
{
	int numbers;
	SOCKET* conTroConnected = (SOCKET*)arg;
	CSocket sockClients;
	//Chuyen ve lai CSocket
	sockClients.Attach(*conTroConnected);

	string  nameClientSelf;

	while (1)
	{
		
		sockClients.Receive((char*)&numbers, sizeof(int), 0);

		if (numbers == 1) //Dang Ky
		{
			sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
			usrname = new char[sizeMessage + 1];
			sockClients.Receive((char*)usrname, sizeMessage, 0); //Nhan username
			usrname[sizeMessage] = '\0';

			sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
			pasword = new char[sizeMessage + 1];
			sockClients.Receive((char*)pasword, sizeMessage, 0); //Nhan password
			pasword[sizeMessage] = '\0';

			//Kiem tra username trung
			Checkusername(usrname, pasword );
			delete usrname;
			usrname = 0;
			delete pasword;
			pasword = 0;
			if (checkusname == true)
			{
				Sthongbao = "khong trung";
				strcpy(thongbao, Sthongbao.c_str());

				sizeMessage = strlen(thongbao);

				sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0); //Goi do dai
				sockClients.Send(thongbao, sizeMessage, 0);  //Goi thong bao khong trung username cho Client
			}
			if (checkusname == false)
			{
				do
				{	
					Sthongbao = "trung";
					strcpy(thongbao, Sthongbao.c_str());
					sizeMessage = strlen(thongbao);

					sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0); //Goi do dai
					sockClients.Send(thongbao, sizeMessage, 0);              //Goi thong bao trung username cho Client

					//Nhan lua chon cua client
					sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
					luaChonTiepTucClient = new char[sizeMessage + 1];
					sockClients.Receive((char*)luaChonTiepTucClient, sizeMessage, 0); //Nhan luachon
					luaChonTiepTucClient[sizeMessage] = '\0';
					if (strcmp(luaChonTiepTucClient,"0") == 0)
					{
						break;
					}

					//Nhan username
					sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
					usrname = new char[sizeMessage + 1];
					sockClients.Receive((char*)usrname, sizeMessage, 0); //Nhan username
					usrname[sizeMessage] = '\0';

					//Nhan password
					sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
					pasword = new char[sizeMessage + 1];
					sockClients.Receive((char*)pasword, sizeMessage, 0); //Nhan password
					pasword[sizeMessage] = '\0';

					//Kiem tra username trung
					Checkusername(usrname, pasword);
					delete usrname;
					usrname = 0;
					delete pasword;
					pasword = 0;
					if (checkusname == true)
					{
						Sthongbao = "khong trung";
						strcpy(thongbao, Sthongbao.c_str());
						sizeMessage = strlen(thongbao);

						sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0); //Goi do dai
						sockClients.Send(thongbao, sizeMessage, 0);  //Goi thong bao khong trung username cho Client

						break;
					}

				} while (checkusname == false);
			}
		}

		if (numbers == 2) //Dang Nhap
		{
			sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
			usrname = new char[sizeMessage + 1];
			sockClients.Receive((char*)usrname, sizeMessage, 0); //Nhan username
			usrname[sizeMessage] = '\0';
			
			sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
			pasword = new char[sizeMessage + 1];
			sockClients.Receive((char*)pasword, sizeMessage, 0); //Nhan password
			pasword[sizeMessage] = '\0';

			//Kiem tra dang nhap 
			Login(usrname, pasword);
			
			if (ktra == true)
			{
				nameClientSelf = usrname;
			}
			if (ktra == false)
			{
				delete usrname;
				delete pasword;
				usrname = 0;
				pasword = 0;
			}
			
			if (ktra == true)
			{

				cout << nameClientSelf <<" da dang nhap" << endl;
				Sthongbao = "thanh cong";
				strcpy(thongbao, Sthongbao.c_str());
				sizeMessage = strlen(thongbao);

				sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0); //Goi do dai
				sockClients.Send(thongbao, sizeMessage, 0);     //Goi thong bao dang nhap thanh cong
			}
			if (ktra == false)
			{
				do
				{
					Sthongbao = "chua thanh cong";
					strcpy(thongbao, Sthongbao.c_str());
					sizeMessage = strlen(thongbao);

					sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0); //Goi do dai
					sockClients.Send(thongbao, sizeMessage, 0);     //Goi thong bao dang nhap chua thanh cong

					//Nhan lua chon cua client
					sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
					luaChonTiepTucClient = new char[sizeMessage + 1];
					sockClients.Receive((char*)luaChonTiepTucClient, sizeMessage, 0); //Nhan luachon
					luaChonTiepTucClient[sizeMessage] = '\0';
					if (strcmp(luaChonTiepTucClient,"0") == 0)
					{
						break;
					}

					sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
					usrname = new char[sizeMessage + 1];
					sockClients.Receive((char*)usrname, sizeMessage, 0); //Nhan username
					usrname[sizeMessage] = '\0';

					sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
					pasword = new char[sizeMessage + 1];
					sockClients.Receive((char*)pasword, sizeMessage, 0); //Nhan password
					pasword[sizeMessage] = '\0';

					//Kiem tra dang nhap
					Login(usrname, pasword);
					/*delete usrname;
					delete pasword;
					usrname = 0;
					pasword = 0;*/
					if (ktra == true)
					{
						nameClientSelf = usrname;

						cout << nameClientSelf << " da dang nhap" << endl;
						Sthongbao = "thanh cong";
						strcpy(thongbao, Sthongbao.c_str());
						sizeMessage = strlen(thongbao);

						sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0); //Goi do dai
						sockClients.Send(thongbao, sizeMessage, 0);      //Goi thong bao dang nhap thanh cong
						break;
					}

					delete pasword;
					usrname = 0;
					pasword = 0;
				} while (ktra == false);
			}
			//--------------------------------------------------------------------------------------------
			//------------------------------------FILE TRANSFER-------------------------------------------
			//--------------------------------------------------------------------------------------------
			if (strcmp(luaChonTiepTucClient,"0") != 0)
			{
				do
				{
					x = 0;
					//Nhan lenh tu Client
					sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
					lenhc = new char[sizeMessage + 1];
					sockClients.Receive((char*)lenhc, sizeMessage, 0); //Nhan lenh
					lenhc[sizeMessage] = '\0';

					if (strcmp(lenhc, "QUIT") == 0 || strcmp(lenhc, "quit") == 0)
					{
						cout << nameClientSelf << " da thoat nhap lenh" << endl;
						break;
					}
					else if (strcmp(lenhc, "DIR") == 0 || strcmp(lenhc, "dir") == 0)
					{
						FILE *fpt;
						fpt = fopen("FilePath.txt", "a");
						fclose(fpt);

						FILE *fp;
						fp = fopen("FilePath.txt", "r");
						char linedir[999],tempdir[999];
						strcpy(tempdir, "");
						while (fgets(linedir, sizeof(linedir), fp) != NULL)
						{
							strcat(tempdir, linedir);
						}
						fclose(fp);

						//Goi du lieu danh sach tap tin cho Client
						if (strcmp(tempdir, "") != 0)
						{
							sizeMessage = strlen(tempdir);
							sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0);
							sockClients.Send(tempdir, sizeMessage, 0);
						}

						cout << nameClientSelf << " da liet ke danh sach tap tin" << endl;
					}
					else if (strcmp(lenhc, "GET") == 0 || strcmp(lenhc, "get") == 0)
					{
						//Nhan file download tu Client
						sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
						fileNameDown = new char[sizeMessage + 1];
						sockClients.Receive((char*)fileNameDown, sizeMessage, 0); //Nhan lenh
						fileNameDown[sizeMessage] = '\0';

						//Noi chuoi
						char str3[100];
						strcpy(str3, fileNameDown);
						strcat(str3, "\n");

						//Kiem tra file name
						FILE *fi;
						fi = fopen("FilePath.txt", "r");
						char lines[999];
						int i;
						fflush(stdin);
						while (fgets(lines, sizeof(lines), fi) != NULL)
						{
							if (strcmp(lines, str3) == 0) //file name co ton tai
							{
								i = 1;
								//Goi thong bao xac nhan file name ton tai
								Sthongbao = "true";
								strcpy(thongbao, Sthongbao.c_str());
								sizeMessage = strlen(thongbao);

								sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0);
								sockClients.Send(thongbao, sizeMessage, 0);
								break;
							}
							i = 0;
						}
						fclose(fi);

						if (i == 0)
						{
							//Goi thong bao xac nhan file name khong ton tai
							Sthongbao = "false";
							strcpy(thongbao, Sthongbao.c_str());
							sizeMessage = strlen(thongbao);

							sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0);
							sockClients.Send(thongbao, sizeMessage, 0);
						}

						if (i == 1)
						{
							char tempget[999], lineget[999];
							strcpy(tempget, "");
							FILE * fileget;
							fileget = fopen(fileNameDown, "r");
							while (fgets(lineget, sizeof(lineget), fileget) != NULL)
							{
								strcat(tempget, lineget);
							}
							fclose(fileget);

							//Goi du lieu cho Client
							sizeMessage = strlen(tempget);
							sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0);
							sockClients.Send(tempget, sizeMessage, 0);

							cout << endl << nameClientSelf << " da download " << fileNameDown << endl;
						}
					}
					else if (strcmp(lenhc, "PUT") == 0 || strcmp(lenhc, "put") == 0)
					{
						//Nhan duong dan tu Client
						sockClients.Receive((char*)&sizeMessage, sizeof(int), 0); //Nhan do dai
						filePathUp = new char[sizeMessage + 1];
						sockClients.Receive((char*)filePathUp, sizeMessage, 0);
						filePathUp[sizeMessage] = '\0';

						//Lay du lieu
						FILE *filepath;
						filepath = fopen(filePathUp, "r");
						char line[999];
						char temp[999];
						strcpy(temp, "");
						while (fgets(line, sizeof(line), filepath) != NULL)
						{
							strcat(temp, line);
						}
						fclose(filepath);

						//Tach ra ten file
						char *posp = strrchr(filePathUp, '\\');
						*posp = '\0';
						fileName = strdup(posp + 1);
						*posp = '\\';

						//Luu du lieu
						FILE *filedown;
						filedown = fopen(fileName, "a");
						char linedown[999];
						if (filedown)
						{
							for (int i = 0; i < strlen(temp); i++)
								putc(temp[i], filedown);
						}
						fclose(filedown);

						//Them file vao danh sach
						AddFile(fileName);


						cout << nameClientSelf << " da upload " << fileName << endl;
					}
					else
					{
						Sthongbao = "Khong hop le";
						strcpy(thongbao, Sthongbao.c_str());
						sizeMessage = strlen(thongbao);

						sockClients.Send(&sizeMessage, sizeof(sizeMessage), 0); //Goi do dai
						sockClients.Send(thongbao, sizeMessage, 0);      //Goi thong bao lenh khong hop le		

						x = 0;
					}
				} while (1);
			}			
		}
		if (numbers == 3) {
			countClientNow = countClientNow - 1;
			cout << endl <<"1 Client da ngat ket noi" <<endl;
			sockClients.Close();
			break;
		}
	}


	if (countClientNow <= 0)
	{
		cout << "Hien tai khong co Client ket noi" << endl;
	}
	else
		cout << endl << "Hien con " << countClientNow << " ket noi den server" << endl;
	
	return 0;
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
		
		unsigned int port = 1234;

		AfxSocketInit(NULL);

		server.Create(port);
		server.Listen(999999);// so hang doi duoc ket noi

		printf("\n Dang lang nghe ket noi tu Client...\n");

		//Tao mang chua cac socket client
		CSocket sockClients, s;
		int index = 0;
		DWORD threadID;
		HANDLE threadStatus;

		do {

			if (server.Accept(sockClients))
			{
				countClientNow = countClientNow + 1;
				cout << "Hien co " << countClientNow << " ket noi den server" << endl;
				//Khoi tao con tro Socket
				SOCKET* controConnected = new SOCKET(); //Chuyen doi CSocket thanh Socket
				
				*controConnected = sockClients.Detach();
				//Khoi tao thread tuong ung voi moi client Connect vao server.
				//Nhu vay moi client se doc lap nhau, khong phai doi tung client xu ly rieng
				threadStatus = CreateThread(NULL, 0, khoiTaoTheardClient, controConnected, 0, &threadID);
			}
		} while (1);

		server.Close();
	}
	return nRetCode;
}
