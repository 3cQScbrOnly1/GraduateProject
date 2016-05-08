#include <map>
#include <vector>
#include <string>

using namespace std;

class static_dic
{
	public:
		map<string,vector<string> > all_static_dic;
		void load_static_dic(string static_dic_path);
};
