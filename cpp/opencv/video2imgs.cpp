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

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
	cv::CommandLineParser parser(argc, argv, "{@input |20160401140135393_00000.avi| }");
	string videoname(parser.get<string>("@input"));

	//another method to get the name of the video
/*	system("DIR *.avi / B >LIST.TXT ");
	string videoname;
	ifstream videoIn("LIST.TXT");
	getline(videoIn, videoname);*/


	//create a new folder with the same name of video
	string folderName;
	int len = videoname.size();
	folderName = videoname.substr(0,len-4);
	string newfolder = "md " + folderName;
	system(newfolder.c_str());

	VideoCapture capture(videoname);
	if (!capture.isOpened())
		cout << "fail to open video!" << endl;

	long totalFrameNum = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "total number of frames: " << totalFrameNum << endl;

	//give every picture a name
	vector<string> PicName;
	long PicNumber = totalFrameNum / 10 + 1;
	for (long i = 0; i < PicNumber; i++){
		stringstream Cnumber;
		Cnumber << i;
		string cacheN = folderName + "_" + Cnumber.str() + ".jpg";
		PicName.push_back(cacheN);
	}

	long PicIndex = 0;
	for (long i = 0; i < totalFrameNum; i++){
		Mat frame;	
		if ((i==0) || ((i+1)% 10 == 0)){
			if (!capture.read(frame)){
				cout << "fail to read a frame from a video!" << endl;
				return -1;
			}
			imwrite(PicName[PicIndex], frame);
			PicIndex = PicIndex + 1;
		}
	}

	//move all pictures to your folder
	for (long i = 0; i < PicNumber; i++){
		string mvorder = "move " + PicName[i] + " " + folderName + " >nul";
		system(mvorder.c_str());
	}

	capture.release();
	system("pause");
	return 0;
}