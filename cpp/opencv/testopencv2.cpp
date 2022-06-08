#include"highgui.h"
#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(){

	cv::Mat image;

	cout << "This image is" << image.rows << "x" << image.cols << endl;

	image = cv::imread("qiaoba.jpg");

	cv::namedWindow("test_sophia");

	cv::imshow("test_sophia", image);

	cv::waitKey(0);

	return 0;

}