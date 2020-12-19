#include "headers.h"
extern unordered_map  <uint32_t,s_users> users;
extern unordered_map  <uint32_t,s_visits> visits;
extern unordered_map  <uint32_t,s_locations> locations;
extern uint16_t year,day;
static  thread_local uint32_t id;
static  thread_local tm * tm_struct;
static  thread_local json json_obj,json_temp;
extern  thread_local string body;
static  thread_local char * pos;
thread_local chrono::steady_clock::time_point t1,t2;
thread_local clock_t t;

int reqHandler(char *buff,string &response)
{
	pos=strstr(buff,"GET /users/");
	if(pos)
	{
		id=atoi(pos+11);
		//		t = clock();
		//		t1= chrono::steady_clock::now();
		//		cout<<users[id].email<<endl;
		//		cout<<"GET /users/ cpu "<<clock()-t<<endl;
		//		t2= chrono::steady_clock::now();
		//		cout << "time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;

		//		t = clock();
		//		t1= chrono::steady_clock::now();
		//		cout<<users.at(id).email<<endl;
		//		cout<<"GET /users/ cpu "<<clock()-t<<endl;
		//		t2= chrono::steady_clock::now();
		//		cout << "time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;
		users.at(id);
		pos=strstr(buff,"visits");
		if(pos)
		{
			//			t = clock();
			//			t1= chrono::steady_clock::now();
			//			fetchVisits(pos,response,id);
			//			t2= chrono::steady_clock::now();
			//			cout << "fetchvisits cl "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;
			//			t = clock();
			//			t1= chrono::steady_clock::now();
			fetchVisitsSorted(pos,response,id);
			//			t2= chrono::steady_clock::now();
			//			cout << "fetchvisits sorted cl "<<clock()-t<<", time " << chrono::duration_cast<chrono::microseconds> (t2 - t1).count() <<endl;
			return 1;
		}
		body="{\"id\":"+to_string(id)+",\"email\":\""+users.at(id).email+"\",\"first_name\":\""+users.at(id).first_name+
				"\",\"last_name\":\""+users.at(id).last_name+"\",\"gender\":\""+users.at(id).gender+"\",\"birth_date\":"+users.at(id).birth_date+"}";
		response=R200;


		response.append("Content-Length: ");
		response.append(to_string(body.length()));
		response.append("\r\n\r\n");
		response.append(body);


		return 1;
	}
	pos=strstr(buff,"GET /locations/");
	if(pos)
	{
		id=atoi(pos+15);

		locations.at(id);
		pos=strstr(buff,"avg");
		if(pos)
		{
			countAvg(pos,response,id);
			return 1;
		}
		body="{\"id\":"+to_string(id)+",\"distance\":"+to_string(locations.at(id).distance)+",\"place\":\""+locations.at(id).place+
				"\",\"country\":\""+locations.at(id).country+"\",\"city\":\""+locations.at(id).city+"\"}";
		response=R200;


		response.append("Content-Length: ");
		response.append(to_string(body.length()));
		response.append("\r\n\r\n");
		response.append(body);
		return 1;
	}
	pos=strstr(buff,"GET /visits/");
	if(pos)
	{
		id=atoi(pos+12);
		body="{\"user\":"+to_string(visits.at(id).user)+",\"id\":"+to_string(id)+",\"location\":"+to_string(visits.at(id).location)+
				",\"visited_at\":"+to_string(visits.at(id).visited_at)+",\"mark\":"+to_string(visits.at(id).mark)+"}";
		response=R200;


		response.append("Content-Length: ");
		response.append(to_string(body.length()));
		response.append("\r\n\r\n");
		response.append(body);
		return 1;
	}
	try
	{
		pos=strstr(buff,"POST /users/new");
		if(pos)
		{
			pos=strstr(buff,"\r\n\r\n");
			json_obj=json::parse(pos+4);
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)//check correctness
			{
				if(("birth_date"==it.key())||("id"==it.key()))
				{
					if(!it.value().is_number())
					{
						response=R400;
						return 1;
					}
				}
				else if(it.value().is_null())
				{
					response=R400;
					return 1;
				}
			}
			id=json_obj["id"];//.get<uint32_t>();
			users[id].email=json_obj["email"];//.get<string>();
			users[id].first_name=json_obj["first_name"];//.get<string>();
			users[id].last_name=json_obj["last_name"];//.get<string>();
			users[id].gender=json_obj["gender"];//.get<string>();
			users[id].birth_date=to_string(json_obj["birth_date"].get<int32_t>());

			time_t timeBorn=json_obj["birth_date"].get<time_t>();
			tm_struct=gmtime(&timeBorn);
			users[id].age=year-tm_struct->tm_year;
			if(day<tm_struct->tm_yday) users[id].age--;
			response=RPOST;
			return 1;
		}
		pos=strstr(buff,"POST /locations/new");
		if(pos)
		{
			pos=strstr(buff,"\r\n\r\n");
			json_obj=json::parse(pos+4);
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)//check correctness
			{
				if(("distance"==it.key())||("id"==it.key()))
				{
					if(!it.value().is_number())
					{
						response=R400;
						return 1;
					}
				}
				else if(it.value().is_null())
				{
					response=R400;
					return 1;
				}
			}
			id=json_obj["id"];
			locations[id].place=json_obj["place"];
			locations[id].country=json_obj["country"];
			//	locations[id].enc_cntry=url_encode(json_obj["country"]);
			locations[id].enc_cntry=url_encode(locations[id].country);
			locations[id].city=json_obj["city"];
			locations[id].distance=json_obj["distance"];
			response=RPOST;
			return 1;
		}
		pos=strstr(buff,"POST /visits/new");
		if(pos)
		{
			pos=strstr(buff,"\r\n\r\n");
			json_obj=json::parse(pos+4);
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)//check correctness
			{
				if(!it.value().is_number())
				{
					response=R400;
					return 1;
				}
			}
			id=json_obj["id"];
			visits[id].location=json_obj["location"];
			visits[id].user=json_obj["user"];
			visits[id].visited_at=json_obj["visited_at"];
			visits[id].mark=json_obj["mark"];

			//users[visits[id].user].vis.insert(id);
			locations[visits[id].location].vis.insert(id);

			users[visits[id].user].vis_sort[visits[id].visited_at]=id;
			response=RPOST;
			return 1;
		}
		pos=strstr(buff,"POST /users/");
		if(pos)
		{
			id=atoi(pos+12);
			auto it = users.find(id);
			if (it == users.end())
			{
				response=R404;
				return 1;
			}
			pos=strstr(buff,"\r\n\r\n");
			//pos=strstr(buff,"nnnn");
			json_obj=json::parse(pos+4);
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)//check correctness
			{
				if("birth_date"==it.key()&&!it.value().is_number())
				{
					response=R400;
					return 1;
				}
				else if(it.value().is_null())
				{
					response=R400;
					return 1;
				}
			}
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)
			{
				if("email"==it.key())
				{
					users[id].email=json_obj["email"];//.get<string>();
				}
				else if("first_name"==it.key())
				{
					users[id].first_name=json_obj["first_name"];//.get<string>();
				}
				else if("last_name"==it.key())
				{
					users[id].last_name=json_obj["last_name"];//.get<string>();
				}
				else if("gender"==it.key())
				{
					users[id].gender=json_obj["gender"];//.get<string>();
				}
				else if("birth_date"==it.key())
				{
					users[id].birth_date=to_string(json_obj["birth_date"].get<int32_t>());


					time_t timeBorn=json_obj["birth_date"].get<time_t>();
					tm_struct=gmtime(&timeBorn);
					users[id].age=year-tm_struct->tm_year;
					if(day<tm_struct->tm_yday) users[id].age--;
				}
				else
				{
					response=R400;
					return 1;
				}
			}
			response=RPOST;
			return 1;
		}
		pos=strstr(buff,"POST /locations/");
		if(pos)
		{
			id=atoi(pos+16);
			auto it = locations.find(id);
			if (it == locations.end())
			{
				response=R404;
				return 1;
			}
			pos=strstr(buff,"\r\n\r\n");
			json_obj=json::parse(pos+4);
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)//check correctness
			{
				if(("distance"==it.key())||("id"==it.key()))
				{
					if(!it.value().is_number())
					{
						response=R400;
						return 1;
					}
				}
				else if(it.value().is_null())
				{
					response=R400;
					return 1;
				}
			}
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)
			{
				if("place"==it.key())
				{
					locations[id].place=json_obj["place"];
				}
				else if("country"==it.key())
				{
					locations[id].country=json_obj["country"];
					locations[id].enc_cntry=url_encode(locations[id].country);
				}
				else if("city"==it.key())
				{
					locations[id].city=json_obj["city"];
				}
				else if("distance"==it.key())
				{
					locations[id].distance=json_obj["distance"];
				}
				else
				{
					response=R400;
					return 1;
				}
			}
			response=RPOST;
			return 1;
		}
		pos=strstr(buff,"POST /visits/");
		if(pos)
		{
			id=atoi(pos+13);
			auto it = visits.find(id);
			if (it == visits.end())
			{
				response=R404;
				return 1;
			}
			pos=strstr(buff,"\r\n\r\n");
			json_obj=json::parse(pos+4);
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)//check correctness
			{
				if(!it.value().is_number())
				{
					response=R400;
					return 1;
				}
			}
			for (auto it = json_obj.begin(); it != json_obj.end(); ++it)
			{
				if("location"==it.key())
				{
					locations[visits[id].location].vis.erase(id);
					visits[id].location=json_obj["location"];
					locations[visits[id].location].vis.insert(id);
				}
				else if("user"==it.key())
				{
					users[visits[id].user].vis_sort.erase(visits[id].visited_at);
					visits[id].user=json_obj["user"];
					users[visits[id].user].vis_sort[visits[id].visited_at]=id;
				}
				else if("visited_at"==it.key())
				{
					users[visits[id].user].vis_sort.erase(visits[id].visited_at);
					visits[id].visited_at=json_obj["visited_at"];
					users[visits[id].user].vis_sort[visits[id].visited_at]=id;
				}
				else if("mark"==it.key())
				{
					visits[id].mark=json_obj["mark"];
				}
				else
				{
					response=R400;
					return 1;
				}
			}
			response=RPOST;
			return 1;
		}
	}
	//	catch(json::parse_error &e)
	//	{
	//			cout<<buff<<endl;
	//			cout<<e.what()<<endl;
	//		response=R400;
	//		return 1;
	//	}
	catch(json::exception &e)
	{
		//cout<<buff<<endl;
		//cout<<e.what()<<endl;
		response=R400;
		return 1;
	}
	response=R404;
	return 1;
}
