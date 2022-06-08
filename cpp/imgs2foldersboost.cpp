//*****************************************************************************
//Author: sophia
//Date: 2016-04-15
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function: 将当前文件夹中的大量的.jpg文件分别放到不同文件夹中
//Input:   在dos提示下，输入想要分装这些图片的文件夹的个数
//Output:  不同文件夹，不同的图片已经分别放入其中
//*****************************************************************************
//改进：只能对.jpg文件处理，没有让用户对需要修改的文件格式进行选择。
//	添加了boost库的多线程处理，会加速分图的过程。
//Usage：
//#1，将带分的图片和可执行文件countfiles.exe置于同一文件夹下；
//#2，在dos提示下，输入想要分装这些图片的文件夹的个数；
//#3，回车即可将大量.jpg文件尽可能平均分放于不同文件夹。
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
	cout <<"\n 当前文件数量filenumber=  "<<filenumber<< endl;


	int foldernumber;
	cout << "请输入文件夹个数:" << endl;
	cin >> foldernumber;
	cout << "\n 文件夹个数foldernumber=  " << foldernumber << endl;


	int filePerfolder;
	filePerfolder = filenumber / foldernumber;
	cout << "\n 每个文件夹中的文件个数filePerfolder=  " << filePerfolder << endl;


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
		else{//最后一个文件夹
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