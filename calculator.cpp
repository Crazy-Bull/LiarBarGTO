#include "solve.h"
#include "configreader.cpp"
using namespace std;

unsigned int iterations = 1000; 
double alpha_0 = 0.02;
double decay_rate = 0.001;
double min_freq = 0.001;
string path = "strat.csv";

int read_config(string path)
{
	ConfigReader config;
    if (config.loadConfig(path)) {
        iterations = config.getValue("iterations", 1000);
        alpha_0 = config.getValue("alpha_0", 0.02);
        decay_rate = config.getValue("decay", 0.001);
        min_freq = config.getValue("min_freq", 0.001);
    }
    cout<<iterations<<endl;
    return 0;
}

int save(string path)
{
	ofstream fout(path);
	for(auto iter=DecisionVars.begin(); iter!=DecisionVars.end(); iter++)
	{
		string str = iter->first;
		auto p = iter->second;
		fout << str <<",";
		for(int i=0;i<=6;i++)
		{
			fout << p->freq[i] <<","<<p->available[i]<<",";
		}
		fout<<endl;
	}
	fout.close();
	return 0;
}

int main()
{
	init();
	cout<<records.size()<<endl;
	cout<<calc_ev()<<endl;
	// string watcher="?22";
	// DecisionVars[watcher]->disp();
	read_config("config.txt");
	for(int i=1;i<=iterations;i++)
	{
		double alpha=alpha_0/(1+i*decay_rate);
		cout<<"Iteration "<<i<<"/"<<iterations<<":"<<endl;
		exploit(alpha,min_freq,0);
		cout<<"1st player exploit complete.  ";
		cout<<"EV:"<<calc_ev()<<endl;
		exploit(alpha,min_freq,1);
		cout<<"2nd player exploit complete.  ";
		cout<<"EV:"<<calc_ev()<<endl;
	}
	
	cout<<"Model Calculation Complete. Saving"<<endl;
	save(path);
	
	cout<<"Model saved."<<endl;
	

	return 0;
}
