#include <iostream>    
#include <fstream>    
#include <string>    
#include <vector> 
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
   
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;    
using namespace std;    

int ImgWidht = 48;
int ImgHeight = 48;

int GetPicNameFromTxt(string filename, vector<string>& img_name);
void computeHOG(int label, int sampleIndex, int bus_total_num, string path, vector<string>& name, Mat& sample_feature_mat, Mat& sample_class_mat);
void testHOG(int label, int total_num, string path, vector<string>& name, Ptr<ml::SVM>& svm, ofstream& predict_txt);

int main()  {	
	string bus_dirname = "D:\\classifierData\\bus_CAT\\";
	string ltr_dirname = "D:\\classifierData\\Lorries_CAT\\";
	string lve_dirname = "D:\\classifierData\\car_CAT\\";
	string van_dirname = "D:\\classifierData\\van_CAT\\";

	string bus_dir_order = "DIR " + bus_dirname + "*.jpg  / B >busFileName.txt";
	system(bus_dir_order.c_str());
	vector<string> bus_name;
	int bus_total_num;
	bus_total_num = GetPicNameFromTxt("busFileName.txt", bus_name);
	bus_total_num--;
	system("del busFileName.txt");

	string car_dir_order = "DIR " + lve_dirname + "*.jpg / B >carFileName.txt";
	system(car_dir_order.c_str());
	vector<string> car_name;
	int car_total_num;
	car_total_num = GetPicNameFromTxt("carFileName.txt", car_name);
	car_total_num--;
	system("del carFileName.txt");

	string lorry_dir_order = "DIR " + ltr_dirname + "*.jpg / B >lorryFileName.txt";
	system(lorry_dir_order.c_str());
	vector<string> lorry_name;
	int lorry_total_num;
	lorry_total_num = GetPicNameFromTxt("lorryFileName.txt", lorry_name);
	lorry_total_num--;
	system("del lorryFileName.txt");

	string van_dir_order = "DIR " + van_dirname + "*.jpg / B >vanFileName.txt";
	system(van_dir_order.c_str());
	vector<string> van_name;
	int van_total_num;
	van_total_num = GetPicNameFromTxt("vanFileName.txt", van_name);
	van_total_num--;
	system("del vanFileName.txt");
	/*
	int num_sample = bus_total_num + car_total_num + lorry_total_num + van_total_num;
	
	Mat sample_feature_mat(num_sample, 900, CV_32FC1);//900=(win_height/8-1)*(win_width/8-1)*(2*2)*9;
	Mat sample_class_mat(num_sample, 1, CV_32SC1);    //样本类别 

	//void computeHOG(int label, int sampleIndex, int bus_total_num, string path, vector<string>& name, Mat& sample_feature_mat, Mat& sample_class_mat)
	computeHOG(5, 0, bus_total_num, bus_dirname, bus_name, sample_feature_mat, sample_class_mat);
	computeHOG(3, bus_total_num, lorry_total_num, ltr_dirname, lorry_name, sample_feature_mat, sample_class_mat);
	computeHOG(1, bus_total_num + lorry_total_num, car_total_num, lve_dirname, car_name, sample_feature_mat, sample_class_mat);
	computeHOG(4, bus_total_num + lorry_total_num + car_total_num, van_total_num, van_dirname, van_name, sample_feature_mat, sample_class_mat);
    
	cout << "*************Starting training*************" << endl;
	Ptr<ml::SVM> svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::RBF);
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON));
	svm->setC(10.0);

	svm->train(sample_feature_mat, ml::ROW_SAMPLE, sample_class_mat);
    svm->save( "SVM_DATA.yml" );  
	cout << "*************Starting end*************" << endl;*/

	//test for the model.
	
	string result_file = "SVM_DATA.yml";
	Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>(result_file.c_str());

	ofstream predict_txt("SVM_PREDICT.txt");
	testHOG(5, bus_total_num, bus_dirname, bus_name, svm, predict_txt);
	testHOG(3, lorry_total_num, ltr_dirname, lorry_name, svm, predict_txt);
	testHOG(1, car_total_num, lve_dirname, car_name, svm, predict_txt);
	testHOG(4, van_total_num, van_dirname, van_name, svm, predict_txt);
    predict_txt.close();
    
	system("Pause");
	return 0;
}  

int GetPicNameFromTxt(string filename, vector<string>& img_name){
	ifstream img_txt(filename);
	int num_img = 0;

	while (!img_txt.eof()){
		string content;
		getline(img_txt, content);
		if (content.size() > 0)
			img_name.push_back(content);

		num_img++;
	}

	img_txt.close();
	return num_img;
}

void computeHOG(int label, int sampleIndex, int total_num, string path, vector<string>& name, Mat& sample_feature_mat, Mat& sample_class_mat){
	//对图片循环 
	for (string::size_type i = 0; i < total_num; i++)
	{
		string img_path = path + name[i];
		//cout << "img_path: " << img_path << endl;
		Mat orig_img = imread(img_path.c_str(), 0);
		if (orig_img.empty()){
			cout << "Can not load the image: " << img_path[i] << endl;
			continue;
		}

		Mat train_img;
		resize(orig_img, train_img, Size(ImgWidht, ImgHeight));

		//申明描述子，每个参数的含义见笔记
		HOGDescriptor hog(Size(ImgWidht, ImgHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9);

		//描述子申请内存并计算
		vector<float> descriptors;
		hog.compute(train_img, descriptors, Size(0, 0), Size(0, 0));

		//每个sample的hog特征个数    
		for (vector<float>::size_type k = 0; k != descriptors.size(); k++)
			sample_feature_mat.at<float>(sampleIndex + i, k) = descriptors[k];

		//类别处理及端口提示信息
		sample_class_mat.at<int>(sampleIndex + i, 0) = label;
	}
}

void testHOG(int label, int total_num, string path, vector<string>& name, Ptr<ml::SVM>& svm, ofstream& predict_txt){
	char line[512];

	for (string::size_type j = 0; j <total_num; j++){
		//读入图像
		string img_path = path + name[j];
		Mat test = imread(img_path.c_str(), 0);

		//要搞成同样的大小才可以检测到
		Mat testImg;
		resize(test, testImg, Size(ImgWidht, ImgHeight));

		//申明描述子，每个参数的含义见笔记
		HOGDescriptor hog(Size(ImgWidht, ImgHeight), Size(16, 16), Size(8, 8), Size(8, 8), 9);

		//结果数组
		vector<float> descriptors;
		//调用计算函数开始计算
		hog.compute(testImg, descriptors, Size(1, 1), Size(0, 0));

		Mat SVMtrainMat = Mat::zeros(1, 900, CV_32FC1); //900=(win_height/8-1)*(win_width/8-1)*(2*2)*9;900=descriptors.size();
		//每个sample的hog特征个数    
		for (vector<float>::size_type k = 0; k != descriptors.size(); k++)
			SVMtrainMat.at<float>(0, k) = descriptors[k];

		int ret = svm->predict(SVMtrainMat);
		sprintf_s(line, "%s\t%d\t%d\r\n", img_path.c_str(), label,ret);
		//printf("%s %d\r\n", img_tst_path[j].c_str(), ret);
		predict_txt << line;
	}
}