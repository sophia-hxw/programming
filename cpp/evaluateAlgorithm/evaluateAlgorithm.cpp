/*****************************************************************************
//Author: sophia
//Date: 2016-06-29
//Email: xinwen618@gmail.com
//Blog: https://sophia-hxw.github.io/
*****************************************************************************
Output: 
	第一个结果是检测出的正确heads数目；
	第二个结果是benchmark的正确heads数目；
	第三个是Xintelligence检测出的正确heads数目；
	第四个和第五个是presicion和recall值。
Usage：
#1，将作为正确人头参数的文档命名为bench_all_faces.txt，将待评价方式产生的人头参数文档命名为algorithm_all_faces.txt。
#2，将上述两个参数文档中涉及的原图放在同一文件夹，命名为：20160524_original；
#3，将可执行文件EvalueAlgorithm.exe和#1两个文档，#2的文件夹放在同一个目录下；
#4，运行程序会在DOS端输出presicion和recall值。
*****************************************************************************/

#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
using namespace cv;

vector<string> PicName;
//vector<vector<Rect> > PicPosition;

void getPicName(string file_name);
vector<vector<Rect> > getRect(string file_name, int PicLen);
void outputindex(vector<vector<int> >& vec);

int main(){
	string OrigFolderName = "20160524_original";
	getPicName(OrigFolderName);
	int PicLen = PicName.size();//Number of all pictures.

	string benchfile = "bench_all_faces.txt";	
	vector<vector<Rect> > bePicPosition = getRect(benchfile, PicLen);
	
	/*
	for (int inde = 0; inde < 10; inde++){
		int lenofout=bePicPosition[inde].size();
		if (lenofout!=0)
			for (int inde0 = 0; inde0 < lenofout; inde0++){
				cout << bePicPosition[inde][inde0] <<endl;
			}
	}*/
	
	string algorithmfile = "algorithm_all_faces.txt";
	vector<vector<Rect> > alPicPosition = getRect(algorithmfile, PicLen);

	vector<vector<int> > truepositive(PicLen);

	for (int ia = 0; ia < PicLen; ia++){
		int blength = bePicPosition[ia].size();//BenchMark
		int alength = alPicPosition[ia].size();//Algorithm

		
		truepositive[ia].resize(blength,0);

		for (int ib = 0; ib < alength; ib++){
			Rect rectalgo = alPicPosition[ia][ib];

			for (int ic = 0; ic < blength; ic++){
				Rect rectbench = bePicPosition[ia][ic];
				Rect interect = rectalgo & rectbench;
				Rect uniorect = rectalgo | rectbench;
				double rationum = (double)interect.area() / (double)uniorect.area();
				//cout << "inter area= " << interect.area() << endl;
				//cout << "union area= " << uniorect.area() << endl;

				if (rationum>0.5)
					truepositive[ia][ic] = 1;
			}
		}
	}

	int tureposi = 0;
	int benchnumber = 0;
	int algorithmnum = 0;

	for (int ja = 0; ja < PicLen; ja++){
		int benlength = bePicPosition[ja].size();
		int alglength = alPicPosition[ja].size();

		benchnumber = benchnumber + benlength;
		algorithmnum = algorithmnum + alglength;

		for (int jb = 0; jb < benlength; jb++){
			if (truepositive[ja][jb] == 1){
				tureposi = tureposi+1;
				//cout << "The " << jb << "-th head in " << ja << "-th pictures is well detected." << endl;
			}
		}
	}

	double precision = (double)tureposi / (double)algorithmnum;
	double recallnum = (double)tureposi / (double)benchnumber;

	cout << "\n";
	cout << "Number of turepositive heads detection: " << tureposi << endl;
	cout << "Total number of exact heads: " << benchnumber << endl;
	cout << "Total number of detect heads: " << algorithmnum << endl;
	cout << "\n";
	cout << "The precision value is: " << precision << endl;
	cout << "The recall value is: " << recallnum << endl;

	system("Pause");
	return 0;
}


void getPicName(string OrigFolderName){
	string orderA = "DIR .\\" + OrigFolderName + "\\*.jpg / B >SmallList.TXT ";
	system(orderA.c_str());
	string OrigFileName="SmallList.TXT";
	ifstream OrigNameIn(OrigFileName);
	while (!OrigNameIn.eof()){
		string cacheNameO;
		getline(OrigNameIn, cacheNameO);
		int len = cacheNameO.size();
		if (len>0){
			string realName = cacheNameO.substr(0, len-1);
			PicName.push_back(realName);
		}
	}

	OrigNameIn.close();
	string order = "del " + OrigFileName;
	system(order.c_str());
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
					string stria = PicName[ia].substr(0, lenstria - 3);
					string::size_type idx = tmp_name.find(stria, 0);
					if (idx != string::npos){
						tmp_pos = ia;
						break;//找到子串就退出循环
					}		
				}
				//cout << Object[tmp_pos].size() << endl;
				myReadFile >> output;
				//cout << "output1: " << output << endl;
				//cout << "tmp_pos: " << tmp_pos << endl;
				int obj_num = atoi(output);
				int obj[4];

				int before_num = Object[tmp_pos].size();
				//int face_num = before_num + obj_num;
				for (int i = 0; i<obj_num; i++){
					//cout << Object[tmp_pos].size()<<endl;
					Object[tmp_pos].resize(before_num+obj_num);
						for (int j = 0; j<4; j++){
							myReadFile >> output;
							int p = atoi(output);
							obj[j] = p;
						}
						Object[tmp_pos][i + before_num] = Rect(obj[0], obj[1], obj[2], obj[3]);
				}
				
			}
		}
	}
	myReadFile.close();
	return(Object);
}


void outputindex(vector<vector<int> >& vec){
	int length1 = vec.size();

	for (int ia = 0; ia < length1; ia++){
		int length2 = vec[ia].size();
		cout << "The " << ia << "-th picture has " << length2 << " faces." << endl;
		for (int ib = 0; ib < length2; ib++){
			cout <<"\t"<< vec[ia][ib];
		}
		cout << endl;
	}
}