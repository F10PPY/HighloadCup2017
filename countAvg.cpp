#include "headers.h"
thread_local loc_params loc_arr[5];
extern unordered_map  <uint32_t,s_users> users;
extern unordered_map  <uint32_t,s_visits> visits;
extern unordered_map  <uint32_t,s_locations> locations;
static thread_local double avg;
extern thread_local string body;
static thread_local size_t cnt;
//thread_local stringstream stream;
int countAvg(char *pos, string &response, uint32_t id)
{
	avg=0.0;
	cnt=0;

	if (*(pos+3)!='?')
	{
		for(auto it=locations[id].vis.begin();it!=locations[id].vis.end();++it)
		{
			++cnt;
			avg+=visits.at(*it).mark;
		}
	}
	else
	{
		getLocParams(pos+3,loc_arr);
		for(auto it=locations[id].vis.begin();it!=locations[id].vis.end();++it)
		{
			if(loc_arr[0].flag)//fromDate
			{
				if(visits.at(*it).visited_at<loc_arr[0].value)continue;
			}
			if(loc_arr[1].flag)//toDate
			{
				if(visits.at(*it).visited_at>loc_arr[1].value)continue;
			}
			if(loc_arr[2].flag)//fromAge
			{

				if(users.at(visits.at(*it).user).age<loc_arr[2].value)continue;

			}
			if(loc_arr[3].flag)//toAge
			{
				//				if(id==76)
				//				{
				//					cout<<users.at(visits.at(*it).user).age<<endl<<users.at(visits.at(*it).user).birth_date<<endl;
				//				}
				if(users.at(visits.at(*it).user).age>=loc_arr[3].value)continue;
				//				if(id==76)
				//				{
				//					cout<<"good"<<endl;
				//				}
			}
			if(loc_arr[4].flag)//gender
			{
				if(users.at(visits.at(*it).user).gender[0]!=loc_arr[4].gender)continue;
			}
			++cnt;
			avg+=visits.at(*it).mark;
		}
	}
	if(cnt)
	{
		avg/=cnt;
		response=R200;

		stringstream stream;
		stream<< setprecision(6)<<avg;
		body="{\"avg\":";
		body.append(stream.str()).append("}");


		response.append("Content-Length: ");
		response.append(to_string(body.length()));
		response.append("\r\n\r\n");
		response.append(body);

	}
	else response=R_zero_avg;

	return 1;
}
