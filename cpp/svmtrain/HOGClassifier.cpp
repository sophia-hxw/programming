//#include "stdafx.h"
#include "HOGClassifier.h"

#define NOMINMAX
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <time.h>
#include <stdlib.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace cv::ml;


HOGClassifier::HOGClassifier()
{
	// Default value
	// ==============
	// Size(128,64), //winSize
	// Size(16,16), //blocksize
	// Size(8,8), //blockStride,
	// Size(8,8), //cellSize,
	// 9, //nbins,
	// 0, //derivAper,
	// -1, //winSigma,
	// 0, //histogramNormType,
	// 0.2, //L2HysThresh,
	// 0 //gammal correction,
	// //nlevels=64

	win_width = 48;
	win_height = 48;
	hog.winSize = Size(win_height,win_width);
	NUM_ATTR_PER_SAMPLE = (win_height/8-1)*(win_width/8-1)*(2*2)*9;

	// SVM

	//params.svm_type = CvSVM::C_SVC;
	//params.kernel_type = CvSVM::LINEAR;
	//params.C = 10;

	svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::LINEAR);
	//svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->setC(10);

}

HOGClassifier::~HOGClassifier()
{

}

int HOGClassifier::ingestByVehicleClass(string dirname, int id)
{
		return ingestByVehicleClassForExp(dirname,id,1);
}

int HOGClassifier::ingestByVehicleClassForExp(string dirname, int id, double trainingFaction)
{
	srand(time(NULL));
	int trainingFactionInt = cvRound(100*trainingFaction);

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	char fullpath[260];
	char ch[260];

	//As much as we'd love to, we can't use memcpy() because
	//sizeof(TCHAR)==sizeof(char) may not be true:
	szDir[dirname.size()]=0;
	copy(dirname.begin(),dirname.end(),szDir);

	//StringCchCopy(szDir, MAX_PATH, dirname.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*jpg"));

	//_tprintf(TEXT("szDir = %s\n"),szDir);

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		_tprintf(TEXT("Error 1 in FindFirstFile\n"));
		return -1;
	} 

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			char DefChar = ' ';			
			WideCharToMultiByte(CP_ACP,0,ffd.cFileName,-1, ch,260,&DefChar, NULL);

			sprintf(fullpath,"%s\\%s",dirname.c_str(),ch);
			//printf("loading %s\n",fullpath);

			Mat input = imread(string(fullpath),CV_LOAD_IMAGE_GRAYSCALE);
			Mat rinput = Mat(win_height,win_width,input.type());
			resize(input,rinput,rinput.size());

			// 0 - 99
			int r = rand() % 100;

			if (r<trainingFactionInt)
			{
				images.push_back(rinput);
				labels.push_back(id);
			}
			else
			{
				testImages.push_back(rinput);
				testLabels.push_back(id);
			}
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		_tprintf(TEXT("Error 2 in FindFirstFile\n"));
		return -1;
	}

	FindClose(hFind);
	return 0;
}

int HOGClassifier::predict(Mat imagePatch)
{
	Mat greyMat;

	if (imagePatch.channels() > 1)
	{
		cv::cvtColor(imagePatch, greyMat, CV_BGR2GRAY);
	}
	else
	{
		greyMat = imagePatch;
	}
	Mat rinput = Mat(win_height,win_width,greyMat.type());
	resize(greyMat,rinput,rinput.size());

	vector<float> descriptorsValues;
	vector<Point> locations;

	hog.compute(rinput, descriptorsValues, Size(0,0), Size(0,0), locations);

	Mat inputFeature = Mat(1, NUM_ATTR_PER_SAMPLE, CV_32FC1);

	for (int c = 0; c < descriptorsValues.size(); c++) 
	{
		inputFeature.at<float>(0, c) = descriptorsValues[c];			
	}

	return svmtest(inputFeature);
}

void HOGClassifier::saveRecognizer(const string& filename)
{
	//svm.save(filename.c_str());
	svm->save(filename.c_str());
}

void HOGClassifier::loadRecognizer(const string& filename)
{
	//svm.load(filename.c_str());
//	svm->load(filename.c_str());
}


