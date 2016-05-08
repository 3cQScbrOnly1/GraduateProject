#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace std;
using namespace boost;

int main(int argc , char *argv[])
{
	ifstream need_extend(argv[1]);
	string line;
	while(getline(need_extend,line))	
	{
		vector<string> words;		
		split(words,line,is_any_of(" "));
		for(int i = 0; i < words.size(); i++)
		{
			int pos = words[i].find("/j");
			if(pos != -1)
			{
				string short_word = words[i].substr(0,pos);
				cout << short_word << endl;
			}
		}
	}
	need_extend.close();
	return 0;
}
