/*****************************************************************************
Author:   sophia
Date:     2016-06-30
Email: xinwen618@gmail.com
Blog: https://sophia-hxw.github.io/

*****************************************************************************
function:
���ֲ�ͬ�ķ�ʽ�õ��ļ�����ڶ�Ӧ��ԭͼ�ϱ��ֳ�����ͬʱ�ò�ͬ��ɫ���ִ��췽ʽ����ȷ��⣬����©�������

Input:  20160524_original��ԭͼ�ļ��У�
		algorithm_all_faces.txt�Ƿ�ʽһ���ɵ�heads�����
		bench_heads_detect.txt��benchmark��face_detect�������ɵ�heads�����
		bench_object_maker.txt��benchmark��objectmaker�ֶ����ɵ�heads�����

Output: AddAnnoImg�ļ��������ɵ���algorithm_all_faces.txt��bench_heads_detect.txt
		��bench_object_maker.txt�ļ������ԭͼ�ϻ�������
		��ɫ�򣺷�ʽһ����heads;  overlap<0.5;
		��ɫ�򣺷�ʽһ©���heads;  overlap<0.5;
		��ɫ�򣺷�ʽһ��ȷ������heads;  overlap>=0.5;
		��ɫ��benchmark�ж�Ӧ��ʽһ�Ѿ���ȷ������heads;  overlap>=0.5;

*****************************************************************************/

#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<string>
#include <time.h>
#include <windows.h> 
#include <io.h>
#include <direct.h>

using namespace std;
using namespace cv;


vector<string> PicName;
void getPicName(string file_name);
vector<vector<Rect> > getRect(string file_name, int PicLen);

Rect setRectInMat(Mat img, Rect rect);
string getSysTime();

void main(int argc, char* argv[]){

	string input_file0 = "algorithm_all_faces.txt";
	string input_file1 = "bench_heads_detect.txt";
	string input_file2 = "bench_object_maker.txt";
	string outputFolder = "AddAnnoImg";

	string OrigFolderName = "20160524_original";
	getPicName(OrigFolderName);
	int PicLen = PicName.size();//Number of all pictures.

	/*
	int digitnum0;
	cout << "������algorithm_all_faces.txt�ļ�������ȷ��һ���ļ��Ĺؼ�����λ����(��.jpg��ǰ����λ��) " << endl;
	cin >> digitnum0;

	int digitnum1;
	cout << "������bench_heads_detect.txt�ļ�������ȷ��һ���ļ��Ĺؼ�����λ����(��.jpg��ǰ����λ��) " << endl;
	cin >> digitnum1;

	int digitnum2;
	cout << "������bench_object_maker.txt�ļ�������ȷ��һ���ļ��Ĺؼ�����λ����(��.jpg��ǰ����λ��) " << endl;
	cin >> digitnum2; */

	int digitnum0 = 9;
	int digitnum1 = 18;
	int digitnum2 = 18;

	const char* t0 = outputFolder.c_str();//string to char
	if (_access(t0, 0) == -1)//ȷ���ļ����ļ��еķ���Ȩ��
		_mkdir(t0);//�����ļ���

	vector<vector<Rect>> Rect_table0 = getRect(input_file0, PicLen);//ȡ�����з�ʽһ����heads����
	vector<vector<Rect>> Rect_table1 = getRect(input_file1, PicLen);//ȡ������benchmark����bench_heads_detect����
	vector<vector<Rect>> Rect_table2 = getRect(input_file2, PicLen);//ȡ������benchmark����bench_object_maker����

	vector<vector<int> > truepositive(PicLen);
	vector<vector<int> > truepositive0(PicLen);


	for (int k = 0; k < PicLen; k++){
		int obj_num0 = Rect_table0[k].size();
		int obj_num1 = Rect_table1[k].size();
		int obj_num2 = Rect_table2[k].size();

		int alength = Rect_table0[k].size();//Algorithm
		int blength1 = Rect_table1[k].size();//bench_heads_detect
		int blength2 = Rect_table2[k].size();//bench_object_maker

		truepositive[k].resize(blength1 + blength2, 0);//benchmark
		truepositive0[k].resize(alength, 0);//Algorithm

		for (int ib = 0; ib < alength; ib++){
			Rect rectalgo = Rect_table0[k][ib];

			for (int ic = 0; ic <blength1; ic++){
				Rect rectbench = Rect_table1[k][ic];
				Rect interect = rectalgo & rectbench;
				Rect uniorect = rectalgo | rectbench;
				double rationum = (double)interect.area() / (double)uniorect.area();

				if (rationum>=0.5){
					truepositive[k][ic] = 1;
					truepositive0[k][ib] = 1;
				}
			}
		}

		for (int ib = 0; ib < alength; ib++){
			Rect rectalgo = Rect_table0[k][ib];

			for (int ic = 0; ic <blength2; ic++){
				Rect rectbench = Rect_table2[k][ic];
				Rect interect = rectalgo & rectbench;
				Rect uniorect = rectalgo | rectbench;
				double rationum = (double)interect.area() / (double)uniorect.area();

				if (rationum>=0.5){
					truepositive[k][blength1 + ic] = 1;
					truepositive0[k][ib] = 1;
				}
			}
		}
	}

	for (int k = 0; k < PicLen; k++){
		PicName[k] = OrigFolderName + "//" + PicName[k];
		cout << PicName[k] << endl;
		Mat img = imread(PicName[k]);

		Mat copyMat;
		img.copyTo(copyMat);

		int obj_num0 = Rect_table0[k].size();
		int obj_num1 = Rect_table1[k].size();
		int obj_num2 = Rect_table2[k].size();

		int blength1 = Rect_table1[k].size();//bench_heads_detect

		for (int j = 0; j<obj_num0; j++)
		{
			if (truepositive0[k][j] == 0){//��ʽһ���
				Rect rect = Rect_table0[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 0, 255), 2, CV_AA, 0);
			}
			else{//��ʽһ�����ȷ��heads
				Rect rect = Rect_table0[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 2, CV_AA, 0);
			}
		}

		for (int j = 0; j < obj_num1; j++)//bench_heads_detect��ʵ���Ƿ�ʽһ©��
		{
			if (truepositive[k][j] == 0){
				Rect rect = Rect_table1[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(255, 0, 0), 2, CV_AA, 0);
			}
			else{
				Rect rect = Rect_table1[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 2, CV_AA, 0);
			}
		}

		for (int j = 0; j<obj_num2; j++)//bench_heads_detect��ʵ���Ƿ�ʽһ©��
		{
			if (truepositive[k][blength1 + j] == 0){
				Rect rect = Rect_table2[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(2 * rect.x, 2 * rect.y), Point(2 * rect.x + 2 * rect.width, 2 * rect.y + 2 * rect.height), Scalar(255, 0, 0), 2, CV_AA, 0);
			}
			else{
				Rect rect = Rect_table2[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(2 * rect.x, 2 * rect.y), Point(2 * rect.x + 2 * rect.width, 2 * rect.y + 2 * rect.height), Scalar(0, 255, 0), 2, CV_AA, 0);
			}
		}

		//imshow("AnnotationResult", copyMat);

		stringstream buf;
		string str = PicName[k];
		size_t found_2 = str.find(".");

		string OutimgName = "";
		string imgNamecache = PicName[k].substr(found_2 - digitnum0, found_2);
		
		OutimgName.append(outputFolder);
		OutimgName.append("//");
		OutimgName.append(imgNamecache);
		
		imwrite(OutimgName, copyMat);

		string tmp_name = str.substr(0, found_2);
		buf << "result/" << tmp_name << "-result.jpg";
		cout << buf.str() << endl;
		imwrite(buf.str(), img);
		
		waitKey(1);
	}

}

