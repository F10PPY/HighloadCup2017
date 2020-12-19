#include "headers.h"
extern unordered_map  <uint32_t,s_users> users;
extern unordered_map  <uint32_t,s_visits> visits;
extern unordered_map  <uint32_t,s_locations> locations;
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/writer.h"
using namespace rapidjson;
uint16_t year,day;
bool parseDataNl(unordered_map<uint32_t, s_users> &users)
{
	chrono::steady_clock::time_point t1,t2;
	t1= chrono::steady_clock::now();
	system("unzip -o ../tmp/data/data.zip -d unz");
	t2= chrono::steady_clock::now();
	std::cout << "unz = " << std::chrono::duration_cast<std::chrono::seconds> (t2 - t1).count() <<std::endl;
	string unz="unz";
	time_t timeNow;
	time(&timeNow);
	tm* tm_struct;
	tm_struct=gmtime(&timeNow);
	year=tm_struct->tm_year;
	day=tm_struct->tm_yday;;

	for (auto & p : fs::directory_iterator(unz))
	{
		ifstream i(p.path());
		if(!i.is_open()){cout<<"can't open"<<p.path()<<endl;return 0;}

		json j;
		i >> j;
		auto it = j.begin();
		//		if("locations"==it.key())
		//		{
		//			json sub=json::parse(it.value().dump());
		//			for (auto it = sub.begin(); it != sub.end(); ++it)
		//			{
		//				json obj=json::parse(it.value().dump());
		//				uint32_t id=obj["id"].get<uint32_t>();
		//				locations[id].place=obj["place"].get<string>();
		//				locations[id].country=obj["country"].get<string>();
		//				locations[id].enc_cntry=url_encode(obj["country"].get<string>());
		//				locations[id].city=obj["city"].get<string>();
		//				locations[id].distance=obj["distance"].get<int32_t>();
		//			}
		//		}
		if("users"==it.key())
		{
			json sub=json::parse(it.value().dump());
			for (auto it = sub.begin(); it != sub.end(); ++it)
			{
				json obj=json::parse(it.value().dump());
				uint32_t id=obj["id"].get<uint32_t>();
				users[id].email=obj["email"].get<string>();
				users[id].first_name=obj["first_name"].get<string>();
				users[id].last_name=obj["last_name"].get<string>();
				users[id].gender=obj["gender"].get<string>();
				users[id].birth_date=to_string(obj["birth_date"].get<int32_t>());

				time_t timeBorn=obj["birth_date"].get<time_t>();
				tm_struct=gmtime(&timeBorn);
				users[id].age=year-tm_struct->tm_year;
				if(day<tm_struct->tm_yday) users[id].age--;
				//	cout<<users[id].age<<endl;

			}
		}
		//		else if("visits"==it.key())
		//		{
		//			json sub=json::parse(it.value().dump());
		//			for (auto it = sub.begin(); it != sub.end(); ++it)
		//			{
		//				json obj=json::parse(it.value().dump());
		//				uint32_t id=obj["id"].get<uint32_t>();
		//				visits[id].location=obj["location"].get<uint32_t>();
		//				visits[id].user=obj["user"].get<uint32_t>();
		//				visits[id].visited_at=obj["visited_at"].get<int32_t>();
		//				visits[id].mark=obj["mark"].get<uint8_t>();

		//				users[visits[id].user].vis.insert(id);
		//				locations[visits[id].location].vis.insert(id);
		//			}
		//		}

	}
	t1= chrono::steady_clock::now();
	std::cout << "parse = " << std::chrono::duration_cast<std::chrono::seconds> (t1 - t2).count() <<std::endl;

	return 1;
}

bool parseData()
{
	chrono::steady_clock::time_point t1,t2;
	t1= chrono::steady_clock::now();
	system("unzip -o ../tmp/data/data.zip -d unz");
	t2= chrono::steady_clock::now();
    std::cout << "unziping took s: " << std::chrono::duration_cast<std::chrono::seconds> (t2 - t1).count() <<std::endl;
	string unz="unz";
	time_t timeNow;
	time(&timeNow);
	tm* tm_struct;
	tm_struct=gmtime(&timeNow);
	year=tm_struct->tm_year;
	day=tm_struct->tm_yday;;

	for (auto & p : fs::directory_iterator(unz))
	{
        ifstream i(p.path());
		if(!i.is_open()){cout<<"can't open"<<p.path()<<endl;return 0;}
		IStreamWrapper isw(i);
		Document doc;
		doc.ParseStream(isw);
		//		StringBuffer buffer;
		//		Writer<StringBuffer> writer(buffer);
		//		d.Accept(writer);
		//	cout << buffer.GetString() << std::endl;
		for (Value::ConstMemberIterator it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
		{
			string key=it->name.GetString();

			if("locations"==key)
			{
				for (SizeType i = 0; i < it->value.Size(); i++){

					uint32_t id=it->value[i]["id"].GetInt();
					locations[id].place=it->value[i]["place"].GetString();
					locations[id].country=it->value[i]["country"].GetString();
					locations[id].enc_cntry=url_encode(locations[id].country);
					locations[id].city=it->value[i]["city"].GetString();
					locations[id].distance=it->value[i]["distance"].GetInt();
				}

			}
			else if("users"==key)
			{
				for (SizeType i = 0; i < it->value.Size(); i++){

					uint32_t id=it->value[i]["id"].GetInt();

					users[id].email=it->value[i]["email"].GetString();
					users[id].first_name=it->value[i]["first_name"].GetString();
					users[id].last_name=it->value[i]["last_name"].GetString();
					users[id].gender=it->value[i]["gender"].GetString();
					users[id].birth_date=to_string(it->value[i]["birth_date"].GetInt());

					time_t timeBorn=it->value[i]["birth_date"].GetInt64();
					tm_struct=gmtime(&timeBorn);
					users[id].age=year-tm_struct->tm_year;
					if(day<tm_struct->tm_yday) users[id].age--;
				}
			}
			else if("visits"==key)
			{
				for (SizeType i = 0; i < it->value.Size(); i++){

					uint32_t id=it->value[i]["id"].GetInt();

					visits[id].location=it->value[i]["location"].GetInt();
					visits[id].user=it->value[i]["user"].GetInt();
					visits[id].visited_at=it->value[i]["visited_at"].GetInt();
					visits[id].mark=it->value[i]["mark"].GetInt();

					//users[visits[id].user].vis.insert(id);
					locations[visits[id].location].vis.insert(id);

					users[visits[id].user].vis_sort[visits[id].visited_at]=id;
				}
			}
		}
	}

	t1= chrono::steady_clock::now();
	std::cout << "parse = " << std::chrono::duration_cast<std::chrono::seconds> (t1 - t2).count() <<std::endl;

	return 1;
}
