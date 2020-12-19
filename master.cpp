#include "headers.h"

struct
{
	unsigned int bit : 1;
}t_num;

void master(int MasterSocket)
{

	int num_threads=1;
	int t_arr[num_threads];
	thread t_pool[num_threads];
	for (int i = 0; i < num_threads; ++i)
	{
		t_arr[i]=epoll_create1(0);
		t_pool[i] = thread(worker,t_arr[i],i);
		t_pool[i].detach();
	}

	epoll_event Event;

	int fd;
	uint32_t tn=0;
	while (true)
	{
        fd=accept(MasterSocket,nullptr,nullptr);
		//Event.data.fd=accept(MasterSocket,0,0);
		if(fd!=-1)
		{
			//epoll_ctl(t_arr[0],EPOLL_CTL_ADD,fd,&Event);
			epoll_mod(t_arr[0],EPOLLIN,fd,Event,EPOLL_CTL_ADD);
			//if(tn==2)tn=0;
		}
	}
	for (int i = 0; i < num_threads; ++i)
	{
		t_pool[i].join();
	}
	int cnt=0;

	int EPoll=epoll_create1(0);
	epoll_event Events[MAX_EVENTS];



	epoll_mod(EPoll,EPOLLIN,MasterSocket,Event,EPOLL_CTL_ADD);

	unsigned int n=0;
	unsigned int i=0;

	while(true)
	{
		n=epoll_wait(EPoll,Events,MAX_EVENTS,-1);
		for(i=0;i<n;++i)
		{
			if(Events[i].data.fd==MasterSocket)
			{
				cnt++;
				if(cnt>3000)cout<<cnt<<endl;
				//	Event.data.fd=accept4(MasterSocket,0,0,O_NONBLOCK);
                Event.data.fd=accept(MasterSocket,nullptr,nullptr);
				epoll_ctl(t_arr[0],EPOLL_CTL_ADD,Event.data.fd,&Event);
				continue;
			}
		}

	}
}
