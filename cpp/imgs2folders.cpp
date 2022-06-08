//*****************************************************************************
//Author: sophia
//Date: 2016-04-13
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function: 将当前文件夹中的大量的.jpg文件分别放到不同文件夹中
//Input:   在dos提示下，输入想要分装这些图片的文件夹的个数
//Output:  不同文件夹，不同的图片已经分别放入其中
//*****************************************************************************
//改进：每次移动一个文件都会在dos中有输出，需要改进；
//	只能对.jpg文件处理，没有让用户对需要修改的文件格式进行选择。
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

using namespace std;

int main()
{
	//当前文件数量
	system("dir /b /a-d .\\*.jpg | find /v /c \" \">CountNum.txt ");
	ifstream fin("CountNum.txt");
	int filenumber = 0;
	fin >> filenumber;
	cout <<"\n 当前文件数量filenumber=  "<<filenumber<< endl;

	//应该划分的文件夹数量
	int foldernumber;
	cout << "请输入文件夹个数:" << endl;
	cin >> foldernumber;
	cout << "\n 文件夹个数foldernumber=  " << foldernumber << endl;

	//每个文件夹中的文件个数
	int filePerfolder;
	filePerfolder = filenumber / foldernumber;
	cout << "\n 每个文件夹中的文件个数filePerfolder=  " << filePerfolder << endl;

	//处理文件夹名字
	vector<string> foldername;
	if (foldernumber != 0){
		for (int i = 0; i < foldernumber ; i++){
			stringstream cache;
			cache << i;
			string str = "picture-" + cache.str();
			string order = "md " + str;
			foldername.push_back(str);		
			system(order.c_str());								//system("md picture-1");
//			cout << "\n 文件夹名字= " << foldername[i] << endl;
//			cout << "\n 新建文件夹命令= " << order << endl;
		}
	}


	//读入每个文件名字
	system("DIR *.jpg / B >LIST.TXT ");

	//处理文件名字
	ifstream in("LIST.TXT");
	string line;
	vector<string> fileNames;
	for (int i = 0; i < filenumber;i++){
		getline(in, line);
		fileNames.push_back(line);
	}	

	//划分文件到相应的文件夹去
	int folderindex = 0;
	for (int i = 0; i < filenumber; i++){
		if ((i % filePerfolder) == 0 & (i!=0))
			folderindex += 1;

		if (folderindex >= (foldernumber-1)){							/*最后一个文件夹，要剩下所有文件*/
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