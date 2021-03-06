﻿// WordChain.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#include <iostream>
#include <string>
#include<string.h>
#include <vector>
#include <fstream>
#include <stack>
#include "Profix.cpp"  //这里使用了profix中定义的类
#include <sstream>
using namespace std;

extern "C"{

    int test(int, char *[]);

}

struct TailandLength {
	int tail;
	int length;
};//保存当前节点的尾和对应单词的长度

class wordChain {
private:
	bool W_or_C_Type, N_Type;
	int alphaMatrix[26][26];
	int copied_alpha[26][26];//用于保存副本，因为在递归的时候，尽管出了结果，但是计数矩阵没有被完全复原
	vector <int> wordSizeMatrix[26][26];
	stack <int> resultChain;  //保存的是按照单词最多取的单词链
	stack <TailandLength> resultChainL;	//按照字符最多取的单词链
	vector <stack <int>> resultChainN;  //这里存放的是所有满足条件的链
	vector <string> Result;
	//vector <vector<string>> num_result;
	int currentLength;
	int resultLength;
	int num_count;
	void error(int errorID) {
		switch (errorID){
		case 1:
			cout << "参数错误" << endl;
			break;
		case 2:
			cout << "输入文件为空" << endl;
			break;
		case 3:
			cout << "文件不存在" << endl;
			break;
		case 4:
			cout << "没有单词存在" << endl;
			break;
		case 5:
			cout << "出现空单词" << endl;
			break;
		case 6:
			cout << "出现非法字符" << endl;
			break;
		case 7:
			cout << "递归深度有误" << endl;
			break;
		case 8:
			cout << "单词链长度必须为正整数" << endl;
		case 9:
			cout << "单词链开始/结束字母不符合要求，请输入单个字母" << endl;
			break;
		case 10:
			cout << "单词链开始/结束字母非法" << endl;
			break;
		case 11:
			cout << "单词链过长，超过总单词数" << endl;
			break;
		default:;
		}
		exit(errorID);
	}
	void printmatrix()
	{
		for (int m=0;m<26;m++)
		{
			for (int n = 0; n < 26; n++)
				cout << alphaMatrix[m][n] << ","  ;
			cout<< endl;
		}
		cout<<"-------"<<endl;
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
			ThisHead = st.top();  //从栈顶取出元素，栈顶元素就是上一个单词的尾部字母，也是当前单词的头部字母
			while (ThisTail != 26 && alphaMatrix[ThisHead][ThisTail] == 0) {//按照顺序寻找下个节点
				ThisTail++; //按照a-z的顺序找能够连接上的下一个节点
			}
			if (ThisTail == 26) {
				//找不到下一个节点，则此节点处理完毕,出栈,判断是否更新结果并返回
				if (st.size() > resultChain.size() && ((t < 0) || (st.top() == t))) {
					//如果此时栈中的数目，也就是单词的个数+1，大于之前找到的最长单词链，则更新数据
					//并且此时的结尾元素等于t或者未规定有效的t
					resultChain = st;
				}
				if (st.size() == 1) {//如果栈中只剩下头节点，表示此时单词链长度为0
					st.pop();
				}
				else {//如果说大小不为1，则递归寻找下一条路径
					ThisTail = st.top() + 1;//找下一种结尾
					st.pop();
					alphaMatrix[st.top()][ThisTail - 1] += 1;//ThisTail - 1
				}
			}
			else {//找到的节点入栈
				alphaMatrix[ThisHead][ThisTail] -= 1;
				st.push(ThisTail);	//栈中保存的是所有单词的尾部元素（也是下一个单词的开始元素）
				ThisTail = 0;		//然后尾部元素清零
			}
		}
	}
	void findLargest(char h, char t) {//寻找字母数最多的链
		if (h != 0) {   //如果了指定头部
			findLargestH(h - 'a', t - 'a');
		}
		else {			//如果没有指定头部，全部扫描一遍
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
		TailandLength ThisTail = { 0, 0 };
		while (!st.empty()) {
			ThisHead = st.top();
			while (ThisTail.tail != 26 && alphaMatrix[ThisHead.tail][ThisTail.tail] == 0) {
				ThisTail.tail++;
			}
			if (ThisTail.tail == 26) {//表明已经走到头了
				//此节点处理完毕,出栈,判断是否更新结果并返回
				if (currentLength > resultLength && ((t < 0) || (st.top().tail == t))) {
					resultChainL = st;//保存新的最长单词栈
					resultLength = currentLength;//保存最长单词数
				}
				if (st.size() <= 1) {//如果栈中没有一个完整的单词链
					st.pop();
				}
				else {
					ThisTail.tail = st.top().tail + 1;//准备下一个要走的边
					currentLength -= st.top().length;//当前路权减去出栈的边权
					sizeTemp = st.top().length; //取出当前栈顶的元素
					st.pop();//出栈
					wordSizeMatrix[st.top().tail][ThisTail.tail - 1].push_back(sizeTemp);//回填出栈的边权
					alphaMatrix[st.top().tail][ThisTail.tail - 1] += 1;//将此边加回去，因为之后可能还会走到这条边
				}
			}
			else {
				alphaMatrix[ThisHead.tail][ThisTail.tail] -= 1;//取出一条边
				ThisTail.length = *(wordSizeMatrix[ThisHead.tail][ThisTail.tail].end() - 1);//得到边权
				wordSizeMatrix[ThisHead.tail][ThisTail.tail].pop_back();//将边权从向量表中删除
				currentLength += ThisTail.length;//调整当前路径边权
				st.push(ThisTail);//边入栈
				ThisTail.tail = 0;//清零
				ThisTail.length = 0;
			}
		}
	}
	void getResultN(Words &wd,stack <int> result,vector <string> answer) {
		//初步想法是对每个搜索到的字母串都单独建表处理，相当于是搜索到一个就直接调用此函数print
		//由于要输出所有符合要求的，所以这里还需要认真考虑所有首尾相同的可能性。并且可能重复出现而且位于不同位置
		//如word和world可以出现在单词链中的不同位置
		//从而造成截然不同的两种单词链

		//现在在最终结果的输出上有了问题

		if (result.size()==1) //此时栈中只剩下一个元素，无法构成单词
		{
			//num_result.push_back(answer);
			ofstream ofile;
			ofile.open("solution_temp.txt",ios::app);
			for (int m=0;m < answer.size();m++)
			{
				cout<<answer[m]<<endl;
				ofile << answer[m] << endl;
			}
			ofile << ""<< endl;
			cout << "" << endl;
			num_count++;
			ofile.close();
			return;
		}
		//接下来，逐层检查每一种的可能性，并且将可能性累加。
		int Head = result.top();
		result.pop();
		int Tail = result.top();
		for (int i=0; i<copied_alpha[Head][Tail];i++)
		{
			vector <string> tempvec = answer;
			//cout<<wd.getWord(Head,Tail,copied_alpha[Head][Tail]-1-i)<<endl;
			string str = wd.deleteword(Head,Tail,copied_alpha[Head][Tail]-1-i);
			copied_alpha[Head][Tail]--;
			tempvec.push_back(str);
			getResultN(wd,result,tempvec);
			wd.insertword(Head,Tail,copied_alpha[Head][Tail]-i,str);
			copied_alpha[Head][Tail]++;
		}

	}
	void findNum(Words &wd, int h, int t, int num) {
		if (h != 0) {
			findNumH(wd, h - 'a', t - 'a', num);
		}
		else {
			for (int i = 0; i < 26; i++) {
				findNumH(wd, i, t - 'a', num);
			}
		}
	}
	void findNumH(Words &wd, int h, int t, int num) {//似乎是实现完全了的……
	//重点和难点在于找到不难，但是基于当前的图很难将所有头尾相同的可选元素打印出来。
	//该函数递归的找到了所有符合要求的字符栈序列。
		//cout<<num<<endl;
		stack <int> st;
		st.push(h);
		int ThisHead;
		int ThisTail = 0;
		while (!st.empty()) {
			//cout<<"qqqq"<<endl;
			ThisHead = st.top();
			while (alphaMatrix[ThisHead][ThisTail] == 0 && ThisTail != 26) {
				ThisTail++;
			}
			if (ThisTail == 26) {
				//此节点处理完毕,出栈,并返回
				ThisTail = st.top() + 1;
				st.pop();
				if (!st.empty())
					alphaMatrix[st.top()][ThisTail - 1] += 1;
			}
			else {//如果当前可以扩展
				alphaMatrix[ThisHead][ThisTail] -= 1;
				st.push(ThisTail);
				//cout<< ThisHead<< " " << ThisTail<<"  "<< st.size()<<endl;
				if (st.size() - 1 == num) {//如果栈的深度已经达到目标要求
					if (t < 0 || st.top() == t) 
					{
						stack <int> tempst = st;
						stack <int> reversed_result;	//result中的是反序，这里要把数据正过来放
						while(!tempst.empty())
						{
							reversed_result.push(tempst.top());
							tempst.pop();
						}
						//resultChainN.push_back(reversed_result);
						vector <string> answer;
						getResultN(wd,reversed_result,answer);
					}
					st.pop();
					alphaMatrix[ThisHead][ThisTail] += 1;
					ThisTail++;
				}
				else {//否则，继续递归
					ThisTail = 0;
				}
			}
		}
		//printmatrix();
	}
	void PrintResultn(Words &wd)
	{
		for (int i = 0; i < resultChainN.size(); i++)
		{
			vector<string> answer;
			getResultN(wd,resultChainN[i],answer);
		}			
	}
	void getResult(Words &wd) {
		stack <int> temp;
		int head, tail;
		Result.clear();
		while (!resultChain.empty()) {
			temp.push(resultChain.top());
			resultChain.pop();
		}
		if (temp.size() < 2) {
			error(0);
		}
		else {
			head = temp.top();
			temp.pop();
			while (!temp.empty()) {
				tail = temp.top();
				Result.push_back(wd.getWord(head, tail, alphaMatrix[head][tail] - 1));
				alphaMatrix[head][tail] -= 1;
				head = tail;
				temp.pop();
			}
		}
		ofstream ofile("solution.txt");
		ofile << "The longest word chain has " << Result.size() << "word(s): " << endl;
		for (auto i : Result) {
			ofile << i << " ";
		}
		ofile.close();
	}
	void getResultL(Words &wd) {
		stack <int> temp;
		int head, tail;
		Result.clear();
		while (!resultChainL.empty()) {
			temp.push(resultChainL.top().tail);
			resultChainL.pop();
		}
		if (temp.size() < 2) {
			error(0);
		}
		else {
			head = temp.top();
			temp.pop();
			while (!temp.empty()) {
				tail = temp.top();
				Result.push_back(wd.getWord(head, tail, alphaMatrix[head][tail] - 1));
				alphaMatrix[head][tail] -= 1;
				head = tail;
				temp.pop();
			}
		}
		ofstream ofile("solution.txt");
		ofile << "The largest word chain has " << Result.size() << "word(s): " << endl;
		for (auto i : Result) {
			ofile << i << " ";
		}
		ofile.close();
	}
public:
	wordChain(){

	}
	wordChain(int argc, char *argv[]) {    //读入不同的参数，并且调用对应的函数
		bool w = false, c = false, n = false;
		bool error_flag = false;
		bool file_error_flag = false;
		char h = 0, t = 0;
		int num = 0;
		int len;
		int total_word_num = 0;
		string filePath(argv[argc-1]);   //最后一个参数表示文件路径
		for (int i = 1; i < argc-1;) {
			if (argv[i][0] != '-') { //如果第一个字符不是'-' 出错
				error(0);
			}
			switch (argv[i][1]) {  //根据参数的不同来进行选择
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
				//这里还需要检查下面的字符串中是否全部都是数字
				len = string(argv[i + 1]).size();
				for (int k = 0; k < len; k++)
				{
					if (k == 0 && argv[i + 1][k] == '+') continue; //该情况合法
					if (argv[i + 1][k] > '9' || argv[i + 1][k] < '0')
					{
						error_flag = true;
						error(7);
					}
				}
				if (error_flag == false)
				{
					num = atoi(argv[i + 1]);  //得到递归深度
					if (num <= 0) //虽然估计没人这么作死但是还是写一下吧
					{
						error_flag = true;
						error(8);
					}
				}
				i += 2;
				break;
			case 'h':				//这里还需要检查一下长度的问题，如果说后面跟着的单词数目超过两个，则是非法表示
				//cout << argv[i+1] << "/   /" <<  string(argv[i+1]).size()<< endl;
				if (string(argv[i + 1]).size() > 1)
				{
					error_flag = true;
					error(9);
				}
				if (!isalpha(argv[i + 1][0]))
				{
					error_flag = true;
					error(10);
				}
				h = argv[i + 1][0];       //得到头部字符
				i += 2;
				break;
			case 't':
				//cout << argv[i+1] << "   " << string(argv[i + 1]).size() << endl;
				if (string(argv[i + 1]).size() > 1)
				{
					error_flag = true;
					error(9);
				}
				if (!isalpha(argv[i + 1][0]))
				{
					error_flag = true;
					error(10);
				}
				t = argv[i + 1][0];       //得到尾部字符
				i += 2;
				break;
			default:
				error_flag = true;
				error(0);
			}
		}
		//这里加上参数错误处理
		//cout << n << "  "<< num << endl;

		/***在导入filePath之前，先检验该文件是否存在以及是否为空****/
		ifstream check_file(filePath);
		if (check_file)
		{
			char c;
			check_file >> c;
			if (check_file.eof())
			{
				file_error_flag = true;
				error(2);
			}
		}
		else
		{
			file_error_flag = true;
			error(3);
		}
		/********文件检验完成*******/
		if (file_error_flag == false)
		{
			Words wd(filePath);						//新建一个Words类，读取文件中的数据并建立起Words类中的内容
			wd.getAlphaMatrix(alphaMatrix);          //注：更改原预处理文件中的函数使得其对输入数组进行更改
			for (int m=0; m<26;m++)
			{
				for (int n=0;n<26;n++)
					copied_alpha[m][n] = alphaMatrix[m][n];
			}
			wd.getWordSizeMatrix(wordSizeMatrix);
			//wd.printWordMatrix();

			/********检验各种参数之间的冲突性********/
			total_word_num = wd.get_word_num();
			//cout<< total_word_num << endl;
			if (total_word_num == 0)
			{
				error_flag = true;
				error(4);
			}
			if (n &&(w|c)||(w&&c))  //最后一个需求和前两个冲突，或者前两个需求冲突
			{
				if (w == true)
				{
					cout << "命令出现冲突，以w(最多单词数)为准"<< endl;
					n = false;
					c = false;
				}
				else
				{
					cout << "命令出现冲突，以c(最多字母数)为准"<< endl;
					n = false;
				}
			}
			if (n && (total_word_num < num))
			{
				error_flag = true;
				error(11);
			}
			/*******参数检验完成*******/
			//printmatrix();
			/**********如果之前没有错误，开始DFS*************/
			if (error_flag == false)	//这里的优先级是w，c，n，因此上面进行了对应的说明
			{
				if (w) {					//如果w不为空，则找最多单词数量的单词链
					W_or_C_Type = true;
					N_Type = false;
					findLongest(h, t);
					getResult(wd);
				}
				else if (c) {              //找字母最多的单词链
					W_or_C_Type = true;
					N_Type = false;
					findLargest(h, t);
					getResultL(wd);
				}
				else {
					W_or_C_Type = false;
					N_Type = true;
					num_count = 0;
					findNum(wd,h, t, num);		//找的是num数据中的单词链
					//PrintResultn(wd);
					cout<<num_count<<endl;
					ofstream ofile("solution.txt");
					ifstream infile("solution_temp.txt");
					ofile << num_count<<endl;
					ofile << "" <<endl;
					string line;
					if(infile) // 有该文件
					{
						while (getline (infile, line)) // line中不包括每行的换行符
						ofile << line << endl;
					}
					else // 没有该文件
						cout <<"no such file" << endl;
					ofile.close();
					infile.close();
					remove("solution_temp.txt");
					//这里被注释掉的是另一种方法，将所有结果保存起来之后统一输出，但是担心复杂度爆炸因此使用了上面的迂回策略
					/*for (int i =0; i< num_result.size();i++)
					{
						for (int j=0; j< num_result[i].size();j++)
							cout<<num_result[i][j]<<endl;
						cout<<endl;
					}*/
				}
				cout << "" << endl;
				cout << "程序执行完成，结果输出在solution.txt文件中，请到对应目录下查看"<<endl;
			}
			else
			{
				cout << "" << endl;
				cout << "程序未能正常执行，请查看错误信息后修正输入"<<endl; 
			}
		}
	}
	wordChain(vector<string> inputString, int len, char h, char t, char type){//测试所用函数接口
		//错误处理，包含非法字符，向量为空，向量中出现空串
		if (inputString.size() == 0){
			error(4);
			return;
		}
		for (auto str : inputString){
			if (str.size() == 0){
				error(5);
				return;
			}
			for (auto p : str){
				if (!isalpha(p)){
					error(6);
					return ;
				}
			}
		}
		//错误处理，处理头尾要求字符非法的情况
		if ((h && !isalpha(h)) || (t && !isalpha(t))){
			error(10);
		}
		//设置此操作类型
		W_or_C_Type = true;
		N_Type = false;
		//预处理得到矩阵
		Words wd(inputString, len);
		wd.getAlphaMatrix(alphaMatrix);
		for (int m = 0; m<26; m++)
		{
			for (int n = 0; n<26; n++)
				copied_alpha[m][n] = alphaMatrix[m][n];
		}
		//计算最长链
		wd.getWordSizeMatrix(wordSizeMatrix);
		if (type == 'w'){
			findLongest(h, t);
			getResult(wd);
		}
		else if (type == 'c'){
			findLargest(h, t);
			getResultL(wd);
		}
		else{
			error(1);
		}
	}
	wordChain(vector<string> inputString, int len, int num, char h, char t, char type){
		//错误处理，包含非法字符，向量为空，向量中出现空串
		if (inputString.size() == 0){
			error(4);
			return;
		}
		for (auto str : inputString){
			if (str.size() == 0){
				error(5);
				return;
			}
			for (auto p : str){
				if (!isalpha(p)){
					error(6);
					return;
				}
			}
		}
		//错误处理，处理头尾要求字符非法的情况
		if ((h && !isalpha(h)) || (t && !isalpha(t))){
			error(10);
		}
		//设置此操作类型
		W_or_C_Type = false;
		N_Type = true;
		//预处理得到矩阵
		Words wd(inputString, len);
		wd.getAlphaMatrix(alphaMatrix);
		for (int m = 0; m<26; m++)
		{
			for (int n = 0; n<26; n++)
				copied_alpha[m][n] = alphaMatrix[m][n];
		}
		wd.getWordSizeMatrix(wordSizeMatrix);
		num_count = 0;
		//计算定长单词链
		findNum(wd, h, t, num);		//找的是num数据中的单词链
		//输出结果
		ofstream ofile("solution.txt");
		ifstream infile("solution_temp.txt");
		ofile << num_count << endl;
		ofile << "" << endl;
		string line;
		if (infile) // 有该文件
		{
			while (getline(infile, line)) // line中不包括每行的换行符
				ofile << line << endl;
		}
		else // 没有该文件
			error(3);
		ofile.close();
		infile.close();
		remove("solution_temp.txt");
		//PrintResultn(wd);
		cout << num_count << endl;
	}
	vector<string> textPreprocess(string inputFilePath){
		bool file_error_flag = false;
		ifstream check_file(inputFilePath);
		if (check_file)
		{
			char c;
			check_file >> c;
			if (check_file.eof())
			{
				file_error_flag = true;
				error(2);
			}
		}
		else
		{
			error(3);
			file_error_flag = true;
			error(0);
		}
		Words wd(inputFilePath);
		return wd.getWordChain();
	}
	void genChain(vector<string> inputString, char h, char t, char type){//测试所用函数接口
		//错误处理，包含非法字符，向量为空，向量中出现空串
		if (inputString.size() == 0){
			error(4);
			return;
		}
		for (auto str : inputString){
			if (str.size() == 0){
				error(5);
				return;
			}
			for (auto p : str){
				if (!isalpha(p)){
					error(6);
					return;
				}
			}
		}
		//错误处理，处理头尾要求字符非法的情况
		if ((h && !isalpha(h)) || (t && !isalpha(t))){
			error(10);
		}
		//设置此操作类型
		W_or_C_Type = true;
		N_Type = false;
		//预处理得到矩阵
		Words wd(inputString, inputString.size());
		wd.getAlphaMatrix(alphaMatrix);
		for (int m = 0; m<26; m++)
		{
			for (int n = 0; n<26; n++)
				copied_alpha[m][n] = alphaMatrix[m][n];
		}
		//计算最长链
		wd.getWordSizeMatrix(wordSizeMatrix);
		if (type == 'w'){
			findLongest(h, t);
			getResult(wd);
		}
		else if (type == 'c'){
			findLargest(h, t);
			getResultL(wd);
		}
		else{
			error(1);
		}
	}
	void genChainNum(vector<string> inputString, int num, char h, char t, char type){
		//错误处理，包含非法字符，向量为空，向量中出现空串
		if (inputString.size() == 0){
			error(4);
			return;
		}
		for (auto str : inputString){
			if (str.size() == 0){
				error(5);
				return;
			}
			for (auto p : str){
				if (!isalpha(p)){
					error(6);
					return;
				}
			}
		}
		//错误处理，处理头尾要求字符非法的情况
		if ((h && !isalpha(h)) || (t && !isalpha(t))){
			error(10);
		}
		//设置此操作类型
		W_or_C_Type = false;
		N_Type = true;
		//预处理得到矩阵
		Words wd(inputString, inputString.size());
		wd.getAlphaMatrix(alphaMatrix);
		for (int m = 0; m<26; m++)
		{
			for (int n = 0; n<26; n++)
				copied_alpha[m][n] = alphaMatrix[m][n];
		}
		wd.getWordSizeMatrix(wordSizeMatrix);
		num_count = 0;
		//计算定长单词链
		findNum(wd, h, t, num);		//找的是num数据中的单词链
		//输出结果
		ofstream ofile("solution.txt");
		ifstream infile("solution_temp.txt");
		ofile << num_count << endl;
		ofile << "" << endl;
		string line;
		if (infile) // 有该文件
		{
			while (getline(infile, line)) // line中不包括每行的换行符
				ofile << line << endl;
		}
		else // 没有该文件
			error(3);
		ofile.close();
		infile.close();
		remove("solution_temp.txt");
		//PrintResultn(wd);
		cout << num_count << endl;
	}
	vector<string> getChain(){
		if (!W_or_C_Type){
			error(1);
			return{};
		}
		return Result;
	}
	int getLen(){
		if (!W_or_C_Type){
			error(1);
			return 0;
		}
		return Result.size();
	}
	vector<vector<string>>getChains(){
		if (!N_Type){
			error(1);
			return{};
		}
		ifstream infile("solution.txt");
		vector<vector<string>> ResultN;
		int num;
		string line = "";
		infile >> num;
		getline(infile, line);
		getline(infile, line);
		ResultN.push_back({});
		while (getline(infile, line) && num > 0){
			if (line == ""){
				ResultN.push_back({});
				num--;
				continue;
			}
			auto iter = ResultN.end() - 1;
			(*iter).push_back(line);
		}
		return ResultN;
	}
	int getNum(){
		if (!N_Type){
			error(1);
			return 0;
		}
		ifstream infile("solution.txt");
		int num;
		infile >> num;
		return num;
	}
};

