#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <stdio.h>
#include <atlstr.h>//CString����������ͷ�ļ�

using namespace std;


int main ()
{
	////������exe��ͬһ�ļ��е�example.txt�ļ��Ĵ�С��
//    const char * filename = "example.txt";
//    long l,m;
//    ifstream file(filename, ios::in|ios::binary);
//    l = file.tellg();
//    file.seekg(0, ios::end);
//    m = file.tellg();
//    file.close();
//    cout <<"size of \" "<< filename;
//    cout <<" \" is "<< (m-l)<<" bytes.\n";

	////��ǰ����·��,��ǰģ��·��
	///*GetCurrentDirectory���ص�ǰ���̵ĵ�ǰĿ¼������һ���������exeӦ�ó����Ŀ¼��
	//�������Ӧ�ó����е����˴��ļ��Ի�����ѡ����һ���ļ�����ô������ļ����ڵ�
	//Ŀ¼�ͳ��˵�ǰ���̵ĵ�ǰĿ¼�ˡ�*/
	//char buf[1000];
	//int i = 1000;
	//GetCurrentDirectory(1000, buf);  //�õ���ǰ����·��  
	//cout << buf << endl;
	//char strModule[256];
	//GetModuleFileName(NULL, strModule, 256); //�õ���ǰģ��·��  
	//cout << strModule << endl;
	//string a;
	//a.assign(buf);
	//cout << a << endl;
	//a.append("//..//");     //����Ϊ��ǰ����·��Ϊ��ʱ����һ��  
	////a=a+"..//";  
	//SetCurrentDirectory(a.c_str());  //����  
	//GetCurrentDirectory(1000, buf);
	//cout << buf << endl;

//	string str1("sophia is a good girl!");//�ó����ַ�����ֵ��
//	string str2;
//	string str3;
//	string str4;
//	string str5;
//	str2.assign(str1);//ֱ������һ���ַ�����ֵ.
//	str3.assign("sophia is a good girl!", 8);//��һ���ַ�����ǰһ���Ӵ���ֵ;
//	str4.assign(str1, 2, 10);//����һ���ַ�����һ���Ӵ���ֵ
//	str5.assign(10, 'c');//�ü�����ͬ���ַ�,��ֵ.
//	cout << "str1=  " << str1 << endl;
//	cout << "str2=  " << str2 << endl;
//	cout << "str3=  " << str3 << endl;
//	cout << "str4=  " << str4 << endl;
//	cout << "str5=  " << str5 << endl;

	/*cinͨ��ʹ�ÿհף��ո��Ʊ���ͻ��з��������ַ����Ľ硣����ζ��cin�ڶ�ȡ
	�ַ���������ʱֻ��ȡһ�����ʣ���ȡ�õ��ʺ�cin�����ַ�����һ�����У����Զ�
	��β��ӿ��ַ������⣬cinҲû�кܺõĿؼ�������ַ������������ַ�����������
	��С�����û�д���*/
	//const int size = 20;
	//char name[size];
	//char pl[size];//program language  
	//cout << "Enter your name:";
	//cin >> name;
	//cout << "Enter your favorite program language:";
	//cin >> pl;
	//cout << "Hello " << name << ", your favorite program language is " << pl << endl;
	/*cin.getline()������ȡ���У���ʹ��ͨ���س�������Ļ��з���ȷ�������β���ú�����
	������������һ�������������洢�����е���������ƣ��ڶ���������Ҫ��ȡ���ַ���������
	���ַ�����cin.getline()��Ա�����ڶ�ȡָ����Ŀ���ַ����������з�ʱֹͣ��ȡ,��Ҫע��
	���ǣ�cin.getline()�����˻��з���*/
	/*cout << "Enter your name:";
	cin.getline(name, size);
	cout << "Enter your favorite program language:";
	cin.getline(pl, size);
	cout << "Hello " << name << ", your favorite program language is " << pl << endl;*/
	/*get()�����кü��ֱ��壬������һ����getline()��ȫ��ͬ�Ĳ��������ú������ٶ�ȡ����
	�����з�(����ȡ��ζ�Ż��з��������������)*/
	/*cout << "Enter your name:";
	cin.get(name, size);
	cout << "Enter your favorite program language:";
	cin.get(pl, size);
	cout << "Hello " << name << ", your favorite program language is " << pl << endl;*/
	/*���ֲ�����ʹ��ԭ�������ڵ�һ�ε��ú󣬻��з���������������У���˵ڶ��ε���ʱ��
	���ĵ�һ���ַ�Ϊ���з������get��Ϊ�Ѿ�������β����û�з����κοɶ�ȡ�����ݡ�get()
	�������κβ����ı��壩���Զ�ȡ��һ���ַ����������з�����*/
	/*cout << "Enter your name:";
	cin.get(name, size).get();
	cout << "Enter your favorite program language:";
	cin.get(pl, size).get();
	cout << "Hello " << name << ", your favorite program language is " << pl << endl;*/
	/*��������ַ���������*/
	/*int age = 0;
	char address[size] = { 0 };
	cout << "Enter your age:";
	cin >> age;
	cin.get();
	cout << "Enter your address:";
	cin.getline(address, size);
	cout << "your age is " << age << ", your address is " << address << endl;*/

	//c++���ַ���������ַ���֮���ת����
	//string str0 = "sophia is a good girl.";
	//const char *str1 = str0.c_str();//string.c_str()���ص���const char����
	//char str2[100]="xinwen is also a good girl.";
	//string str3 = str2;
	//cout << "str0=  " << str0 << endl;
	//cout << "str1=  " << str1 << endl;
	//cout << "str2=  " << str2 << endl;
	//cout << "str3=  " << str3 << endl;
	/*CString����MFC�����¶�����ַ����࣬�ṩ�˸�����ַ������������ܡ�*/
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
	
	//�Ӽ��̸�EOF������־����������
	/*int c;
	do{
		printf("�������ĵ��Ľ�β��־");
	} while ((c = getchar()) != EOF);
	cout<<"�ѵõ��ĵ�������־"<<endl;*/

	system("Pause");
    return 0;
}