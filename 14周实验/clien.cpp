
#include "winsock2.h"  
#include <iostream>  
#include <fstream> 
#include <vector>
#include <cmath>
#include <time.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
using namespace std;
typedef  vector<double>  Point;
ofstream  foutB, foutG;
double rand_k = 90.;
double A_maxV = 4,
	   B_maxV = 2;
const  double PI = 3.1415926;
double dushu(Point A, Point B)
{
    double Long = sqrt((B[0] - A[0]) * (B[0] - A[0]) + (B[1] - A[1]) * (B[1] - A[1]));
    if ((B[1] - A[1]) >= 0) {
        return (acos((B[0] - A[0]) / Long) / PI * 180);
    }
    else {
        return -(acos((B[0] - A[0]) / Long) / PI * 180);
    }

}

void  updata(vector<double> &data){
	double k = dushu({data[0],data[1]}, {data[2],data[3]}) + rand_k;
    if (k > 180) k = k - 360;
	data[2] += A_maxV * cos(k / 180 * PI) * 0.01;
	data[3] += A_maxV * sin(k / 180 * PI) * 0.01;
	data[0] += B_maxV * 0.01 * cos(PI / 6);
	data[1] += B_maxV * 0.01 * sin(PI / 6);
	
}
string  accdata(char  str[]){
	int len = strlen(str);
	vector<double>  data;
	string  str1 = "";
	for(int i = 0; i < len; i++)
	{
		if(str[i] == (char) 32){
			data.push_back(atof(str1.c_str()));
			str1 = "";
		}
		else
			str1 += str[i];
	}
	data.push_back(atof(str1.c_str()));
	foutB << data[2] << " " << data[3] << " "<< endl;
	foutG << data[0] << " " << data[1] << endl;
	double k = dushu({data[2],data[3]}, {data[0],data[1]});
	if(((data[2]-data[0])*(data[2]-data[0])+(data[3]-data[1])*(data[3]-data[1]) < 900))
		k += rand_k;
	str1 = to_string(A_maxV)+" " + to_string(k);
	cout <<str1 << endl;
	return str1;
}
int main()
{
	WSADATA wsd; //WSADATA变量  
	SOCKET sHost; //客户端套接字  
	SOCKADDR_IN servAddr; //服务器地址  
 
	const int BUF_SIZE = 64;
	char buf[BUF_SIZE]; //接收数据缓冲区  
	char bufRecv[BUF_SIZE];
	int retVal; //返回值  
	
	foutB.open("boat_path.txt");
	foutG.open("gold.txt");
	if(!foutB.is_open() ||  !foutG.is_open()){
		printf("no open file/n");
		system("pause");
		return 0;
	}
	//初始化套结字动态库 
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return -1;
	}
	//创建套接字  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源  
		return  -1;
	}
	servAddr.sin_family = AF_INET;
	//如果编译通不过 属性 c++ 常规  sdl 改成否
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//设置服务端地址  这里表示本机
	servAddr.sin_port = htons((short)8081);
	int nServAddlen = sizeof(servAddr);
	
	//连接服务器  
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //关闭套接字 
		WSACleanup(); //释放套接字资源  
		return -1;
	}
	//成功建立连接  可以开始通信了
	//ifstream  in("new.txt");
	double x, y;
	
	send(sHost,"$cset0000**#",13,0);
	
	for(int i  = 0; i < 500; i++){
		ZeroMemory(bufRecv, BUF_SIZE);
		recv(sHost,bufRecv,BUF_SIZE,0);
		cout << bufRecv << endl;
		string data = accdata(bufRecv);
		send(sHost,data.c_str(),data.length(),0);
		
	}
	send(sHost,"$cend0000**#",13,0);
	closesocket(sHost); //关闭套接字  
	
	WSACleanup(); //释放套接字资源  
	
	foutB.close();
	foutG.close();
	system("pause");
	
	return 0;
}
