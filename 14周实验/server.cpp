#include<Winsock2.h>
#include<fstream>
#include<stdio.h>
#include<iostream>
#include<time.h>
#include<cmath>
#include<cstring>
#include<stdlib.h>
using namespace std;
void  toHsx(char str[], double  &hs, double &hx){
	if(!strcmp(str,"$cset0000**#"))
		return;
	int len = strlen(str);
	string str1="";
	for(int i = 0; i < len; i++){
		if(str[i] == (char)32){
			hs = atof(str1.c_str());
			str1 = "";
		}
		else{
			str1 += str[i];
		}
	}
	hx  = atof(str1.c_str());
}
int main()
{

        

         //��һ��������socket�⺯��

         //**********************************************************

         WORD wVersionRequested;

         WSADATA wsaData;

         int err;
         wVersionRequested = MAKEWORD( 1, 1 );
     
        err = WSAStartup( wVersionRequested, &wsaData );

        

         if ( err != 0 ) {                           

                   return 0;

         }

        

         if ( LOBYTE( wsaData.wVersion ) != 1 ||

        HIBYTE( wsaData.wVersion ) != 1 ) {

                   WSACleanup( );

                   return 0;

         }

         //**********************************************************

        

         //�ڶ��������׽���

         SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

        

         //�����������׽���

        

         //��ȡ��ַ�ṹ

         SOCKADDR_IN addrSrv;

         addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

         //��IP��ַָ��ΪINADDR_ANY�������׽������κη�������ػ�����IP��ַ���ͻ��������

         //htonl()���������޷��ų�������ת���������ֽ�˳��


         addrSrv.sin_family = AF_INET;

         //sin_family ��ʾ��ַ�壬����IP��ַ��sin_family��Ա��һֱ��AF_INET


         addrSrv.sin_port = htons(8081);

         //htons()���������޷��Ŷ�������ת���������ֽ�˳��

        

         bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

        

         //�����ͻ���

         listen(sockSrv,5);

        

         //����ӿͻ��˽��ܵĵ�ַ��Ϣ

         SOCKADDR_IN addrClient ;

         int len = sizeof(SOCKADDR);

        
	ifstream  in("new.txt");
	if(!in.good())
	{
		printf("no open  file\n");
		system("pause");
		return 0;
	}
	SOCKET sockConn;
	double x, y;
	double x1 = 20., y1 = 30.;
	double wucha = 0.02;
	srand(time(0));
	rand();
	char recvBuf[100];
	
	while(1){
			bool  t = 0;
			sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);
			while(1){
			recv(sockConn,recvBuf,100,0);
			cout << recvBuf << endl;
			cout << strcmp(recvBuf,"$cset0000**#") << endl;
			if(!strcmp(recvBuf,"$cset0000**#")){
				t = 1;
			}
			else if(!strcmp(recvBuf,"$cend0000**#")){
				break;
			}
			if(t){
			double  hs = 4,  hx = 0;  //����  ����
			toHsx(recvBuf, hs, hx);
			hx = (hx/180)*3.1415926;
			cout << "���٣�" << hs << endl;
			x1 += hs*cos(hx);//+(-1+rand()%3)*wucha;
			y1 += hs*sin(hx);//+(-1+rand()%3)*wucha;
			
			in >> x >> y;
			if(in.eof())
				break;
			string  str = to_string(x);
			str  +=  " "  + to_string(y) + " " + to_string(x1) + " " + to_string(y1);
			cout << recvBuf  << endl;
				
               //���ϵȴ��ͻ��˵�����ĵ����������ܿͻ��˵���������
                
            send(sockConn,str.c_str(),str.length()+1,0);
			}
			}
            closesocket(sockConn);
	}
		in.close();
        WSACleanup();
         return 0;
}
