#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main(int argc,char *argv[])
{
	ifstream need_extend_file(argv[1]);
	ifstream my_answer(argv[2]);
	string line;
	while(getline(need_extend_file,line))
	{
		vector<string> words;	
		split(words,line,is_any_of(" "));
		for(int i = 0; i < words.size(); i++)
		{
			if(words[i].find("/j") != -1)
			{
				string answer;
				my_answer >> answer;
				if(answer == "NULL")
					cout<< words[i] << " ";
				else
					cout << answer << "/j" << " ";
			}
			else
			{
				if(i+1 == words.size())
					cout << words[i] << endl;
				else
					cout << words[i] << " ";
			}
		}
	}
	my_answer.close();
	need_extend_file.close();
	return 0;
}
