/***
2019.3.13
part 1,2,3 completed and debugged simply.
output a file "solution.txt" contains the result.
simply uses DFS to solve the problem


***/



#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include "Profix.cpp"
using namespace std;

struct TailandLength{
	int tail;
	int length;
};//���浱ǰ�ڵ��β�Ͷ�Ӧ���ʵĳ���

class wordChain {
private:
	int alphaMatrix[26][26];
	vector <int> wordSizeMatrix[26][26];
	stack <int> resultChain;
	stack <TailandLength> resultChainL;
	vector <stack <int>> resultChainN;
	int currentLength;
	int resultLength;
	void error(int errorID) {

	}
	void findLongest(char h, char t) {//�ҳ�������൥��������
		if (h != 0) {
			findLongestH(h - 'a', t - 'a');
		}
		else {//���û��h��������ÿ����ĸ��Ϊͷ������һ��
			for (int i = 0; i < 26; i++) {
				findLongestH(i, t - 'a');
			}
		}
	}
	void findLongestH(int h, int t) {//�涨��������ͷѰ��Ŀ����
		//����ջ��Ϊ��ʱ���ڴ���ĵ��ʵ�ͷ
		stack <int> st;
		st.push(h);
		int ThisHead;
		int ThisTail = 0;
		while (!st.empty()) {
			ThisHead = st.top();
			while (ThisTail != 26 && alphaMatrix[ThisHead][ThisTail] == 0) {//Ѱ���¸��ڵ�
				ThisTail++;
			}
			if (ThisTail == 26) {
				//�Ҳ�����һ���ڵ㣬��˽ڵ㴦�����,��ջ,�ж��Ƿ���½��������
				if (st.size() > resultChain.size() && ((t < 0) || (st.top() == t))) {
					resultChain = st;
				}
				if (st.size() == 1){//���ջ��ֻʣ��ͷ�ڵ�
					st.pop();
				}
				else{
					ThisTail = st.top() + 1;
					st.pop();
					alphaMatrix[st.top()][ThisTail - 1] += 1;//ThisTail - 1
				}
			}
			else {//�ҵ��Ľڵ���ջ
				alphaMatrix[ThisHead][ThisTail] -= 1;
				st.push(ThisTail);
				ThisTail = 0;
			}
		}
	}
	void findLargest(char h, char t) {//Ѱ����ĸ��������
		if (h != 0) {
			findLargestH(h - 'a', t - 'a');
		}
		else {
			for (int i = 0; i < 26; i++) {
				findLargestH(i, t - 'a');
			}
		}
	}
	void findLargestH(int h, int t) {//����ͷѰ����ĸ��������
		//������Ҫ��¼��ջ�ĵ��ʵ���ĸ��������Ҫʹ�ýṹ�����ֻ������ĸ��int
		//ps:����ͨ��alphaMatrix��ʣ��������Ƴ����ʴ�С�����ض�wordSizeMatrix�������޸�Ҳ����Ҫʹ�ýṹ���¼���ʳ���
		//�˺���ʵ��Ӧ�޸�
		int sizeTemp;
		stack <TailandLength> st;
		TailandLength temp = { h, 0 };
		st.push(temp);
		TailandLength ThisHead;
		TailandLength ThisTail = {0, 0};
		while (!st.empty()) {
			ThisHead = st.top();
			while (ThisTail.tail != 26 && alphaMatrix[ThisHead.tail][ThisTail.tail] == 0) {
				ThisTail.tail++;
			}
			if (ThisTail.tail == 26) {
				//�˽ڵ㴦�����,��ջ,�ж��Ƿ���½��������
				if (currentLength > resultLength && ((t < 0) || (st.top().tail == t))) {
					resultChainL = st;
					resultLength = currentLength;
				}
				if (st.size() <= 1){
					st.pop();
				}
				else{
					ThisTail.tail = st.top().tail + 1;//׼����һ��Ҫ�ߵı�
					currentLength -= st.top().length;//��ǰ·Ȩ��ȥ��ջ�ı�Ȩ
					sizeTemp = st.top().length;
					st.pop();//��ջ
					wordSizeMatrix[st.top().tail][ThisTail.tail - 1].push_back(sizeTemp);//�����ջ�ı�Ȩ
					alphaMatrix[st.top().tail][ThisTail.tail - 1] += 1;//���˱߼ӻ�ȥ
				}
			}
			else {
				alphaMatrix[ThisHead.tail][ThisTail.tail] -= 1;//ȡ��һ����
				ThisTail.length = *(wordSizeMatrix[ThisHead.tail][ThisTail.tail].end() - 1);//�õ���Ȩ
				wordSizeMatrix[ThisHead.tail][ThisTail.tail].pop_back();//����Ȩ����������ɾ��
				currentLength += ThisTail.length;//������ǰ·����Ȩ
				st.push(ThisTail);//����ջ
				ThisTail.tail = 0;
				ThisTail.length = 0;
			}
		}
	}
	void findNum(int h, int t, int num){
		if (h != 0) {
			findNumH(h - 'a', t - 'a', num);
		}
		else {
			for (int i = 0; i < 26; i++) {
				findNumH(i, t - 'a', num);
			}
		}
	}
	void findNumH(int h, int t, int num){//ֻ����ʵ���㷨��δdebug
		stack <int> st;
		st.push(h);
		int ThisHead;
		int ThisTail = 0;
		while (!st.empty()) {
			ThisHead = st.top();
			while (alphaMatrix[ThisHead][ThisTail] == 0 && ThisTail != 26) {
				ThisTail++;
			}
			if (ThisTail == 26) {
				//�˽ڵ㴦�����,��ջ,������
				ThisTail = st.top() + 1;
				st.pop();
				alphaMatrix[st.top()][ThisTail - 1] += 1;
			}
			else {
				alphaMatrix[ThisHead][ThisTail] -= 1;
				st.push(ThisTail);
				if (st.size() - 1 == num){
					if (t < 0 || st.top() == t){
						resultChainN.push_back(st);
					}
					st.pop();
					alphaMatrix[ThisHead][ThisTail] += 1;
					ThisTail++;
				}
				else{
					ThisTail = 0;
				}
			}
		}
	}
	void getResult(Words &wd){
		stack <int> temp;
		vector <string> Result;
		int head, tail;
		while (!resultChain.empty()){
			temp.push(resultChain.top());
			resultChain.pop();
		}
		if (temp.size() < 2){
			error(0);
		}
		else{
			head = temp.top();
			temp.pop();
			while (!temp.empty()){
				tail = temp.top();
				Result.push_back(wd.getWord(head, tail, alphaMatrix[head][tail] - 1));
				alphaMatrix[head][tail] -= 1;
				head = tail;
				temp.pop();
			}
		}
		ofstream ofile("solution.txt");
		ofile << "The longest word chain has " << Result.size() << "word(s): " << endl;
		for (auto i : Result){
			ofile << i << " ";
		}
	}
	void getResultL(Words &wd){
		stack <int> temp;
		vector <string> Result;
		int head, tail;
		while (!resultChainL.empty()){
			temp.push(resultChainL.top().tail);
			resultChainL.pop();
		}
		if (temp.size() < 2){
			error(0);
		}
		else{
			head = temp.top();
			temp.pop();
			while (!temp.empty()){
				tail = temp.top();
				Result.push_back(wd.getWord(head, tail, alphaMatrix[head][tail] - 1));
				alphaMatrix[head][tail] -= 1;
				head = tail;
				temp.pop();
			}
		}
		ofstream ofile("solution.txt");
		ofile << "The largest word chain has " << Result.size() << "word(s): " << endl;
		for (auto i : Result){
			ofile << i << " ";
		}
	}
	void getResultN(Words &wd){
		//�����뷨�Ƕ�ÿ������������ĸ��������������
	}
public:
	wordChain(int argc, char *argv[]) {
		bool w = false, c = false, n = false;
		char h = 0, t = 0;
		int num = 0;
		string filePath(argv[0]);
		for (int i = 1; i < argc;) {
			if (argv[i][0] != '-') {
				error(0);
			}
			switch (argv[i][1]) {
			case 'w': 
				w = true;
				i++;
				break;
			case 'c': 
				c = true;
				i++;
				break;
			case 'n':
				n = true;
				num = atoi(argv[i + 1]);
				i += 2;
				break;
			case 'h':
				h = argv[i + 1][0];
				i += 2;
				break;
			case 't':
				t = argv[i + 1][0];
				i += 2;
				break;
			default:
				error(0);
			}
		}
		//������ϲ���������
		
		Words wd(filePath);
		wd.getAlphaMatrix(alphaMatrix);       //ע������ԭԤ�����ļ��еĺ���ʹ���������������и���
		wd.getWordSizeMatrix(wordSizeMatrix);
		wd.printWordMatrix();
		if (w) {
			findLongest(h, t);
			getResult(wd);
		}
		else if (c) {
			findLargest(h, t);
			getResultL(wd);
		}
		else {
			findNum(h, t, num);
		}
	}
};
int main(int argc, char *argv[]) {
	wordChain wc(argc, argv);
}
/*int main(){//������main����,ʹ��ʱӦע���޸�wordChain��ʼ�������Ĳ���
	char p[][20] = { "testfile.txt", "-w", "-t", "j"};
	wordChain wc(4, p);
}*/