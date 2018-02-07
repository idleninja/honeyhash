// honeyhash.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include "md5.h"


using namespace std;

string transformers(string h, string DTS){
	char special_chars[] = { '*', '!', '#', '%', '+', '!', '#', '%', '-', '.', '$', '=', '@', '|', '/', '*', '!', '#', '%', '+', '|', '/', '*', '+', '-', '-', '.', '$', '=', '@', '.', '$', '=', '@', '|', '/' };
	int mon = std::stoi(DTS.substr(5, 6));
	
	for (string::size_type i = 0; i < h.length(); i++)
	{
		if (i % mon == 0)
		{
			h[i] = special_chars[i];
		} else {
			if (i % 2 == 0)
			{
				h[i] = tolower(h[i]);
			}
			else{
				h[i] = toupper(h[i]);
			}			
		}
	}
	return h;
}

int createLogin(string username, string password, string domain){

	std::wstring tUsername = std::wstring(username.begin(), username.end());
	LPCWSTR Username = tUsername.c_str();

	std::wstring tPassword = std::wstring(password.begin(), password.end());
	LPCWSTR Password = tPassword.c_str();

	std::wstring tDomain = std::wstring(domain.begin(), domain.end());
	LPCWSTR Domain = tDomain.c_str();


	STARTUPINFO StartInfo;
	PROCESS_INFORMATION ProcInfo;
	memset(&ProcInfo, 0, sizeof(ProcInfo));
	memset(&StartInfo, 0, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartInfo.wShowWindow = SW_HIDE;
	CreateProcessWithLogonW(Username, Domain, Password,
		LOGON_NETCREDENTIALS_ONLY, NULL, L"C:\\Windows\\System32\\cmd.exe",
		CREATE_NO_WINDOW, NULL, NULL,
		(LPSTARTUPINFOW)&StartInfo, &ProcInfo);

	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	TCHAR infoBuf[150];
	CHAR hostname[150];
	DWORD bufCharCount = 150;
	int i = 0;

	bufCharCount = 150;
	if (GetComputerName(infoBuf, &bufCharCount))
	{
		for (i = 0; i < 150; i++)
		{
			hostname[i] = tolower(infoBuf[i]);
		}
	}
	string hh = "";
	// Generate datetimestamp string
	std::time_t t = std::time(NULL);
	char mbstr[100];	
	std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d", std::localtime(&t));

	hh = std::string(mbstr) + ":" + std::string(hostname);
	
	
	
	std::string poisoned_honey = transformers(md5(md5(hh)), mbstr);
	//std::cout << "\n\nbefore: " << hh << "\n\n";
	//std::cout << "\n\npoisoned hash: " << poisoned_honey << "\n\n";
	createLogin("interesting_service_account", poisoned_honey, "subdomain.domain.com");

}
