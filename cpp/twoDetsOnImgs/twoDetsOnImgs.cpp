/*****************************************************************************
Author:   sophia
Date:     2016-06-30
Email: xinwen618@gmail.com
Blog: https://sophia-hxw.github.io/

*****************************************************************************
function:
两种不同的方式得到的检测结果在对应的原图上表现出来，同时用不同颜色区分待检方式的正确检测，错检和漏检情况。

Input:  20160524_original是原图文件夹；
		algorithm_all_faces.txt是方式一生成的heads结果；
		bench_heads_detect.txt是benchmark中face_detect程序生成的heads结果；
		bench_object_maker.txt是benchmark中objectmaker手动生成的heads结果；

Output: AddAnnoImg文件夹中生成的是algorithm_all_faces.txt和bench_heads_detect.txt
		与bench_object_maker.txt的检测结果在原图上画出来；
		红色框：方式一误检的heads;  overlap<0.5;
		蓝色框：方式一漏检的heads;  overlap<0.5;
		绿色框：方式一正确检测出的heads;  overlap>=0.5;
		黄色框：benchmark中对应方式一已经正确检测出的heads;  overlap>=0.5;

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
	cout << "请依据algorithm_all_faces.txt文件输入能确定一个文件的关键名称位数：(从.jpg往前多少位？) " << endl;
	cin >> digitnum0;

	int digitnum1;
	cout << "请依据bench_heads_detect.txt文件输入能确定一个文件的关键名称位数：(从.jpg往前多少位？) " << endl;
	cin >> digitnum1;

	int digitnum2;
	cout << "请依据bench_object_maker.txt文件输入能确定一个文件的关键名称位数：(从.jpg往前多少位？) " << endl;
	cin >> digitnum2; */

	int digitnum0 = 9;
	int digitnum1 = 18;
	int digitnum2 = 18;

	const char* t0 = outputFolder.c_str();//string to char
	if (_access(t0, 0) == -1)//确定文件或文件夹的访问权限
		_mkdir(t0);//创建文件夹

	vector<vector<Rect>> Rect_table0 = getRect(input_file0, PicLen);//取得所有方式一检测的heads参数
	vector<vector<Rect>> Rect_table1 = getRect(input_file1, PicLen);//取得所有benchmark检测的bench_heads_detect参数
	vector<vector<Rect>> Rect_table2 = getRect(input_file2, PicLen);//取得所有benchmark检测的bench_object_maker参数

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
			if (truepositive0[k][j] == 0){//方式一误检
				Rect rect = Rect_table0[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 0, 255), 2, CV_AA, 0);
			}
			else{//方式一检测正确的heads
				Rect rect = Rect_table0[k][j];
				rect = setRectInMat(img, rect);

				rectangle(copyMat, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0), 2, CV_AA, 0);
			}
		}

		for (int j = 0; j < obj_num1; j++)//bench_heads_detect，实际是方式一漏检
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

		for (int j = 0; j<obj_num2; j++)//bench_heads_detect，实际是方式一漏检
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
						break;//找到子串就退出循环
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