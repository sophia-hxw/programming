//*****************************************************************************
//Author: sophia
//Date: 2016-05-11
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function: Giving detected regions to its original picture and print rect params in MS-DOS
//Input:   Foler name that you give to your detected pictures.
//Output:  Folder named detection that have all pictures you need. 
//	   A file named "drawPic2Pic.txt" that has information of detected 
//		heads in original pictures.
//*****************************************************************************
//Improve：This program may needs some parallel processing.
//Usage：
//#1，将DrawPic2Pic.exe放在与原图文件夹（例如为：20160510_original）和检测出的头像文件夹（例如为：20160510）同一个目录下；,
//#2，在dos端输入检测出的头像文件夹名，此处为：20160510；原图像文件的命名会默认为：检测文件夹名称_original，如：20160510_original
//#3，获得detection文件夹，其中保存的就是原图中画出了人头的结果图，MS-DOS端会输出检测人头的rect数据，同时还有PicParameter.txt记录原图名称和相应人头个数与位置。
//Note：人头文件的名称中皆有人头位置参数！
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
Rect setRectInMat(Mat img, Rect rect);

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

	ofstream parameter;
	parameter.open("PicParameter.txt", ios::app);
	parameter << "\n \n 1-col: Name of file \t 2-col: number of heads \t 3-col: [x,y,width,height] "<<endl;

	for (int ia = 0; ia < OrigLen; ia++){
		string OrigPic = OrigName + "\\" + OrigNamePic[ia] + ".jpg";
		Mat DeteImg;
		DeteImg = imread(OrigPic.c_str(), 1);
		vector<Point> PtAll1;
		vector<Point> PtAll2;

		string txtOrigName = OrigNamePic[ia] + "_detected.jpg";
		parameter << "\n"<<txtOrigName;

		int numberPic = 0;
		for (int ib = smallindex; ib < SmallgLen; ib++){
			string::size_type idx = SmallNamePic[ib].find(OrigNamePic[ia]);
			if (idx != string::npos){
				getPos(pt1, pt2, SmallNamePic[ib], OrigNamePic[ia], DeteImg);
				PtAll1.push_back(pt1);
				PtAll2.push_back(pt2);
				smallindex = smallindex + 1;
				numberPic = numberPic + 1;
			}
			else
				break;
		}

		parameter << "\t"<<numberPic;
		for (int ic = 0; ic < numberPic; ic++){
			parameter << "\t" << PtAll1[ic].x << "  " << PtAll1[ic].y << "  " << PtAll2[ic].x - PtAll1[ic].x << "  " << PtAll2[ic].y - PtAll1[ic].y ;
		}
		
		if (numberPic != 0){
			string detectName = "detection//" + OrigNamePic[ia] + "_detected.jpg";
			imwrite(detectName, DeteImg);		
			//string moveorder = "move " + detectName + " detection >nul";
			//system(moveorder.c_str());
		}	
	}

	parameter.close();

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

	rectangle(DeteImg, pt1, pt2, 255, 2, 8, 0);
}

Rect setRectInMat(Mat img, Rect rect)
{
	int x = rect.x;
	int y = rect.y;
	int w = rect.width;
	int h = rect.height;

	x = max(x, 5);
	x = min(x, img.cols - 5);
	y = max(y, 5);
	y = min(y, img.rows - 5);

	w = min(w, img.cols - x - 5);
	h = min(h, img.rows - y - 5);

	return Rect(x, y, w, h);
}
