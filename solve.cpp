#include <bits/stdc++.h>
#include "solve.h"
using namespace std; 

const int REAL_NUM=8;
const int TOTAL_NUM=20;

vector<string> records;
map<string,FrequencyInfo*> DecisionVars;
double start_hand_prob[6][6];

string mask(string s)
{
	string ret(s);
	int len=s.length();
	for(int i=len-1;i>=0;i-=2)
	{
		if(i==0||i==1) ret[i]='?';
		else if(ret[i]>='4') ret[i]-=3;
	}
	return ret;
}


void dfs(int t1, int r1, int t2, int r2, int pl, string prefix)
{
	string masked=mask(prefix);
	bool exist_flag=DecisionVars.count(masked);
	FrequencyInfo *tmp;
	if(!exist_flag)
		tmp=new FrequencyInfo;
	if(t1!=5)
	{
		records.push_back(prefix+"0");
		if(!exist_flag) tmp->enable(0);
	}
	if(pl==0)
	{
		if(t2!=0)
		for(int i=1;i<=3;i++)
		{
			if(r1>=i) 
			{
				dfs(t1-i,r1-i,t2,r2,1,prefix+string(1,'0'+i));
				if(!exist_flag) tmp->enable(i);
			}
			if(t1>=i&&r1!=t1)
			{
				dfs(t1-i,min(t1-i,r1),t2,r2,1,prefix+string(1,'3'+i));
				if(!exist_flag) tmp->enable(3+i);
			}
		}
	}
	else if(pl==1)
	{
		if(t1!=0)
		for(int i=1;i<=3;i++)
		{
			if(r2>=i)
			{
				dfs(t1,r1,t2-i,r2-i,0,prefix+string(1,'0'+i));
				if(!exist_flag) tmp->enable(i);
			}
			if(t2>=i&&r2!=t2) 
			{
				dfs(t1,r1,t2-i,min(t2-i,r2),0,prefix+string(1,'3'+i));
				if(!exist_flag) tmp->enable(3+i);
			}
		}
	}
	if(!exist_flag)
	{
		tmp->init_freq_uniform();
		DecisionVars[masked]=tmp;
	}
}

void init()
{
	unsigned long long fac[21];
	fac[0]=1;
	for(int i=1;i<=20;i++) fac[i]=fac[i-1]*i;
	unsigned long long denom=fac[20]/fac[5]/fac[5]/fac[10];
	for(int r1=0;r1<=5;r1++)
		for(int r2=0;r2<=min(5,8-r1);r2++)
		{
			dfs(5,r1,5,r2,0,string(1,'0'+r1)+string(1,'0'+r2));
			unsigned long long nom1=fac[8]/fac[r1]/fac[r2]/fac[8-r1-r2];
			unsigned long long nom2=fac[12]/fac[5-r1]/fac[5-r2]/fac[r1+r2+2];
			start_hand_prob[r1][r2]=(double)nom1*nom2/denom;
		}
}


bool first_win(string s)
{
	int len=s.length();
	assert(len>=3);
	assert(s[len-1]=='0');
	return (len%2) ^ (s[len-2]<='3');
}

double calc_ev()
{
	double sum=0;
	for(auto iter=records.begin(); iter!=records.end(); iter++)
	{
		string str=*iter;
		double tmp=start_hand_prob[str[0]-'0'][str[1]-'0'];
		if(first_win(str))
		{
			string slice=str.substr(0,2);
			for(int i=2;i<=str.length()-1;i++)
			{
				tmp*=DecisionVars[mask(slice)]->freq[str[i]-'0'];
				slice.append(string(1,str[i]));
			}
			sum+=tmp;
			slice=str.substr(0,2);
			for(int i=2;i<=str.length()-1;i++)
			{
				DecisionVars[mask(slice)]->s[str[i]-'0']+=tmp;
				slice.append(string(1,str[i]));
			}
		}
	}
	return sum;
}

void exploit(double alpha, double min_freq, bool pl)
{
	for(auto iter=DecisionVars.begin(); iter!=DecisionVars.end(); iter++)
	{
		string str=iter->first;
		if(pl==0&&str.length()%2==0)
		{
			iter->second->grad_descent(alpha,min_freq,1);
			iter->second->clear_grad();
		}
		if(pl==1&&str.length()%2==1)
		{
			iter->second->grad_descent(alpha,min_freq,-1);
			iter->second->clear_grad();
		}
	}
}

