# 计算英语单词链（Part2）

## 接口的实现

### 设计思路

在上一次的实验中，已经基本实现了类的定义与使用。

由于用户可能具有特殊的需求，因此需要定义新的接口以方便和其他模块之间交换数据。实验一中，我们已经定义了两个类：`wordChain`和`Words`，分别在`WordChain.cpp`和`Profix.cpp`中。其中`Words`类的主要功能实现了**文件的读取**、**字母矩阵、长度矩阵、单词矩阵的建立**、**单词总量的计算**、**重复单词的检测**、调试输出函数等多个**预处理**方法。

`wordChain`类通过调用`Words`类中实现的方法，实现了不同的用户需求，计算了不同的单词链：

* `findLongest`及下层方法`findLongestH`对应**计算单词最多单词链**需求。
* `findLargest`及下层方法`findLargestH`对应**计算字母最多单词链**需求。
* `findNum`及下层方法`findNumH`对应**计算出所有指定深度的单词链**，并在`getResultN`中递归输出。
* 以上三种不同的方法都支持**给定首尾字母计算单词链**的功能。

由于第一部分中，已经通过三个不同的方法来实现了不同的功能需求，因此只需要在`wordChain`类中新建方法，规定新的参数和接口，并在方法中分别调用上述三个方法，即可以实现实验需求。相当于为上述三个方法新建了不同的接口。

### 实现内容及参数说明

在上一次实验的代码的基础上，又进一步实现了两种接口，用于满足不同的用户需求：

#### 第一类接口：

```C++
wordChain(vector<string> inputString, int len, char h, char t, char type)
```

第一种接口对应于计算**最多单词数量**和计算**最多字母数量**两种要求，即`-w`和`-c`两种需求，且支持**指定首尾字母**的输入方法。

**参数说明如下：**

- `vector<string> inputString`：输入的单词数组（通过C++`vector`来实现）
- `int len`：输入的单词个数，即`inputString`的长度
- `char h`：指定的单词链开始字母，缺省情况下ASCII码为0。
- `char t`：指定的单词链结束字母，缺省情况下ASCII码为0。
- `char type`：用于区分计算**最多单词数量**和计算**最多字母数量**两种要求：
  - 当`type=='W'`时，执行**最多单词数量单词链**的计算。
  - 当`type=='C'`时，执行**最多字母数量单词链**的计算。
  - 当type为其他情况时，报错，因为此时的情况与需求不相符合，即合法的type只能取`w`或者`c`。

**具体实现如下**：

```C++
wordChain(vector<string> inputString, int len, char h, char t, char type)
{
    if (inputString.size() == 0){
		cout << "error : no string detected!" << endl;
		return;
	}
	for (auto str : inputString){
		if (str.size() == 0){
			cout << "error : an empty string!" << endl;
			return;
		}
		for (auto p : str){
		if (!isalpha(p)){
				cout << "error : illegal character!" << endl;
				return ;
			}
		}
	}
	W_or_C_Type = true;
	N_Type = false;
	Words wd(inputString, len);
	wd.getAlphaMatrix(alphaMatrix);
	for (int m = 0; m<26; m++)
	{
		for (int n = 0; n<26; n++)
			copied_alpha[m][n] = alphaMatrix[m][n];
	}
    /*******以下部分调用两个下层方法分别实现w功能和c功能*****/
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
		cout << "parameter error!" << endl;
	}
}
```



#### 第二类接口：

```C++
wordChain(vector<string> inputString, int len, int num, char h, char t, char type)
```

第二类接口实现了计算**指定单词链长度的单词链数量及内容**的需求，即`-n`类需求，同样支持**指定首尾字母**的输入方法。

**参数说明如下**：

- `vector<string> inputString`：输入的单词数组（通过C++`vector`来实现）
- `int len`：输入的单词个数，即`inputString`的长度
- `int num`：指定的单词链的长度。
- `char h`：指定的单词链开始字母，缺省情况下ASCII码为0。
- `char t`：指定的单词链结束字母，缺省情况下ASCII码为0。
- `char type`：用于保持重载函数的参数相同，无实际意义，但处于健壮性和易读性应当设置为`'n'`，如果type为`'c'、'w'及其他非法type类型`则会产生报错。

