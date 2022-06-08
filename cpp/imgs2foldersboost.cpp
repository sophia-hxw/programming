//*****************************************************************************
//Author: sophia
//Date: 2016-04-15
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function: ����ǰ�ļ����еĴ�����.jpg�ļ��ֱ�ŵ���ͬ�ļ�����
//Input:   ��dos��ʾ�£�������Ҫ��װ��ЩͼƬ���ļ��еĸ���
//Output:  ��ͬ�ļ��У���ͬ��ͼƬ�Ѿ��ֱ��������
//*****************************************************************************
//�Ľ���ֻ�ܶ�.jpg�ļ�����û�����û�����Ҫ�޸ĵ��ļ���ʽ����ѡ��
//	�����boost��Ķ��̴߳�������ٷ�ͼ�Ĺ��̡�
//Usage��
//#1�������ֵ�ͼƬ�Ϳ�ִ���ļ�countfiles.exe����ͬһ�ļ����£�
//#2����dos��ʾ�£�������Ҫ��װ��ЩͼƬ���ļ��еĸ�����
//#3���س����ɽ�����.jpg�ļ�������ƽ���ַ��ڲ�ͬ�ļ��С�
//*****************************************************************************

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <atomic>
#include <time.h>

using namespace std;

void movefile(string &foldername, vector<string> &fileNames);

int main()
{
	double beginTime = clock();


	system("dir /b /a-d .\\*.jpg | find /v /c \" \">CountNum.txt ");
	ifstream fin("CountNum.txt");
	int filenumber = 0;
	fin >> filenumber;
	cout <<"\n ��ǰ�ļ�����filenumber=  "<<filenumber<< endl;


	int foldernumber;
	cout << "�������ļ��и���:" << endl;
	cin >> foldernumber;
	cout << "\n �ļ��и���foldernumber=  " << foldernumber << endl;


	int filePerfolder;
	filePerfolder = filenumber / foldernumber;
	cout << "\n ÿ���ļ����е��ļ�����filePerfolder=  " << filePerfolder << endl;


	vector<string> foldername;
	if (foldernumber != 0){
		for (int i = 0; i < foldernumber ; i++){
			stringstream cache;
			cache << i;
			string str = "picture-" + cache.str();
			string order = "md " + str;
			foldername.push_back(str);		
			system(order.c_str());//system("md picture-1");
		}
	}


	system("DIR *.jpg / B >LIST.TXT ");


	ifstream in("LIST.TXT");
	string line;
	vector<string> fileNames;
	for (int i = 0; i < filenumber;i++){
		getline(in, line);
		fileNames.push_back(line);
	}	

	vector<thread> threads;
	for (int i = 0; i < foldernumber; i++){
		string FolderNameFunc = foldername[i];
		if (i != (foldernumber - 1)){
			vector<string> FileNameFunc;
			int beginNum = i*filePerfolder;
			for (int j = 0; j < filePerfolder;j++)
				FileNameFunc.push_back(fileNames[beginNum+j]);
			threads.push_back(thread(movefile, FolderNameFunc, FileNameFunc));
		}
		else{//���һ���ļ���
			vector<string> FileNameFunc;
			int beginNum = i*filePerfolder;
			int NewNum = filenumber - (foldernumber - 1)*filePerfolder;
			for (int j = 0; j < NewNum;j++)
				FileNameFunc.push_back(fileNames[beginNum + j]);
			threads.push_back(thread(movefile, FolderNameFunc, FileNameFunc));
		}
	}

	for (auto & th : threads)
		th.join();

	double endTime = clock();

	cout << "beginTime:" << beginTime << endl
		<< "endTime:" << endTime << endl
		<< "endTime-beginTime:" << endTime - beginTime <<"ms"<< endl;

	system("Pause");
	return 0;
}


void movefile(string &foldername, vector<string> &fileNames){
	for (vector<string>::size_type i = 0; i != fileNames.size(); i++){
		string forcache;
		forcache = "move " + fileNames[i] + " " + foldername + " >nul";
		system(forcache.c_str());			//	system("move 000753116_000006.jpg picture-1");
	}										
}