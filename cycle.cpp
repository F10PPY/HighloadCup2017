#include "headers.h"
//thread_local string body;
//thread_local vis_params vis_arr[4];


int cycle(int &MasterSocket,int t_id)
{
//	body.reserve(2048);
	//vis_arr[3].country.resize(256);
	int EPoll=epoll_create1(0);

	epoll_event *Events;
	Events = new epoll_event[MAX_EVENTS];

	epoll_event Event;
	//epoll_event Events[MAX_EVENTS];
	epoll_mod(EPoll,EPOLLIN,MasterSocket,Event,EPOLL_CTL_ADD);

	unsigned int n=0;
	unsigned int i=0;

	int RecvResult=0;
	char buff[1024];
	string response;
	size_t R404_l=strlen(R404);
	size_t R400_l=strlen(R400);
	size_t R_zero_avg_l=strlen(R_zero_avg);
	size_t  R_empty_vis_l=strlen( R_empty_vis);
	size_t RPOST_l=strlen(RPOST);
	chrono::steady_clock::time_point t1,t2;
	clock_t t;
	//t1= chrono::steady_clock::now();
	int fd;
	int test=50;
	int cnt=0,cnt2=0;


	while(true)
	{

		fd=accept4(MasterSocket,0,0,O_NONBLOCK);
		if(fd==-1)continue;
		RecvResult=recv(fd,buff,1024,MSG_NOSIGNAL);
		if(RecvResult>0)
		{

			buff[RecvResult]='\0';
			try
			{
				//						t = clock();
				//						t1= chrono::steady_clock::now();
				reqHandler(buff,response);
				//						t2= chrono::steady_clock::now();
				//						cout << "request cpu "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;
				//send(Events[i].data.fd,R404,R404_l,MSG_NOSIGNAL);
				send(fd,response.c_str(),response.length(),MSG_NOSIGNAL);
			}
			catch (const invalid_argument& ia)
			{
				//	cout<<ia.what()<<endl;
				send(fd,R400,R400_l,MSG_NOSIGNAL);
			}

			catch (exception& e)
			{
				//cout<<buff<<"ex:"<<e.what()<<endl;
				send(fd,R404,R404_l,MSG_NOSIGNAL);
			}
		}
		else if(RecvResult==-1)
		{
			cout<<RecvResult<<strerror(errno)<<endl;
			//	close(Events[i].data.fd);
		}
		close(fd);

	}






	while(true)
	{
		n=epoll_wait(EPoll,Events,MAX_EVENTS,-1);
		for(i=0;i<n;++i)
		{
			if(Events[i].data.fd==MasterSocket)
			{
				//++cnt;
				//if(cnt>2000) cout<<"accept:"<<cnt<<endl;
				//Event.events=EPOLLIN|EPOLLET;
                Event.data.fd=accept4(MasterSocket,nullptr,nullptr,O_NONBLOCK);
				//Event.data.fd=accept(MasterSocket,0,0);
				//set_nonblock(Event.data.fd);
				if(-1==Event.data.fd)
				{
					cout<<"accept err: "<<strerror(errno)<<endl;
				}
				else

					epoll_ctl(EPoll,EPOLL_CTL_ADD,Event.data.fd,&Event);
				continue;
			}
			if (Events[i].events & EPOLLHUP)
			{
				//cout<<"ehup"<<strerror(errno)<<endl;
				close(Events[i].data.fd);
				continue;
			}
			if(Events[i].events & EPOLLIN)
			{
				RecvResult=recv(Events[i].data.fd,buff,1024,MSG_NOSIGNAL|MSG_DONTWAIT);
				++cnt;
				if (RecvResult>0)
				{
					buff[RecvResult]='\0';
					try
					{
						//						t = clock();
						//						t1= chrono::steady_clock::now();

						reqHandler(buff,response);

						//						t2= chrono::steady_clock::now();
						//						cout << "request cpu "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;
						//send(Events[i].data.fd,R404,R404_l,MSG_NOSIGNAL);
						send(Events[i].data.fd,response.c_str(),response.length(),MSG_NOSIGNAL);
					}
					catch (const invalid_argument& ia)
					{
						//	cout<<ia.what()<<endl;
						send(Events[i].data.fd,R400,R400_l,MSG_NOSIGNAL);
					}

					catch (exception& e)
					{
						//cout<<buff<<"ex:"<<e.what()<<endl;
						send(Events[i].data.fd,R404,R404_l,MSG_NOSIGNAL);
					}
				}
				else if(RecvResult==-1)
				{
					//cout<<cnt<<" "<<"fd:"<<Events[i].data.fd<<"rcv "<<RecvResult<<strerror(errno)<<endl;
					//	close(Events[i].data.fd);
				}
				else
				{
					//cout<<cnt<<" "<<"fd:"<<Events[i].data.fd<<"rcv "<<RecvResult<<endl;
					close(Events[i].data.fd);
				}
			}

		}
	}
	delete []Events;
	return 1;
}

