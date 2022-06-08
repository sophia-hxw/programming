//*****************************************************************************
//Author: sophia
//Date: 2016-05-11
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function:  Transform many pictures into a video with suffix ".avi".
//Input:   	 A folder name that have all your pictures.
//Output:  	 A video made by your images.

//Usage：
//#1，将所有图片放在文件夹中，并与可执行文件夹GeneVideo.exe置于同一目录下；
//#2，按照DOS提示给出#1中图片文件夹的名字；
//#3，运行程序将会生成与#1中图片文件夹同名的视频文件，会在生成视频的同时播放该文件。
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

void getNameFromTxt(vector<string> &PicName, string TxtName);

int main(){
	string foldername;
	cout << "Give a name of folder that have all your pictures: " << endl;
	cin >> foldername;

	string orderca = "DIR .\\" + foldername + "\\*.jpg / B >Picture.TXT ";
	system(orderca.c_str());

	vector<string> PicName;
	getNameFromTxt(PicName, "Picture.TXT");
	string PicPathName = foldername + "\\" + PicName[1];
	Mat picture = imread(PicPathName.c_str(), 1);
	cout << "rows: " << picture.rows << "\t height: " << picture.cols<<endl;
	int frameRate = 5;
	

	string videoName = foldername + ".avi";
	VideoWriter myVideo;

	int len = PicName.size();
	Mat framePic;
	Size framesize;
	for (int ia = 0; ia < len; ia++){
		string PicPathName = foldername + "\\" + PicName[ia];
		framePic = imread(PicPathName.c_str(), 1);
		
		if (!myVideo.isOpened()){
			framesize.width = framePic.cols;
			framesize.height = framePic.rows;
			if (!myVideo.open(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, framesize, true)){
				cout << "open writer error..." << endl;
				return -1;
			}
		}
		
		myVideo.write(framePic);

		imshow("video", framePic);
		waitKey(frameRate);
	}
	myVideo.release();

	system("Pause");
	return 0;
}

void getNameFromTxt(vector<string> &PicName, string TxtName){
	ifstream OrigNameIn(TxtName);
	while (!OrigNameIn.eof()){
		string cacheNameO;
		getline(OrigNameIn, cacheNameO);
		int len = cacheNameO.size();
		if (len>0)
			PicName.push_back(cacheNameO);
	}

	OrigNameIn.close();
	string order = "del " + TxtName;
	system(order.c_str());
}