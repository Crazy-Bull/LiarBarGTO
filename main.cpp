#include <bits/stdc++.h>
#include "configreader.cpp"
using namespace std;

double min_freq = 0.001;

struct FrequencyInfoReadOnly 
{
	double freq[7];	//0:Liar, 1~3:Real, 4~6:Fake
	bool available[7];
	
	FrequencyInfoReadOnly()
	{
		for(int i=0;i<=6;i++)
		{
			freq[i]=0;
			available[i]=0;
		}
	}
	
	void reallocate()
	{
		double sum_major = 0;
		for (int i=0;i<=6;i++)
		{
			if (freq[i]>min_freq+1e-10)
			{
				sum_major += freq[i];
			}
		}
		
		for (int i=0;i<=6;i++)
		{
			if (freq[i]>min_freq+1e-10)
			{
				freq[i] /= sum_major;
			}
			else
			{
				freq[i] = 0;
			}
		}
	} 
	
	void disp()
	{
		cout<<fixed<<setprecision(2)<<endl;
		cout<<"-----------------"<<endl;
		for(int i=0;i<=6;i++)
		{
			if(available[i])
			{
				cout<<"Strategy "<<i<<"- Frequency: "<<freq[i]*100<<"%"<<endl;
			}
		}
		cout<<"-----------------"<<endl;
	} 
};

int read_config(string path)
{
	ConfigReader config;
    if (config.loadConfig(path)) 
	{
        min_freq = config.getValue("min_freq", 0.001);
    }
    return 0;
}

map<string,FrequencyInfoReadOnly*> StrategySet;

int loadStrategy(string path, bool do_reallocate)
{
	cout<<"Loading Strategy..."<<endl;
    ifstream fin(path);
    
    if (!fin.is_open()) 
	{
        cerr << "无法打开文件: " << fin << endl;
        return 1;
    }
    
    string line;
    
    while (std::getline(fin, line)) 
	{
        // 跳过空行和注释
        if (line.empty() || line[0] == '#') continue;
        
        istringstream iss(line);
        string key, freq, available;
        
        while (getline(iss, key, ','))
        {
        	FrequencyInfoReadOnly* tmp = new FrequencyInfoReadOnly;
        	// 使用逗号分割
        	for (int i=0;i<=6;i++) 
	        	if (getline(iss, freq, ',') && getline(iss, available, ',')) 
				{
		            // 去除可能的空格
		            freq.erase(0, freq.find_first_not_of(" \t"));
		            freq.erase(freq.find_last_not_of(" \t") + 1);
		            available.erase(0, available.find_first_not_of(" \t"));
		            available.erase(available.find_last_not_of(" \t") + 1);
		            
		            tmp->freq[i]=stod(freq);
		            tmp->available[i]=stoi(available);
	        	}
        	
        	if(do_reallocate) tmp->reallocate();
        	StrategySet[key] = tmp;
		}
    }
    
    fin.close();
    cout<<"Loading complete"<<endl;
    return 0;
}


int main(int argc, char** argv) {
	//load strategy set
	loadStrategy("strat.csv", true);
	
	cout<<"Input a query string, type \"exit\" to quit."<<endl;
	cout<<"Strategy 0 means calling \"Liar\"."<<endl;
	cout<<"Strategy 1/2/3 means playing 1/2/3 real cards."<<endl;
	cout<<"Strategy 4/5/6 means playing 1/2/3 fake cards."<<endl;

	while(1)
	{
		string s;
		cin>>s;
		if(s=="exit") break;
		else if(StrategySet.count(s))
		{
			StrategySet[s]->disp();
		}
		else cout<<"Not a legal input"<<endl;
	}

	return 0;
}
