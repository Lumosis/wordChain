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
};//保存当前节点的尾和对应单词的长度

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
	void findLongest(char h, char t) {//找出含有最多单词数的链
		if (h != 0) {
			findLongestH(h - 'a', t - 'a');
		}
		else {//如果没有h参数，则将每个字母作为头都运行一遍
			for (int i = 0; i < 26; i++) {
				findLongestH(i, t - 'a');
			}
		}
	}
	void findLongestH(int h, int t) {//规定单词链的头寻找目标链
		//保持栈顶为当时正在处理的单词的头
		stack <int> st;
		st.push(h);
		int ThisHead;
		int ThisTail = 0;
		while (!st.empty()) {
			ThisHead = st.top();
			while (ThisTail != 26 && alphaMatrix[ThisHead][ThisTail] == 0) {//寻找下个节点
				ThisTail++;
			}
			if (ThisTail == 26) {
				//找不到下一个节点，则此节点处理完毕,出栈,判断是否更新结果并返回
				if (st.size() > resultChain.size() && ((t < 0) || (st.top() == t))) {
					resultChain = st;
				}
				if (st.size() == 1){//如果栈中只剩下头节点
					st.pop();
				}
				else{
					ThisTail = st.top() + 1;
					st.pop();
					alphaMatrix[st.top()][ThisTail - 1] += 1;//ThisTail - 1
				}
			}
			else {//找到的节点入栈
				alphaMatrix[ThisHead][ThisTail] -= 1;
				st.push(ThisTail);
				ThisTail = 0;
			}
		}
	}
	void findLargest(char h, char t) {//寻找字母数最多的链
		if (h != 0) {
			findLargestH(h - 'a', t - 'a');
		}
		else {
			for (int i = 0; i < 26; i++) {
				findLargestH(i, t - 'a');
			}
		}
	}
	void findLargestH(int h, int t) {//给定头寻找字母数最多的链
		//由于需要记录出栈的单词的字母数所以需要使用结构体代替只保存字母的int
		//ps:可以通过alphaMatrix中剩余边数量推出单词大小而不必对wordSizeMatrix矩阵做修改也不需要使用结构体记录单词长度
		//此函数实现应修改
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
				//此节点处理完毕,出栈,判断是否更新结果并返回
				if (currentLength > resultLength && ((t < 0) || (st.top().tail == t))) {
					resultChainL = st;
					resultLength = currentLength;
				}
				if (st.size() <= 1){
					st.pop();
				}
				else{
					ThisTail.tail = st.top().tail + 1;//准备下一个要走的边
					currentLength -= st.top().length;//当前路权减去出栈的边权
					sizeTemp = st.top().length;
					st.pop();//出栈
					wordSizeMatrix[st.top().tail][ThisTail.tail - 1].push_back(sizeTemp);//回填出栈的边权
					alphaMatrix[st.top().tail][ThisTail.tail - 1] += 1;//将此边加回去
				}
			}
			else {
				alphaMatrix[ThisHead.tail][ThisTail.tail] -= 1;//取出一条边
				ThisTail.length = *(wordSizeMatrix[ThisHead.tail][ThisTail.tail].end() - 1);//得到边权
				wordSizeMatrix[ThisHead.tail][ThisTail.tail].pop_back();//将边权从向量表中删除
				currentLength += ThisTail.length;//调整当前路径边权
				st.push(ThisTail);//边入栈
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
	void findNumH(int h, int t, int num){//只初步实现算法还未debug
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
				//此节点处理完毕,出栈,并返回
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
		//初步想法是对每个搜索到的字母串都单独建表处理
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
		//这里加上参数错误处理
		
		Words wd(filePath);
		wd.getAlphaMatrix(alphaMatrix);       //注：更改原预处理文件中的函数使得其对输入数组进行更改
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
/*int main(){//测试用main函数,使用时应注意修改wordChain初始化函数的参数
	char p[][20] = { "testfile.txt", "-w", "-t", "j"};
	wordChain wc(4, p);
}*/