/*int test(int argc, char *argv[]) {
	//cout << argv[0] << endl;
	cout << argc << endl;
	cout << *argv << endl;

	wordChain wc(argc, argv);
}*/

int main(){
	vector<string> inputString = { "ABC", "CDE", "EDAOF", "FEIKFOSAEI", "IAWRUGH" };
	/**wordChain wc(inputString, 5, 2, 0, 0, 'n');
	auto p = wc.getChains();
	for (auto i : p){
		cout << endl;
		for (auto m : i){
			cout << m << " ";
		}
	}**/
	wordChain wc(inputString, 5, 0, 0, 'w');
	auto p = wc.getChain();
	for (auto i : p){
		cout << i << " ";
	}
	system("pause");
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

/**************
使用方法:
初始化函数有四种：
wordChain()  不传入任何参数，将创建一个空的对象
wordChain(int argc, char *argv[])    将参数以命令行参数的形式直接传入初始化函数，并计算得到相应结果
wordChain(vector<string> inputString, int len, char h, char t, char type)   针对寻找最长单词链和最多字母单词链，参数type为操作类型
wordChain(vector<string> inputString, int len, int num, char h, char t, char type) 针对寻找定长单词链

*****若使用后两种方法进行初始化需调用者自己保证传入的inputstring中单词均为合法的大写单词*****
vector<string> getChain() 得到w或c参数下的结果
int getLen() 得到w或者c参数下的结果长度
vector<vector<string>>getChains() 得到n参数下的结果
int getNum() 得到n参数下的单词链数量

*****若使用第一种方法创建一个空的对象，若需要对原始文本进行分词处理则需要先调用
textPreprocess(string inputFilePath)来获得一个处理过的只有不重复单词的字符串向量
再调用genChain() 对应w, c 要求或genChainNum()对应n要求。
最后调用相应函数获得结果

***************/