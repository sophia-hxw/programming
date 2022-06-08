//*****************************************************************************
//Author: sophia
//Date: 2016-04-13
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function: ����ǰ�ļ����еĴ�����.jpg�ļ��ֱ�ŵ���ͬ�ļ�����
//Input:   ��dos��ʾ�£�������Ҫ��װ��ЩͼƬ���ļ��еĸ���
//Output:  ��ͬ�ļ��У���ͬ��ͼƬ�Ѿ��ֱ��������
//*****************************************************************************
//�Ľ���ÿ���ƶ�һ���ļ�������dos�����������Ҫ�Ľ���
//	ֻ�ܶ�.jpg�ļ�����û�����û�����Ҫ�޸ĵ��ļ���ʽ����ѡ��
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

using namespace std;

int main()
{
	//��ǰ�ļ�����
	system("dir /b /a-d .\\*.jpg | find /v /c \" \">CountNum.txt ");
	ifstream fin("CountNum.txt");
	int filenumber = 0;
	fin >> filenumber;
	cout <<"\n ��ǰ�ļ�����filenumber=  "<<filenumber<< endl;

	//Ӧ�û��ֵ��ļ�������
	int foldernumber;
	cout << "�������ļ��и���:" << endl;
	cin >> foldernumber;
	cout << "\n �ļ��и���foldernumber=  " << foldernumber << endl;

	//ÿ���ļ����е��ļ�����
	int filePerfolder;
	filePerfolder = filenumber / foldernumber;
	cout << "\n ÿ���ļ����е��ļ�����filePerfolder=  " << filePerfolder << endl;

	//�����ļ�������
	vector<string> foldername;
	if (foldernumber != 0){
		for (int i = 0; i < foldernumber ; i++){
			stringstream cache;
			cache << i;
			string str = "picture-" + cache.str();
			string order = "md " + str;
			foldername.push_back(str);		
			system(order.c_str());								//system("md picture-1");
//			cout << "\n �ļ�������= " << foldername[i] << endl;
//			cout << "\n �½��ļ�������= " << order << endl;
		}
	}


	//����ÿ���ļ�����
	system("DIR *.jpg / B >LIST.TXT ");

	//�����ļ�����
	ifstream in("LIST.TXT");
	string line;
	vector<string> fileNames;
	for (int i = 0; i < filenumber;i++){
		getline(in, line);
		fileNames.push_back(line);
	}	

	//�����ļ�����Ӧ���ļ���ȥ
	int folderindex = 0;
	for (int i = 0; i < filenumber; i++){
		if ((i % filePerfolder) == 0 & (i!=0))
			folderindex += 1;

		if (folderindex >= (foldernumber-1)){							/*���һ���ļ��У�Ҫʣ�������ļ�*/
			string forcache;
			forcache = "move " + fileNames[i] + " " + foldername[folderindex-1]+" >nul";
			system(forcache.c_str());									//	system("move 000753116_000006.jpg picture-1");
		}
		else{
			string forcache;
			forcache = "move " + fileNames[i] + " " + foldername[folderindex]+" >nul";
			system(forcache.c_str());
		}
		
	}

	system("Pause");
	return 0;
}