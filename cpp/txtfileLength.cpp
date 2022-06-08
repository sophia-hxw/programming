#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <stdio.h>
#include <atlstr.h>//CString类必须包含的头文件

using namespace std;


int main ()
{
	////测试与exe在同一文件夹的example.txt文件的大小。
//    const char * filename = "example.txt";
//    long l,m;
//    ifstream file(filename, ios::in|ios::binary);
//    l = file.tellg();
//    file.seekg(0, ios::end);
//    m = file.tellg();
//    file.close();
//    cout <<"size of \" "<< filename;
//    cout <<" \" is "<< (m-l)<<" bytes.\n";

	////当前工作路径,当前模块路径
	///*GetCurrentDirectory返回当前进程的当前目录，并不一定返回你的exe应用程序的目录。
	//如果你在应用程序中调用了打开文件对话框，你选择了一个文件，那么，这个文件所在的
	//目录就成了当前进程的当前目录了。*/
	//char buf[1000];
	//int i = 1000;
	//GetCurrentDirectory(1000, buf);  //得到当前工作路径  
	//cout << buf << endl;
	//char strModule[256];
	//GetModuleFileName(NULL, strModule, 256); //得到当前模块路径  
	//cout << strModule << endl;
	//string a;
	//a.assign(buf);
	//cout << a << endl;
	//a.append("//..//");     //设置为当前工作路径为当时的上一级  
	////a=a+"..//";  
	//SetCurrentDirectory(a.c_str());  //设置  
	//GetCurrentDirectory(1000, buf);
	//cout << buf << endl;

//	string str1("sophia is a good girl!");//用常量字符串赋值。
//	string str2;
//	string str3;
//	string str4;
//	string str5;
//	str2.assign(str1);//直接用另一个字符串赋值.
//	str3.assign("sophia is a good girl!", 8);//用一个字符串的前一段子串赋值;
//	str4.assign(str1, 2, 10);//用另一个字符串的一个子串赋值
//	str5.assign(10, 'c');//用几个相同的字符,赋值.
//	cout << "str1=  " << str1 << endl;
//	cout << "str2=  " << str2 << endl;
//	cout << "str3=  " << str3 << endl;
//	cout << "str4=  " << str4 << endl;
//	cout << "str5=  " << str5 << endl;

	/*cin通过使用空白（空格、制表符和换行符）来定字符串的界。这意味着cin在读取
	字符数组输入时只读取一个单词，读取该单词后，cin将该字符串放一数组中，并自动
	结尾添加空字符。另外，cin也没有很好的控件输入的字符数，即输入字符数大于数组
	大小的情况没有处理。*/
	//const int size = 20;
	//char name[size];
	//char pl[size];//program language  
	//cout << "Enter your name:";
	//cin >> name;
	//cout << "Enter your favorite program language:";
	//cin >> pl;
	//cout << "Hello " << name << ", your favorite program language is " << pl << endl;
	/*cin.getline()函数读取整行，它使用通过回车键输入的换行符来确定输入结尾。该函数有
	两个参数。第一个参数是用来存储输入行的数组的名称，第二个参数是要读取的字符数（包括
	空字符），cin.getline()成员函数在读取指定数目的字符或遇到换行符时停止读取,需要注意
	的是，cin.getline()丢弃了换行符。*/
	/*cout << "Enter your name:";
	cin.getline(name, size);
	cout << "Enter your favorite program language:";
	cin.getline(pl, size);
	cout << "Hello " << name << ", your favorite program language is " << pl << endl;*/
	/*get()函数有好几种变体，其中有一种与getline()完全相同的参数，但该函数不再读取并丢
	弃换行符(不读取意味着换行符还在输入队列中)*/
	/*cout << "Enter your name:";
	cin.get(name, size);
	cout << "Enter your favorite program language:";
	cin.get(pl, size);
	cout << "Hello " << name << ", your favorite program language is " << pl << endl;*/
	/*发现并不好使，原因是由于第一次调用后，换行符将留存输入队列中，因此第二次调用时看
	到的第一个字符为换行符。因此get认为已经到达行尾，而没有发现任何可读取的内容。get()
	（不带任何参数的变体）可以读取下一个字符（包括换行符）。*/
	/*cout << "Enter your name:";
	cin.get(name, size).get();
	cout << "Enter your favorite program language:";
	cin.get(pl, size).get();
	cout << "Hello " << name << ", your favorite program language is " << pl << endl;*/
	/*混合输入字符串和数字*/
	/*int age = 0;
	char address[size] = { 0 };
	cout << "Enter your age:";
	cin >> age;
	cin.get();
	cout << "Enter your address:";
	cin.getline(address, size);
	cout << "your age is " << age << ", your address is " << address << endl;*/

	//c++中字符串数组和字符串之间的转化。
	//string str0 = "sophia is a good girl.";
	//const char *str1 = str0.c_str();//string.c_str()返回的是const char类型
	//char str2[100]="xinwen is also a good girl.";
	//string str3 = str2;
	//cout << "str0=  " << str0 << endl;
	//cout << "str1=  " << str1 << endl;
	//cout << "str2=  " << str2 << endl;
	//cout << "str3=  " << str3 << endl;
	/*CString类是MFC中重新定义的字符串类，提供了更多的字符串操作处理功能。*/
	//string->CString
	/*CString cstr;
	string str = "sophia is a good girl.";
	cstr = str.c_str();
	cout << "str=  " << str << endl;
	cout << "cstr=  " << cstr << endl;*/
	//CString->string
	/*CString str0 = "sophia is wonderful!";
	string str1 = str0.GetBuffer(0);
	cout << "str0=  " << str0 << endl;
	cout << "str1=  " << str1 << endl;*/
	
	//从键盘给EOF结束标志，结束程序。
	/*int c;
	do{
		printf("请输入文档的结尾标志");
	} while ((c = getchar()) != EOF);
	cout<<"已得到文档结束标志"<<endl;*/

	system("Pause");
    return 0;
}