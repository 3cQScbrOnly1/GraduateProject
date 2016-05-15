#include <iostream>
#include <fstream>

using namespace std;

int main(int argc,char *argv[])
{
	ifstream answer(argv[1]);
	ifstream result(argv[2]);
	ifstream input(argv[3]); // short
	ifstream mark(argv[4]);

	double all_num=0,correct_num=0,null=0;
	double sh_right_same = 0,sh_right_different = 0,sh_all_same = 0 , sh_all_different;
	string words_of_answer,words_of_result,words_of_input,words_of_mark;
	while(getline(result,words_of_result)
			&&getline(answer,words_of_answer)
			&&getline(input,words_of_input)
			&&getline(mark,words_of_mark))
	{
		if(words_of_result==words_of_input)
			null++;
		if(words_of_answer==words_of_result)
		{
			correct_num++;
			if(words_of_mark == "sh_same")
				sh_right_same++;
			if(words_of_mark == "sh_different")
				sh_right_different++;
		}
		if(words_of_mark == "sh_same")
			sh_all_same++;
		if(words_of_mark == "sh_different")
			sh_all_different++;
		all_num++;
	}
	cout<<argv[2]<<endl;
	double r=(correct_num/all_num)*100;
	double c=(correct_num/(all_num-null))*100;
	double f=(2*c*r)/(c+r);
	cout << "sh right same / sh all same: " << (sh_right_same / sh_all_same) * 100 << "%" << endl;
	cout << "sh right different / sh_all different: " << (sh_right_different / sh_all_different) * 100<< "%" << endl;
	cout<<"correct rate: "<<c<<"%"<<endl;
	cout<<"recall rate: "<<r<<"%"<<endl;
	cout<<"F: "<<f<<"%"<<endl;

	//right_result.close();
	input.close();
	answer.close();
	result.close();
	mark.close();
	return 0;
}
