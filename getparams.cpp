#include "headers.h"
static  thread_local char * beg;
static  thread_local size_t i;
int getVisParams(char * pch, vis_params *arr)
{
	arr[0].flag=0;
	arr[1].flag=0;
	arr[2].flag=0;
	arr[3].flag=0;
	beg=pch;

	while(true)
	{
		while(*pch!='=')
		{
			++pch;
		}
		++pch;
		if(*(beg+4)=='m')//fromDate
		{
			arr[0].value=stoi(pch);
			arr[0].flag=1;
			while(*pch!='&')
			{
				if(*pch==' ')return 1;
				if(!isdigit(*pch))throw invalid_argument( "invalid val" );
				++pch;
			}
		}
		else if(*(beg+4)=='a')//toDate
		{
			arr[1].value=stoi(pch);
			arr[1].flag=1;
			while(*pch!='&')
			{
				if(*pch==' ')return 1;
				if(!isdigit(*pch))throw invalid_argument( "invalid val" );
				++pch;
			}
		}
		else if(*(beg+4)=='i')//toDistance
		{
			arr[2].value=stoi(pch);
			arr[2].flag=1;
			while(*pch!='&')
			{
				if(*pch==' ')return 1;
				if(!isdigit(*pch))throw invalid_argument( "invalid val" );
				++pch;
			}
		}
		else if (*(beg+4)=='n')//country
		{
			i=0;
			arr[3].flag=1;
			while(*pch!='&')
			{
				arr[3].country[i]=*pch;
				++pch;
				if(*pch==' ')return 1;
				++i;
			}
		}
		else
		{
			throw invalid_argument( "invalid val" );
		}
		beg=pch;

	}
	return 1;
}
int getLocParams(char * pch, loc_params *arr)
{
	arr[0].flag=0;
	arr[1].flag=0;
	arr[2].flag=0;
	arr[3].flag=0;
	arr[4].flag=0;
	beg=pch;

	while(true)
	{
		while(*pch!='=')
		{
			++pch;
		}
		++pch;
		if(*(beg+5)=='D')//fromDate
		{
			arr[0].value=stoi(pch);
			arr[0].flag=1;
			while(*pch!='&')
			{
				if(*pch==' ')return 1;
				if(!isdigit(*pch))throw invalid_argument( "invalid val" );
				++pch;
			}
		}
		else if(*(beg+5)=='t')//toDate
		{
			arr[1].value=stoi(pch);
			arr[1].flag=1;
			while(*pch!='&')
			{
				if(*pch==' ')return 1;
				if(!isdigit(*pch))throw invalid_argument( "invalid val" );
				++pch;
			}
		}
		else if(*(beg+5)=='A')//fromAge
		{
			arr[2].value=stoi(pch);
			arr[2].flag=1;
			while(*pch!='&')
			{
				if(*pch==' ')return 1;
				if(!isdigit(*pch))throw invalid_argument( "invalid val" );
				++pch;
			}
		}
		else if(*(beg+4)=='g')//toAge
		{
			arr[3].value=stoi(pch);
			arr[3].flag=1;
			while(*pch!='&')
			{
				if(*pch==' ')return 1;
				if(!isdigit(*pch))throw invalid_argument( "invalid val" );
				++pch;
			}
		}
		else if (*(beg+1)=='g')//gender
		{
			if('m'==*pch)arr[4].gender='m';
			else if('f'==*pch)arr[4].gender='f';
			else throw invalid_argument( "invalid val" );
			arr[4].flag=1;
			if(*(++pch)!='&')
			{
				if(*pch==' ')return 1;
				throw invalid_argument( "invalid val" );
			}

		}
		else
		{
			throw invalid_argument( "invalid val" );
		}
		beg=pch;

	}
	return 1;
}
