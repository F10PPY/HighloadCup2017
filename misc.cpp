#include "headers.h"

int set_nonblock(int fd)
{
    int flags;
#if defined(O_NONBLOCK)
    if(-1==(flags=fcntl(fd,F_GETFL,0)))
        flags=0;
    return fcntl(fd,F_SETFL,flags|O_NONBLOCK);
#else
    flags=1;
    return ioctl(fd,FIOBIO,&flags);
#endif
}

int epoll_mod(int &EPoll,uint32_t mod,int fd,epoll_event &Event,int op)
{
    Event.data.fd=fd;
    Event.events=mod;
    return epoll_ctl(EPoll,op,fd,&Event);
}

string url_encode(const string &value)
{
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }
        if(c==' ')
        {
            escaped<<'+';
            continue;
        }
        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char) c);
        escaped << nouppercase;
    }

    return escaped.str();
}