void HOGClassifier::trainRBF()
{

	//params.svm_type = CvSVM::C_SVC;
	//params.kernel_type = CvSVM::RBF;
	//params.gamma = 1;
	//params.C = 10;
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::RBF);
	svm->setC(10);


	training_data = Mat(images.size(), NUM_ATTR_PER_SAMPLE, CV_32FC1);
	training_labels = Mat(images.size(), 1, CV_32SC1);

	for (int i = 0; i < images.size(); i++)
	{
		vector<float> descriptorsValues;
		vector<Point> locations;

		hog.compute(images[i], descriptorsValues, Size(0,0), Size(0,0), locations);

		if (i == 0)
		{
			printf("descriptorsValues length = %d\n",descriptorsValues.size());
			printf("NUM_ATTR_PER_SAMPLE = %d\n",NUM_ATTR_PER_SAMPLE);
			cvWaitKey(0);
		}


		for (int c = 0; c < descriptorsValues.size(); c++) 
		{
			training_data.at<float>(i, c) = descriptorsValues[c];			
		}

		training_labels.at<int>(i, 0) = labels[i];

	}


	// starting training SVM
    printf("start SVM RBF training...\n");
	//printf("Finding optimal parameters for SVM ...\n");

	// Use auto-training parameter grid search (ignore params manually
	// specified above)
	//svm.train_auto(training_data, training_labels, Mat(), Mat(), params, 10);
	//svm.train(training_data, training_labels, Mat(), Mat(), params);
	svm->train(training_data, ROW_SAMPLE, training_labels);
	//params = svm.get_params();

	//printf("The optimal parameters are: degree = %f, gamma = %f, coef0 = %f, C = %f, nu = %f, p = %f\n",
	//	params.degree, params.gamma, params.coef0, params.C, params.nu, params.p);


	//printf("Number of support vectors in the SVM: %i\n", svm.get_support_vector_count());

}

void HOGClassifier::train()
{
	training_data = Mat(images.size(), NUM_ATTR_PER_SAMPLE, CV_32FC1);
	training_labels = Mat(images.size(), 1, CV_32SC1);

	for (int i = 0; i < images.size(); i++)
	{
		vector<float> descriptorsValues;
		vector<Point> locations;

		hog.compute(images[i], descriptorsValues, Size(0,0), Size(0,0), locations);

		if (i == 0)
		{
			printf("descriptorsValues length = %d\n",descriptorsValues.size());
			printf("NUM_ATTR_PER_SAMPLE = %d\n",NUM_ATTR_PER_SAMPLE);
			cvWaitKey(0);
		}


		for (int c = 0; c < descriptorsValues.size(); c++) 
		{
			training_data.at<float>(i, c) = descriptorsValues[c];			
		}

		training_labels.at<int>(i, 0) = labels[i];

	}


	// starting training SVM
	 printf("start SVM training...\n");
	//printf("Finding optimal parameters for SVM ...\n");

	// Use auto-training parameter grid search (ignore params manually
	// specified above)

//	svm.train_auto(training_data, training_labels, Mat(), Mat(), params, 10);

	//svm.train(training_data, training_labels, Mat(), Mat(), params);
	 svm->train(training_data, ROW_SAMPLE, training_labels);
	//params = svm.get_params();

	/*printf("The optimal parameters are: degree = %f, gamma = %f, coef0 = %f, C = %f, nu = %f, p = %f\n",
		params.degree, params.gamma, params.coef0, params.C, params.nu, params.p);*/


//	printf("Number of support vectors in the SVM: %i\n", svm.get_support_vector_count());

}

void HOGClassifier::testRecognizer()
{
	if (testImages.size() < 1) return;

	testing_data = Mat(testImages.size(), NUM_ATTR_PER_SAMPLE, CV_32FC1);
	testing_labels = Mat(testImages.size(), 1, CV_32FC1);

	for (int i = 0; i < testImages.size(); i++)
	{
		vector<float> descriptorsValues;
		vector<Point> locations;

		hog.compute(testImages[i], descriptorsValues, Size(0,0), Size(0,0), locations);

		for (int c = 0; c < descriptorsValues.size(); c++) 
		{
			testing_data.at<float>(i, c) = descriptorsValues[c];			
		}

		testing_labels.at<float>(i, 0) = testLabels[i];
	}

	

	// zero the false positive counters in a simple loop

	int confusion_matrix[NUM_CLASSES][NUM_CLASSES];

	for (int i = 0; i < NUM_CLASSES; i++)
		for (int j = 0; j < NUM_CLASSES; j++)
			confusion_matrix[i][j] = 0;

	
	for (int i = 0; i < testImages.size(); i++)
	{
		// test
		int result = svmtest(testing_data.row(i));
		if (result > -1)
			confusion_matrix[testLabels[i]][result]++;
		else
			printf("wrong results\n");

	}

	int rowSum[NUM_CLASSES];

	for (int i = 0; i < NUM_CLASSES; i++)
	{
		printf("\n");
		rowSum[i] = 0;
		for (int j = 0; j < NUM_CLASSES; j++)
		{
			printf("%d ",confusion_matrix[i][j]);
			rowSum[i] = rowSum[i] + confusion_matrix[i][j];
		}
	}

	printf("\n normalized scores\n");

	for (int i = 0; i < NUM_CLASSES; i++)
	{
		printf("\n");

		for (int j = 0; j < NUM_CLASSES; j++)
		{
			if (rowSum[i] > 0)
				printf("%f ", (float) confusion_matrix[i][j]/(float)rowSum[i]);
			else
				printf("%d ", confusion_matrix[i][j]);
		}
	}


}


int HOGClassifier::svmtest(const Mat &x)
{
	/*float result = svm.predict(x);

	for (int j = 0; j < NUM_CLASSES ; j++)
	{
		if (fabs(result - j) < FLT_EPSILON)
		{
			return j;
		} 
	}
*/
	return -1;
}