//*****************************************************************************
//Author: sophia
//Date: 2016-05-04
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
//*****************************************************************************
//Function: 对当前文件夹下的所有视频进行处理，每隔10帧保存图片
//Input:   
//Output:  视频中每隔10帧的图片，保存为.jpg格式
//*****************************************************************************
//改进：没有给定格式视频的情况下，dos段会提示“找不到文件”。
//Usage：
//#1，将后缀文件VideoFormat.txt，视频文件和可执行文件PicFromVideo.exe置于同一文件夹；
//#2，运行程序可得到与视频同名的文件夹，里面保存的是从该视频中截取的图片。
//Note：
//所有VideoFormat.txt中列出的后缀的视频都可以处理，不同类别、不同数量的视频会依次处理得到相应的图片。
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

void creatFolder(vector<string> videoname, vector<string>& folderName);
void moveFile(vector<string>& PicName, string folderName, long PicNumber);
void givenameFile(vector<string>& PicName, long PicNumber, string folderName);
void getnameVideo(string filesuf, vector<string>& videoname);
int savePicture(vector<string>& PicName, string videoname, string folderName, long &PicNumber);

int main(int argc, char *argv[]){
	double beginTime = clock();
	ifstream formatfile;
	string formatname = {"VideoFormat.txt"};
	formatfile.open(formatname, ios::in);
	vector<string > Vformat;
	if (formatfile.fail()){
		cout << "There is no \"VideoFormat.txt \" file here! " << endl;
		formatfile.close();
	}
	else{
		while (!formatfile.eof()){
			string cacheFor;
			getline(formatfile,cacheFor);
			Vformat.push_back(cacheFor);
		}
		Vformat.pop_back();
		formatfile.close();
	}
	
	int numFV=Vformat.size();
	for (int bi = 0; bi < numFV; bi++){
		string filesuf=Vformat[bi];
		vector<string> videoname;
		getnameVideo(filesuf, videoname);

		//create a new folder with the same name of video
		vector<string> folderName;
		creatFolder(videoname, folderName);

		size_t numVideo = videoname.size();
		size_t numFold = folderName.size();
		if (numVideo != numFold)
			cout << "something wrong with the \" getnameVideo \" function! " << endl;
		for (int i = 0; i < (int)numVideo; i++){
			//save picture
			vector<string> PicName;
			long PicNumber;
			int saveindex;
			saveindex = savePicture(PicName, videoname[i], folderName[i], PicNumber);

			if (saveindex < 0)
				return -1;

			//move all pictures to your folder
			moveFile(PicName, folderName[i], PicNumber);
		}
	}

	double endTime = clock();
	cout << "This program use " << endTime - beginTime << " (ms) in total." << endl;

	system("pause");
	return 0;
}

void creatFolder(vector<string> videoname, vector<string>& folderName){
	size_t numberVideo = videoname.size();
	for (int i = 0; i < (int)numberVideo; i++){
		string cacheFN;
		int len = videoname[i].size();
		cacheFN = videoname[i].substr(0, len - 4);
		folderName.push_back(cacheFN);
		string newfolder = "md " + cacheFN;
		system(newfolder.c_str());
	}
}

void moveFile(vector<string>& PicName, string folderName, long PicNumber){
	for (long i = 0; i < PicNumber; i++){
		string mvorder = "move " + PicName[i] + " " + folderName + " >nul";
		system(mvorder.c_str());
	}
}

void givenameFile(vector<string>& PicName, long PicNumber, string folderName){
	for (long i = 0; i < PicNumber; i++){
		stringstream Cnumber;
		Cnumber << i;
		string cacheN = folderName + "_" + Cnumber.str() + ".jpg";
		PicName.push_back(cacheN);
	}
}

void getnameVideo(string filesuf, vector<string>& videoname){
	string cacheorder;
	cacheorder = "DIR *." + filesuf + " / B >LIST.TXT ";
	system(cacheorder.c_str());
	ifstream videoIn("LIST.TXT");
	while (!videoIn.eof()){
		string cacheFor;
		getline(videoIn, cacheFor);
		videoname.push_back(cacheFor);
	}
	videoname.pop_back();
	videoIn.close();
	system("del LIST.TXT");//delete the txt file
}

int savePicture(vector<string>& PicName, string videoname, string folderName, long &PicNumber){
	VideoCapture capture(videoname);
	if (!capture.isOpened())
		cout << "fail to open video!" << endl;

	long totalFrameNum = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "total number of frames: " << totalFrameNum << endl;

	//give every picture a name	
	PicNumber = totalFrameNum / 10 + 1;
	givenameFile(PicName, PicNumber, folderName);

	long PicIndex = 0;
	for (long i = 0; i < totalFrameNum; i++){
		Mat frame;
		if ((i == 0) || ((i + 1) % 10 == 0)){
			if (!capture.read(frame)){
				cout << "fail to read a frame from a video!" << endl;
				return -1;
			}
			imwrite(PicName[PicIndex], frame);
			PicIndex = PicIndex + 1;
		}
	}
	capture.release();

	return 0;
}