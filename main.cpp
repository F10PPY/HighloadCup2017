#include "headers.h"
#include <netinet/tcp.h>
unordered_map  <uint32_t,s_users> users;
unordered_map  <uint32_t,s_visits> visits;
unordered_map  <uint32_t,s_locations> locations;
pid_t pid;

int main()
{
	parseData();
//	unordered_map  <uint32_t,s_users> userz;
//	parseDataNl(userz);

//for(int i=0;i<10000;++i)
//{
//	if(userz[i].age!=users[i].age)
//	{
//	cout<<users[i].age<< " "<<userz[i].age<<endl;
//	}
//}
	//	system("ulimit -a");
	//	system("sysctl -a");
	//clock_t t;
	//chrono::steady_clock::time_point t1,t2;

	//	t = clock();
	//	string body="sdfsdfsdf";
	//	string test;
	//	t1= chrono::steady_clock::now();
	//	test+=body+body+body+body+body+body;
	//	t2= chrono::steady_clock::now();
	//	cout << "make response cl "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;
	//	t = clock();
	//	t1= chrono::steady_clock::now();
	//	test.append(body).append(body).append(body).append(body).append(body).append(body);
	//	t2= chrono::steady_clock::now();
	//	cout << "make response cl "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;

//		pid=fork();
//		if(pid!=0)pid=fork();
//		if(pid==0)
//		{
//			sleep(1);
//			int sock;
//			int MasterSocket;
//			char buf[128];
//			if(!U_Connect(sock,"test"))
//			{
//				cout<<"couldn't connect"<<endl;
//			}
//			do
//			{
//				sleep(1);
//			}
//			while(!sock_fd_read(sock,buf,128,&MasterSocket));
//			cycle(MasterSocket,pid);
//		}

	int MasterSocket=socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
	int enable = 1;
	if(-1==setsockopt(MasterSocket, SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT, &enable, sizeof(int)))
	{
		cout<<"err setsockopt"<<endl;
	}
	if(-1==setsockopt(MasterSocket, SOL_SOCKET,SO_BUSY_POLL|TCP_NODELAY, &enable, sizeof(int)))
	{
		cout<<"err setsockopt"<<endl;
	}
	set_nonblock(MasterSocket);
	struct sockaddr_in sa;
	sa.sin_family=AF_INET;

	sa.sin_port=htons(80);
	inet_pton(AF_INET, "0.0.0.0", &sa.sin_addr);
	if(-1==bind(MasterSocket,( sockaddr*)&sa,sizeof(sa)))
	{
		cout<<"bind err"<<strerror(errno)<<endl;
		return 0;
	}

	if(-1==listen(MasterSocket,2048))
	{
		cout<<"tcp listen error"<<endl;
		return 0;
	}


	//	int rcvBufferSize;
	//	socklen_t sockOptSize = sizeof(rcvBufferSize);
	//	getsockopt(MasterSocket, SOL_SOCKET, SO_RCVBUF, &rcvBufferSize, &sockOptSize);
	//	printf("initial socket receive buf %d\n", rcvBufferSize);
	//	int mss;
	//	socklen_t len = sizeof(mss);
	//	getsockopt(MasterSocket, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);
	//	printf("MSS = %d\n", mss);

	//	rcvBufferSize = 9973;      /* a prime number */
	//	setsockopt(MasterSocket, SOL_SOCKET, SO_RCVBUF, &rcvBufferSize, sizeof(rcvBufferSize));
	//	len = sizeof(rcvBufferSize);
	//	getsockopt(MasterSocket, SOL_SOCKET, SO_RCVBUF, &rcvBufferSize, &len);
	//	printf("SO_RCVBUF = %d (after setting it to 9973)\n\n\n\n", rcvBufferSize);




//				int num_threads=3;
//				thread t[num_threads];
//				for (int i = 0; i < num_threads; ++i) {
//					t[i] = thread(cycle,ref(MasterSocket),i);
//					t[i].detach();
//				}


//	//passfd(MasterSocket,"test",3);
//	cycle(MasterSocket,1);
	master(MasterSocket);
	return 1;
}

