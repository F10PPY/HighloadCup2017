#ifndef HEADERS_H
#define HEADERS_H
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include "json.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <time.h>
#include <array>
#include <thread>

#define MAX_EVENTS 65536
using json = nlohmann::json;
namespace fs = std::experimental::filesystem;
using namespace std;


struct s_users{
	string birth_date;
	int32_t age;
	string email;
	string first_name;
	string last_name;
	string gender;
	unordered_set <uint32_t> vis;
	map <int32_t,uint32_t> vis_sort;
};
struct s_locations{
	int32_t distance;
	string place;
	string country;
	string enc_cntry;
	string city;
	unordered_set <uint32_t> vis;
};
struct s_visits{
	uint32_t location;
	uint32_t user;
	int32_t visited_at;
	uint8_t mark;
};
struct vis_params
{
	bool flag=0;
	string country;
	int32_t value;
};

struct loc_params
{
	bool flag=0;
	int32_t value;
	char gender;
};
const string R200="HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/plain; charset=utf-8\r\n";
const char R400[]="HTTP/1.1 400 Bad Request\r\nConnection: keep-alive\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 3\r\n\r\n400";
const char R404[]="HTTP/1.1 404 Not Found\r\nConnection: keep-alive\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 3\r\n\r\n404";
const char RPOST[]="HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 2\r\n\r\n{}";
const char R_empty_vis[]="HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 13\r\n\r\n{\"visits\":[]}";
const char R_zero_avg[]="HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 11\r\n\r\n{\"avg\":0.0}";

//const string R200="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain; charset=utf-8\r\n";
//const char R400[]="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 3\r\n\r\n400";
//const char R404[]="HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 3\r\n\r\n404";
//const char RPOST[]="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 2\r\n\r\n{}";
//const char R_empty_vis[]="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 13\r\n\r\n{\"visits\":[]}";
//const char R_zero_avg[]="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain; charset=utf-8\r\nContent-length: 11\r\n\r\n{\"avg\":0.0}";


size_t sendall(int fd, const char* buf,size_t len);
int set_nonblock(int fd);
int epoll_mod(int &EPoll, uint32_t mod, int fd, epoll_event &Event, int op);
bool parseData();
bool parseDataNl(unordered_map<uint32_t, s_users> &users);
int reqHandler(char *buff, string &response);
int getParams(char * pch, unordered_map<string, string> &params);
int fetchVisits(char *pos, string &response,uint32_t id);
int fetchVisitsSorted(char *pos, string &response, uint32_t id);
int countAvg(char *pos, string &response, uint32_t id);
int getVisParams(char * pch, vis_params *arr);
int getLocParams(char * pch, loc_params *arr);
string url_encode(const string &value);


int cycle(int &MasterSocket,int t_id);

void master(int MasterSocket);
void worker(int wrk_epoll, int tid);
void passfd(int fd, const char* path, int num_threads);
bool U_Connect(int &s,const char path[]);
ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd);
ssize_t sock_fd_read(int sock, void *buf, ssize_t buflen, int *fd);
#endif // HEADERS_H