**具体实现如下**：

由于实验一中已经基本实现了相关的按照模块的划分不同功能的需求，因此这一部分本质上相当于是为初始的`findNUM`函数重新定义了接口，所以该函数的主要实现的功能类似于`findNum`函数。

```C++
wordChain(vector<string> inputString, int len, int num, char h, char t, char type){
    //错误处理，包含非法字符，向量为空，向量中出现空串
	if (inputString.size() == 0){
		cout << "error : no string detected!" << endl;
		return;
	}
	for (auto str : inputString){
		if (str.size() == 0){
			cout << "error : an empty string!" << endl;
			return;
		}
		for (auto p : str){
			if (!isalpha(p)){
				cout << "error : illegal character!" << endl;
				return;
			}
		}
	}
	W_or_C_Type = false；
	N_Type = true;
	Words wd(inputString, len);
	wd.getAlphaMatrix(alphaMatrix);
	for (int m = 0; m<26; m++)
	{
		for (int n = 0; n<26; n++)
			copied_alpha[m][n] = alphaMatrix[m][n];
	}
	wd.getWordSizeMatrix(wordSizeMatrix);
	num_count = 0;
	findNum(wd, h, t, num);		//调用函数寻找
	ofstream ofile("solution.txt");
	ifstream infile("solution_temp.txt");
    //此处temp文件中保存的是递归时输出的所有单词链，由于递归中无法得到总单词链个数
    //因此采用临时文件保存所有单词链，以便按照格式要求输出
	ofile << num_count << endl;
	ofile << "" << endl;
	string line;
	if (infile) 
	{
		while (getline(infile, line)) // line中不包括每行的换行符
			ofile << line << endl;
	}
	else 
		cout << "no such file" << endl;
	ofile.close();
	infile.close();
	remove("solution_temp.txt");
	cout << num_count << endl;
}
```

#### 原始接口：

此外，仍然保留了第一部分中实现的`wordChain`方法：

```C++
wordChain(int argc, char *argv[])
```

读入命令行的参数，实现从读取文件到计算单词链的完整功能。即实验第一部分实现的命令行读取参数，也可以用于GUI的输入方式。

#### 其他接口：

本次实验中除了提供了计算的接口，同时也提供了输出结果的结果，即读取计算结果并输出到界面给用户的功能。

考虑到用户会有其他方面的需求，如在求指定深度的单词链的时候，用户可能想直接得到单词链的总数，而不关心具体的单词链；用户也有可能关注最长单词链的单词数量，而不关心具体由哪些单词组成。

由于上述两类需求可能彼此独立，因此不适合将两者放在同一个方法中，应当分别用不同方法实现。

综合考虑之下，额外实现了四个接口：

**输出类**

1. `vector<string> getChain()`
   * 用于在**最多单词数单词链**和**最多字母数单词链的情况下**输出结果，返回的vector容器中包括单词链。
   * 在**输出指定长度的单词链**的情况下调用会产生报错。
2. `vector<vector<string>>getChains()`
   - 用于在**最多单词数单词链**和**最多字母数单词链的情况下**输出结果，返回的vector容器中包括所有符合要求的单词链。
   - 在其他情况下调用会产生报错。

**长度、数量类**

1. `int getNum()`

   该接口用于在**输出指定长度的单词链**的情况下，输出所有合法单词链的数目。

   * 从-n条件下的输出文件中读取出对应的数值。
   * 如果在非-n条件下调用接口，则会产生报错，保证了程序的健壮性。

2. `int getLen()`

   用于在**最多单词数单词链**和**最多字母数单词链的情况下**输出单词链的单词数。

   - 从-c，-w条件下的类中读取出对应的数值。
   - 如果在-n条件下调用接口，则会产生报错。

## 单元测试

# 1551 冉哥哥你快点