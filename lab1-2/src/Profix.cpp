/***
2019.3.12 by zhengGei
***********
Demo of profix of document, turns all the words into lowercase, and saves them in the matrix of vector wordMatrix
Classify the words by their head and tail
***********
example:
Words ex(input file path);
int matrix = ex.getAlphaMatrix();
then 'matrix' gets the adjacent matrix
***********
still having not found a good way to deal with the wordMatrix
error handling not completed
***/


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
using namespace std;

class Words
{
private:
	vector <string> wordMatrix[26][26];  //用于保存字符串的向量矩阵，[i][j]中表示从i开始j结束的所有单词组成的向量
	vector <int> wordSizeMatrix[26][26]; //用于保存单词长的向量矩阵，[i][j]中表示从i开始j结束的所有单词长度的向量
	int alphaMatrix[26][26];
	int total_word_num;
	void insertWord(string wd){//将单词转换为小写并按照长度从小到大顺序插入
		transform(wd.begin(), wd.end(), wd.begin(), ::tolower);
		char a = *(wd.begin()); a -= 'a';    //a表示头部
		char b = *(wd.end() - 1); b -= 'a';  //b表示尾部
		//alphaMatrix[a][b] += 1;    			 //此时矩阵中对应位置加一
		auto endFlag = wordMatrix[a][b].end();
		if (wordMatrix[a][b].size() == 0){  //如果此向量为空
			total_word_num ++;
			wordMatrix[a][b].push_back(wd); 
			wordSizeMatrix[a][b].push_back(wd.size());
			alphaMatrix[a][b] += 1;    			 //此时矩阵中对应位置加一
		}
		else{	//如果不为空，为了做到按序存放，需要进行调整。
			auto i = wordMatrix[a][b].begin();  //得到向量头部
			auto j = wordSizeMatrix[a][b].begin();
			bool flag = false;
			while(i != endFlag){//从头扫描到尾
				if ((*i).size() > wd.size()) //找到了合适的插入位置了
				{
					total_word_num++;
					wordMatrix[a][b].insert(i, wd);  //在对应的位置插入
					wordSizeMatrix[a][b].insert(j, wd.size());
					alphaMatrix[a][b] += 1;    			 //此时矩阵中对应位置加一
					flag = true;
					break;
				}
				else
				{
					if ((*i).size() == wd.size())   //这里试着进行了重复单词的检测工作。
					{								//当两个单词长度一致并且首尾字母相同，则有着出现重复单词的风险。
						string temp_str = *i;		
						//cout<<temp_str<<endl;
						//cout<<wd<<endl;
						if (*i == wd) 
						{
							flag = true;
							break;
						}
					}
				}
				i++;
				j++;
			}
			if (!flag)  //如果flag为假，表示这个时候还没有插入，直到循环结束仍然不满足要求，证明是最短的
			{
				total_word_num++;
				wordMatrix[a][b].push_back(wd);	
				wordSizeMatrix[a][b].push_back(wd.size());
				alphaMatrix[a][b] += 1;    			 //此时矩阵中对应位置加一
			}
		}
	}
public:
	Words(string inputFileName)  
	{
		total_word_num = 0;
		for (int i = 0; i < 26; i++){	//初始化计数矩阵
			for (int j = 0; j < 26; j++){
				alphaMatrix[i][j] = 0;
			}
		}
		ifstream ifile(inputFileName);  //装入文件
		char ch;
		string wd = "";
		//分词并调用insertWord函数
		ifile.get(ch); //读入当前文字
		while (!isalpha(ch)&&!ifile.eof()){ //持续读入，直到读到字母
			ifile.get(ch);
		}
		while (!ifile.eof()){
			while (isalpha(ch)&&!ifile.eof()){
				wd += ch;
				if (ifile.eof()) break;
				ifile.get(ch);
			}
			if (wd.size() != 0){
				//cout<<wd<<endl;
				//total_word_num ++ ;
				insertWord(wd);
			}
			if (!ifile.eof())
				ifile.get(ch);
			wd = "";
		}
		//cout<<"fuck"<<endl;
	}
	Words(vector<string> inputWords, int len){//这里假设测试用样例没有非法单词
		total_word_num = 0;
		for (int i = 0; i < 26; i++){	//初始化计数矩阵
			for (int j = 0; j < 26; j++){
				alphaMatrix[i][j] = 0;
			}
		}
		string wd = "";
		for (int i = 0; i < len; i++){
			wd = inputWords[i];
			insertWord(wd);
		}
	}

	int get_word_num()
	{
		return total_word_num;
	}

	string deleteword(int h, int t, int n)
	{
		string str = getWord(h, t, n);
		wordMatrix[h][t].erase(wordMatrix[h][t].begin()+n);
		alphaMatrix[h][t]-=1;
		return str;
	}
	void insertword( int h, int t, int n, string str)
	{
		wordMatrix[h][t].insert(wordMatrix[h][t].begin()+n,str);
		alphaMatrix[h][t]+=1;
	}

	void getAlphaMatrix(int a[26][26]){//将字母矩阵拷贝到目标位置
		for (int i = 0; i < 26; i++){
			memcpy(a[i], alphaMatrix[i], 26 * sizeof(int));
		}
	}
	void getWordSizeMatrix(vector<int> a[26][26]){//拷贝单词长度矩阵
		for (int i = 0; i < 26; i++){
			for (int j = 0; j < 26; j++){
				a[i][j] = wordSizeMatrix[i][j];
			}
		}
	}
	string getWord(int h, int t, int n){  //得到以h开头，t结尾，第n个单词
		return wordMatrix[h][t][n];
	}
	void printWordMatrix(){//输出读取结果以供测试
		for (int i = 0; i < 26; i++){
			for (int j = 0; j < 26; j++){
				cout << (char)(i + 'a') << " " << (char)(j + 'a') << ": ";
				for (auto iter : wordMatrix[i][j]){
					string str = iter;
					cout << str << ", ";
				}
				cout << endl;
			}
		}
	}
};
/*int main(){
	Words p("testfile.txt");
	p.printWordMatrix();
	system("pause");
}*/