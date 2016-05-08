#include <map>
#include <vector>
#include <string>

using namespace std;

class extends
{
	private:
		void insert_to_all_extends(vector<string> data);
	public:
		extends();
		map<string,vector<string> > all_extends;
		extends(string extends_path);
		void load_extends(string extends_path);
		void show();
};

class combos
{
	public:
		vector<string> all_combos;
		void load_combos(string combo_path);
};

class static_dic
{
	public:
		map<string,vector<string> > all_static_dic;
		void load_static_dic(string static_dic_path);
};
