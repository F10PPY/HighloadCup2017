#include "headers.h"
thread_local string body;
thread_local vis_params vis_arr[4];
void worker(int wrk_epoll,int tid)
{
	body.reserve(2048);
	vis_arr[3].country.resize(256);

	epoll_event Events[MAX_EVENTS];

	unsigned int n=0;
	unsigned int i=0;

	int RecvResult=0;
	char buff[1024];
	string response;
	size_t R404_l=strlen(R404);
	size_t R400_l=strlen(R400);
	const char C404[]="HTTP/1.1 404 Not Found\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 3\r\n\r\n404";
	size_t len=strlen(C404);
	chrono::steady_clock::time_point t1,t2;
	while(true)
	{
		n=epoll_wait(wrk_epoll,Events,MAX_EVENTS,0);
		for(i=0;i<n;++i)
		{
			if (Events[i].events & EPOLLHUP)
			{
				//cout<<"ehup"<<strerror(errno)<<endl;
				close(Events[i].data.fd);
				continue;
			}
			if(Events[i].events & EPOLLIN)
			{
				RecvResult=recv(Events[i].data.fd,buff,1024,MSG_NOSIGNAL);

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

					catch (...)
					{
						//cout<<buff<<"ex:"<<e.what()<<endl;
						send(Events[i].data.fd,R404,R404_l,MSG_NOSIGNAL);
					}
				}
				else if(RecvResult==-1)
				{
					cout<<RecvResult<<strerror(errno)<<endl;
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
}
