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
	vector <string> wordMatrix[26][26];
	vector <int> wordSizeMatrix[26][26];
	int alphaMatrix[26][26];
	void insertWord(string wd){//������ת��ΪСд�����ճ��ȴ�С����˳�����
		transform(wd.begin(), wd.end(), wd.begin(), tolower);
		char a = *(wd.begin()); a -= 'a';
		char b = *(wd.end() - 1); b -= 'a';
		alphaMatrix[a][b] += 1;
		auto endFlag = wordMatrix[a][b].end();
		if (wordMatrix[a][b].size() == 0){//���������Ϊ��
			wordMatrix[a][b].push_back(wd);
			wordSizeMatrix[a][b].push_back(wd.size());
		}
		else{
			auto i = wordMatrix[a][b].begin();
			auto j = wordSizeMatrix[a][b].begin();
			bool flag = false;
			while(i != endFlag){//����ǿ�����
				if ((*i).size() > wd.size()){
					wordMatrix[a][b].insert(i, wd);
					wordSizeMatrix[a][b].insert(j, wd.size());
					flag = true;
					break;
				}
				i++;
				j++;
			}
			if (!flag){
				wordMatrix[a][b].push_back(wd);
				wordSizeMatrix[a][b].push_back(wd.size());
			}
		}
	}
public:
	Words(string inputFileName)
	{
		for (int i = 0; i < 26; i++){//��ʼ����������
			for (int j = 0; j < 26; j++){
				alphaMatrix[i][j] = 0;
			}
		}
		ifstream ifile(inputFileName);
		char ch;
		string wd = "";
		//�ִʲ�����insertWord����
		ifile.get(ch);
		while (!isalpha(ch)){
			ifile.get(ch);
		}
		while (!ifile.eof()){
			while (isalpha(ch)){
				wd += ch;
				ifile.get(ch);
			}
			if (wd.size() != 0){
				insertWord(wd);
			}
			ifile.get(ch);
			wd = "";
		}
	}
	void getAlphaMatrix(int a[26][26]){//����ĸ���󿽱���Ŀ��λ��
		for (int i = 0; i < 26; i++){
			memcpy(a[i], alphaMatrix[i], 26 * sizeof(int));
		}
	}
	void getWordSizeMatrix(vector<int> a[26][26]){//�������ʳ��Ⱦ���
		for (int i = 0; i < 26; i++){
			for (int j = 0; j < 26; j++){
				a[i][j] = wordSizeMatrix[i][j];
			}
		}
	}
	string getWord(int h, int t, int n){
		return wordMatrix[h][t][n];
	}
	void printWordMatrix(){//�����ȡ����Թ�����
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