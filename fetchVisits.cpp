#include "headers.h"

extern  thread_local vis_params vis_arr[4];
extern unordered_map  <uint32_t,s_users> users;
extern unordered_map  <uint32_t,s_visits> visits;
extern unordered_map  <uint32_t,s_locations> locations;
extern  thread_local string body;
static thread_local chrono::steady_clock::time_point t1,t2;
static  thread_local clock_t t;
int fetchVisits(char *pos, string &response, uint32_t id)
{

	//	t = clock();
	//	t1= chrono::steady_clock::now();




	map <uint32_t,s_visits*> sort;


	if (*(pos+6)!='?')
	{
		for(auto it=users[id].vis.begin();it!=users[id].vis.end();++it)
		{
			sort[visits.at(*it).visited_at]=&visits.at(*it);
		}
	}
	else
	{
		getVisParams(pos+6,vis_arr);
		for(auto it=users[id].vis.begin();it!=users[id].vis.end();++it)
		{
			if(vis_arr[0].flag)//fromDate
			{
				if(visits.at(*it).visited_at<vis_arr[0].value)continue;
			}
			if(vis_arr[1].flag)//toDate
			{
				if(visits.at(*it).visited_at>vis_arr[1].value)continue;
			}
			if(vis_arr[2].flag)//toDistance
			{
				if(locations.at(visits.at(*it).location).distance>=vis_arr[2].value)continue;
			}
			if(vis_arr[3].flag)//country
			{
				if(string::npos==vis_arr[3].country.find(locations.at(visits.at(*it).location).enc_cntry))continue;
			}
			sort[visits.at(*it).visited_at]=&visits.at(*it);
		}
	}
	//	t2= chrono::steady_clock::now();
	//	cout << "sort cl "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;



	//	t = clock();
	//	t1= chrono::steady_clock::now();
	if(!sort.empty())
	{
		response=R200;
		body=R"({"visits": [)";

		for (auto it=sort.begin();it!=sort.end();++it)
		{

			body.append("{\"mark\":").append(to_string(it->second->mark)).append(",\"visited_at\":"+to_string(it->first)).append(",\"place\": \"")
					.append(locations.at(it->second->location).place).append("\"},");

		}

		body.pop_back();
		body.append("]}");


		response.append("Content-Length: ");
		response.append(to_string(body.length()));
		response.append("\r\n\r\n");
		response.append(body);

		//		t2= chrono::steady_clock::now();
		//		cout << "make response cl "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;
		return 1;

	}
	response=R_empty_vis;

	return 1;
}

int fetchVisitsSorted(char *pos, string &response, uint32_t id)
{

	response=R200;
	body=R"({"visits": [ )";
	if (*(pos+6)!='?')
	{
		if(users[id].vis_sort.empty())
		{
			response=R_empty_vis;
			return 1;
		}
		for(auto it=users[id].vis_sort.begin();it!=users[id].vis_sort.end();++it)
		{
			body.append("{\"mark\":").append(to_string(visits.at(it->second).mark)).append(",\"visited_at\":"+to_string(it->first)).append(",\"place\": \"")
					.append(locations.at(visits.at(it->second).location).place).append("\"},");
		}
	}
	else
	{
		getVisParams(pos+6,vis_arr);
		if(users[id].vis_sort.empty())
		{
			response=R_empty_vis;
			return 1;
		}
		for(auto it=users[id].vis_sort.begin();it!=users[id].vis_sort.end();++it)
		{
			if(vis_arr[0].flag)//fromDate
			{
				if(it->first<vis_arr[0].value)continue;
			}
			if(vis_arr[1].flag)//toDate
			{
				if(it->first>vis_arr[1].value)continue;
			}
			if(vis_arr[2].flag)//toDistance
			{
				if(locations.at(visits.at(it->second).location).distance>=vis_arr[2].value)continue;
			}
			if(vis_arr[3].flag)//country
			{
				if(string::npos==vis_arr[3].country.find(locations.at(visits.at(it->second).location).enc_cntry))continue;
			}
			body.append("{\"mark\":").append(to_string(visits.at(it->second).mark)).append(",\"visited_at\":"+to_string(it->first)).append(",\"place\": \"")
					.append(locations.at(visits.at(it->second).location).place).append("\"},");
		}
	}


	body.pop_back();
	body.append("]}");


	response.append("Content-Length: ");
	response.append(to_string(body.length()));
	response.append("\r\n\r\n");
	response.append(body);

	return 1;


}








