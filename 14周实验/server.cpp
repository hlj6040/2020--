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

        

         //第一步：加载socket库函数

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

        

         //第二步创建套接字

         SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

        

         //第三步：绑定套接字

        

         //获取地址结构

         SOCKADDR_IN addrSrv;

         addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

         //将IP地址指定为INADDR_ANY，允许套接字向任何分配给本地机器的IP地址发送或接收数据

         //htonl()将主机的无符号长整形数转换成网络字节顺序。


         addrSrv.sin_family = AF_INET;

         //sin_family 表示地址族，对于IP地址，sin_family成员将一直是AF_INET


         addrSrv.sin_port = htons(8081);

         //htons()将主机的无符号短整形数转换成网络字节顺序

        

         bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

        

         //监听客户端

         listen(sockSrv,5);

        

         //定义从客户端接受的地址信息

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
			double  hs = 4,  hx = 0;  //航速  航向
			toHsx(recvBuf, hs, hx);
			hx = (hx/180)*3.1415926;
			cout << "航速：" << hs << endl;
			x1 += hs*cos(hx);//+(-1+rand()%3)*wucha;
			y1 += hs*sin(hx);//+(-1+rand()%3)*wucha;
			
			in >> x >> y;
			if(in.eof())
				break;
			string  str = to_string(x);
			str  +=  " "  + to_string(y) + " " + to_string(x1) + " " + to_string(y1);
			cout << recvBuf  << endl;
				
               //不断等待客户端的请求的到来，并接受客户端的连接请求
                
            send(sockConn,str.c_str(),str.length()+1,0);
			}
			}
            closesocket(sockConn);
	}
		in.close();
        WSACleanup();
         return 0;
}