vector<vector<Rect> > getRect(string file_name, int PicLen){
	vector<vector<Rect> > Object(PicLen);
	for (int mm = 0; mm < PicLen; mm++)
		Object[mm].resize(0);

	ifstream myReadFile;
	myReadFile.open(file_name);
	char output[1000];

	if (myReadFile.is_open()){
		while (!myReadFile.eof()){

			myReadFile >> output;
			//cout << "output0: " << output << endl;
			string s0 = string(output);
			if (s0.length() > 10){
				size_t found1 = s0.find(" ");
				string tmp_name = s0.substr(0, found1);
				int tmp_pos;
				for (int ia = 0; ia < PicLen; ia++){
					size_t lenstria = PicName[ia].size();
					string stria = PicName[ia].substr(0, lenstria - 4);
					//cout << stria<<endl;
					string::size_type idx = tmp_name.find(stria, 0);
					if (idx != string::npos){
						tmp_pos = ia;
						break;//�ҵ��Ӵ����˳�ѭ��
					}
				}
				
				myReadFile >> output;
				
				int obj_num = atoi(output);
				int obj[4];

				//int before_num = Object[tmp_pos].size();
				
				for (int i = 0; i<obj_num; i++){
					//cout << Object[tmp_pos].size()<<endl;
					Object[tmp_pos].resize(obj_num);
					for (int j = 0; j<4; j++){
						myReadFile >> output;
						int p = atoi(output);
						obj[j] = p;
					}
					Object[tmp_pos][i] = Rect(obj[0], obj[1], obj[2], obj[3]);
				}

			}
		}
	}
	myReadFile.close();
	return(Object);
}

void getPicName(string OrigFolderName){
	string orderA = "DIR .\\" + OrigFolderName + "\\*.jpg / B >SmallList.TXT ";
	system(orderA.c_str());
	string OrigFileName = "SmallList.TXT";
	ifstream OrigNameIn(OrigFileName);
	while (!OrigNameIn.eof()){
		string cacheNameO;
		getline(OrigNameIn, cacheNameO);
		int len = cacheNameO.size();
		if (len>0){
			string realName = cacheNameO.substr(0, len);
			//cout << realName << endl;
			PicName.push_back(realName);
		}
	}

	OrigNameIn.close();
	string order = "del " + OrigFileName;
	system(order.c_str());
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

string getSysTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	std::stringstream ss;

	stringstream os_year_, os_month_, os_day_, os_hour_, os_minute_, os_second_, os_millisecond_;
	os_year_ << sys.wYear;
	os_month_ << sys.wMonth;
	os_day_ << sys.wDay;
	os_hour_ << sys.wHour;
	os_minute_ << sys.wMinute;
	os_second_ << sys.wSecond;
	os_millisecond_ << sys.wMilliseconds;

	string stmStr;
	char buf[1024];
	sprintf(buf, "%04d%02d%02d%02d%02d%02d%03d", atoi(os_year_.str().c_str()), atoi(os_month_.str().c_str()), atoi(os_day_.str().c_str()),
		atoi(os_hour_.str().c_str()), atoi(os_minute_.str().c_str()), atoi(os_second_.str().c_str()), atoi(os_millisecond_.str().c_str()));
	stmStr.assign(buf);

	return stmStr;
}