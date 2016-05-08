#include <iostream>
#include <fstream>

using namespace std;

int main(int argc,char *argv[])
{
	ifstream answer(argv[1]);
	ifstream result(argv[2]);
	//ofstream right_result(right_result_name.c_str());

	double all_num=0,correct_num=0,null=0;
	string words_of_answer,words_of_result;
	while((getline(result,words_of_result))&&(getline(answer,words_of_answer)))
	{
		if(words_of_result=="NULL")
			null++;
		//cout<<words_of_result<<" "<<words_of_answer;
		if(words_of_answer==words_of_result)
		{
			correct_num++;
			//right_result<<words_of_result<<endl;
		}
		//if(words_of_result!="NULL")
		all_num++;
	}
	cout<<argv[2]<<endl;
	double r=(correct_num/all_num)*100;
	double c=(correct_num/(all_num-null))*100;
	double f=(2*c*r)/(c+r);
	cout<<"correct rate: "<<c<<"%"<<endl;
	cout<<"recall rate: "<<r<<"%"<<endl;
	cout<<"F: "<<f<<"%"<<endl;

	//right_result.close();
	answer.close();
	result.close();
	return 0;
}
