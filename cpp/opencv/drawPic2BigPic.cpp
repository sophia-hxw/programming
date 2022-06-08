//*****************************************************************************
//Author: sophia
//Date: 2016-05-10
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function: Giving detected regions to its original picture. 
//Input:   Foler name that you give to your detected pictures.
//Output:  Folder named detection that have all pictures you need. 
//*****************************************************************************
//Improve：This program may needs some parallel processing.
//Usage:
//#1，将DrawPic2Pic.exe放在与原图文件夹（例如为：20160510_original）和检测出的头像文件夹（例如为：20160510）同一个目录下；,
//#2，在dos端输入检测出的头像文件夹名，此处为：20160510；原图像文件的命名会默认为：检测文件夹名称_original，如：20160510_original
//#3，获得detection文件夹，其中保存的就是原图中画出了人头的结果图。
//*****************************************************************************


#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>

using namespace std;
using namespace cv;

void outputVecStr(vector<string>& p);
void getNameFromTxt(vector<string> &OrigNamePic, string OrigFileName);
void getPos(Point& pt1, Point& pt2, string& SmallNamePic, string& OrigNamePic,Mat& DeteImg);

int main(){
	string SmallName;
	cout << "Give your name of folder that has detected small pictures:" << endl;
	cin >> SmallName;
//	string SmallName = "20160510";
	string orderS = "DIR .\\" + SmallName + "\\*.jpg / B >SmallList.TXT ";
	system(orderS.c_str());

	string OrigName = SmallName+"_original";
	string orderA = "DIR .\\" + OrigName + "\\*.jpg / B >OrigList.TXT ";
	system(orderA.c_str());

	vector<string> OrigNamePic;
	string OrigTxt = "OrigList.TXT";
	getNameFromTxt(OrigNamePic, OrigTxt);

//	outputVecStr(OrigNamePic);

	vector<string> SmallNamePic;
	string SmallTxt = "SmallList.TXT";
	getNameFromTxt(SmallNamePic, SmallTxt);

//	outputVecStr(SmallNamePic);
	
	system("md detection");

	int SmallgLen = SmallNamePic.size();
	int OrigLen = OrigNamePic.size();
	int smallindex = 0;
	Point pt1;
	Point pt2;
	for (int ia = 0; ia < OrigLen; ia++){
		string OrigPic = OrigName + "\\" + OrigNamePic[ia] + ".jpg";
		Mat DeteImg;
		DeteImg = imread(OrigPic.c_str(), 1);
	
		for (int ib = smallindex; ib < SmallgLen; ib++){
			string::size_type idx = SmallNamePic[ib].find(OrigNamePic[ia]);
			if (idx != string::npos){
				getPos(pt1, pt2, SmallNamePic[ib], OrigNamePic[ia], DeteImg);
				smallindex = smallindex + 1;
			}
			else
				break;
		}

		string detectName = OrigNamePic[ia] + "_detected.jpg";
		imwrite(detectName, DeteImg);
		string moveorder = "move " + detectName + " detection >nul";
		system(moveorder.c_str());
	}
	system("Pause");
	return 0;
}

void outputVecStr(vector<string>& p){
	int len = p.size();

	for (int i = 0; i < len; i++)
		cout << "The "<<i<<"-th element is: " << p[i] << endl;
}

void getNameFromTxt(vector<string> &OrigNamePic, string OrigFileName){
	ifstream OrigNameIn(OrigFileName);
	while (!OrigNameIn.eof()){
		string cacheNameO;
		getline(OrigNameIn, cacheNameO);
		int len = cacheNameO.size();
		if (len>0){
			string realName = cacheNameO.substr(0, len-4);
			OrigNamePic.push_back(realName);
		}
	}

	OrigNameIn.close();
	string order = "del " + OrigFileName;
	system(order.c_str());
}

void getPos(Point& pt1, Point& pt2, string& SmallNamePic, string& OrigNamePic, Mat& DeteImg){
	int len = OrigNamePic.size();
	int Slen = SmallNamePic.size();
	int Numpos = SmallNamePic.find('_', 0);
	int xpos = SmallNamePic.find('_', Numpos + 1);
	int ypos = SmallNamePic.find('_', xpos+1);
	int wpos = SmallNamePic.find('_', ypos + 1);
	int hpos = SmallNamePic.find('_', wpos + 1);

	int x, y, w, h;
	string xcache = SmallNamePic.substr(xpos + 1, ypos-1);
	x = atoi(xcache.c_str());
	string ycache = SmallNamePic.substr(ypos + 1, wpos-1);
	y = atoi(ycache.c_str());
	string wcache = SmallNamePic.substr(wpos + 1, hpos-1);
	w = atoi(wcache.c_str());
	string hcache = SmallNamePic.substr(hpos + 1, Slen);
	h = atoi(hcache.c_str());

	pt1.x = x;
	pt1.y = y;
	pt2.x = x + h;
	pt2.y = y + w;

	rectangle(DeteImg, pt1, pt2, 255, 1, 8, 0);
}