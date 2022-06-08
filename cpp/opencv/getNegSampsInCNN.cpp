#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<windows.h>

#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int GetInfoFromTxt(string fileName, vector<string>& TxtFileName);
int GetPicPathFromTxt(string fileName, vector<string>& img_path, vector<int>& roi_num_img, vector<vector<Rect>>& roi_img);
void stringreplaceall(string origpath, string origpicname, string& resultstring, int k);

int main(int argc, char ** argv){
	string totalTxtPath = "E:/NegSampleGet/";
	string totalTxtName = "txtFileName.txt";

	vector<string> TxtFileName;
	string totalFileName = totalTxtPath + totalTxtName;
	int num_total_txt = 0;
	num_total_txt = GetInfoFromTxt(totalFileName, TxtFileName);

	vector<string> img_path;
	vector<int> roi_num_img;
	vector<vector<Rect>> roi_img;

	for (int i = 0; i < num_total_txt; i++){
		cout << "***Start Processing: \t" << TxtFileName[i]<< endl;
			
		int num_img_in_txt = 0;
		num_img_in_txt = GetPicPathFromTxt(TxtFileName[i], img_path, roi_num_img, roi_img);

		cout << "***End Processing:"<< endl;

		size_t posDot = TxtFileName[i].find(".");
		string txtName = TxtFileName[i].substr(0, posDot);
		string mkfolder = "mkdir " + txtName;//有一个存储的绝对路径问题
		size_t pos0 = mkfolder.find("/");
		do{
			mkfolder = mkfolder.replace(pos0, 1, "\\");
			pos0 = mkfolder.find("/");
		} while (pos0 != string::npos);
		//cout << mkfolder << endl;
		system(mkfolder.c_str());

		size_t posf = txtName.find_last_of("/");
		string foldername = txtName.substr(posf + 1, txtName.size());
		//cout << foldername << endl;
	
		for (int j = 0; j < num_img_in_txt; j++){
			Mat img_cache = imread(img_path[j], 1);

			for (int k = 0; k < roi_num_img[j]; k++){
				Mat roi_in_img(img_cache, roi_img[j][k]);
				string new_roi_path;
				string origpath = totalTxtPath + foldername + "/";
				stringreplaceall(origpath, img_path[j], new_roi_path, k);
				imwrite(new_roi_path, roi_in_img);
			}
		}

		img_path.clear();
		roi_num_img.clear();
		roi_img.clear();
	}

	system("Pause");
	return 0;
}

//TxtFileName是txt文件的目录，【返回】txt文件个数
int GetInfoFromTxt(string fileName, vector<string>& TxtFileName){
	ifstream txt_file(fileName);//fileName有绝对路径
	char content[1000];

	int TxtNum = 0;
	while (!txt_file.eof()){
		txt_file >> content;
		string s0 = string(content);
		if (s0.length()>8){
			TxtFileName.push_back(s0);
			TxtNum++;
		}
	}
	txt_file.close();

	cout << "There are " << TxtNum << " \" txt\" files here. " << endl;
	return TxtNum;
}

//img_path是每张图片的完整绝对路径，roi_num_img是每张图片的sample数量，roi_img是每张图片的sample参数，【返回】图片张数
int GetPicPathFromTxt(string fileName, vector<string>& img_path, vector<int>& roi_num_img, vector<vector<Rect>>& roi_img){
	ifstream img_txt(fileName);////fileName有完整绝对路径
	char content[1000];
	int num_img = 0;

	while (!img_txt.eof()){
		img_txt >> content;
		string s0 = string(content);

		if (s0.length()>8){
			num_img++;
			size_t pos1 = s0.find(" ");
			string substr1 = s0.substr(0, pos1);//第二个参数pos1-1可以吗？
			img_path.push_back(substr1);

			img_txt >> content;
			int num = atoi(content);
			roi_num_img.push_back(num);

			vector<Rect> obj_list;
			int obj[4];
			for (int i = 0; i < num; i++){
				for (int j = 0; j < 4; j++){
					img_txt >> content;
					int p = atoi(content);
					obj[j] = p;
				}
				Rect obj_cache = Rect(obj[0], obj[1], obj[2]-5, obj[3]-5);
				obj_list.push_back(obj_cache);
			}
			roi_img.push_back(obj_list);
		}
	}
	img_txt.close();

	return num_img;
}

void stringreplaceall(string origpath, string origpicname, string& resultstring, int k){
	size_t pos0=origpath.find("/");

	do{
		origpath = origpath.replace(pos0, 1, "\\");
		pos0 = origpath.find("/");
	} while (pos0 != string::npos);

	size_t pos2 = origpicname.find_last_of("/");
	size_t pos3 = origpicname.find_first_of(" ");
	string name = origpicname.substr(pos2 + 1, pos3-1);

	stringstream cac;
	cac << k;
	string number;
	cac >> number;

	size_t nameDot = name.find(".");
	string newname = name.substr(0, nameDot) + "-" + number + ".jpg";

	resultstring = origpath + newname;
	//cout << "The path and name that will be given to your ROI_of_image is: " << resultstring << endl;
}