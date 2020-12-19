#include "headers.h"
#include <sys/stat.h>
void passfd(int fd,const char* path,int num_threads)
{
	int s, s2;
	sockaddr_un local, remote;
	unsigned int len;

	s = socket(AF_UNIX, SOCK_STREAM, 0);

	int enable = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

	local.sun_family=AF_UNIX;
	strcpy(local.sun_path,path);
	unlink(local.sun_path);
	bind(s,(sockaddr*)&local,sizeof(local));
	chmod(local.sun_path, 0777);
	if (listen(s, 1) == -1) {
		cout<<"Socket pass listen error"<<endl;
		//return -1;
	}

	len = sizeof(remote);
	for(int i=0;i<num_threads;++i)
	{
		cout<<"accepting"<<endl;
		if ((s2 = accept(s, (sockaddr *)&remote, &len)) == -1)
		{
			cout<<"Socket pass accept error"<<endl;
			//close(s);
			close(s2);
			//break;
			//	return -1;
		}
		else
		{

			sock_fd_write(s2,(void*)"1",1,fd);
			close(s2);
		}
	}
	//return 1;
}
bool U_Connect(int &s,const char path[])
{
	sockaddr_un remote;
	unsigned int len=sizeof(remote);
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	//set_nonblock(s);
	remote.sun_family=AF_UNIX;
	strcpy(remote.sun_path,path);

	if (-1==connect(s, (sockaddr *)&remote, len))
	{
		close(s);
		s=-1;
		return 0;
	}
	return 1;
}

ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd)
{
	ssize_t     size;
	struct msghdr   msg;
	struct iovec    iov;
	union {
		struct cmsghdr  cmsghdr;
		char        control[CMSG_SPACE(sizeof (int))];
	} cmsgu;
	struct cmsghdr  *cmsg;

	iov.iov_base = buf;
	iov.iov_len = buflen;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	if (fd != -1) {
		msg.msg_control = cmsgu.control;
		msg.msg_controllen = sizeof(cmsgu.control);

		cmsg = CMSG_FIRSTHDR(&msg);
		cmsg->cmsg_len = CMSG_LEN(sizeof (int));
		cmsg->cmsg_level = SOL_SOCKET;
		cmsg->cmsg_type = SCM_RIGHTS;

		printf ("passing fd %d\n", fd);
		//*((int *) CMSG_DATA(cmsg)) = fd;
		memcpy(CMSG_DATA(cmsg),&fd,sizeof(int));

	} else {
		msg.msg_control = NULL;
		msg.msg_controllen = 0;
		printf ("not passing fd\n");
	}

	size = sendmsg(sock, &msg, 0);

	if (size < 0)
		printf("sendmsg error\n");
	return size;
}

ssize_t sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd)
{
	ssize_t     size;

	if (fd) {
		struct msghdr   msg;
		struct iovec    iov;
		union {
			struct cmsghdr  cmsghdr;
			char        control[CMSG_SPACE(sizeof (int))];
		} cmsgu;
		struct cmsghdr  *cmsg;

		iov.iov_base = buf;
		iov.iov_len = bufsize;

		msg.msg_name = NULL;
		msg.msg_namelen = 0;
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;
		msg.msg_control = cmsgu.control;
		msg.msg_controllen = sizeof(cmsgu.control);
		size = recvmsg (sock, &msg, 0);
		if (size < 0) {
			perror ("recvmsg");
			return 0;
		}
		cmsg = CMSG_FIRSTHDR(&msg);
		if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) {
			if (cmsg->cmsg_level != SOL_SOCKET) {
				fprintf (stderr, "invalid cmsg_level %d\n",
						 cmsg->cmsg_level);
				return 0;
			}
			if (cmsg->cmsg_type != SCM_RIGHTS) {
				fprintf (stderr, "invalid cmsg_type %d\n",
						 cmsg->cmsg_type);
				return 0;
			}

			//*fd = *((int *) CMSG_DATA(cmsg));
			memcpy(fd,CMSG_DATA(cmsg),sizeof(int));

			printf ("received fd %d\n", *fd);
		} else
			*fd = -1;
	} else {
		size = read (sock, buf, bufsize);
		if (size < 0) {
			perror("read");
			return 0;
		}
	}
	return size;
}
