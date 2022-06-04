#ifndef HOGCLASSIFIER_H
#define HOGCLASSIFIER_H


#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
//#include <opencv2/contrib/contrib.hpp>
//#include <opencv/ml.h>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect.hpp>

using namespace std;


class HOGClassifier
{


	cv::HOGDescriptor hog;
	cv::Mat testing_data;
	cv::Mat training_data;
	cv::Mat testing_labels;
	cv::Mat training_labels;

	int NUM_ATTR_PER_SAMPLE;

	int win_width;
	int win_height;

	//CvSVMParams params;
	//CvSVM svm;

	cv::Ptr<cv::ml::SVM> svm;
	static const int NUM_CLASSES = 6;

public:
	HOGClassifier();
	~HOGClassifier();
	int ingestByVehicleClass(string dirname, int id);
	int ingestByVehicleClassForExp(string dirname, int id, double trainingFaction);
	int predict(cv::Mat imagePatch);
	void saveRecognizer(const string& filename);
	void loadRecognizer(const string& filename);
	void train();
	void trainRBF();
	void testRecognizer();

	vector<cv::Mat> images;
	vector<int> labels;

	vector<cv::Mat> testImages;
	vector<int> testLabels;

protected:
private:
	int svmtest(const cv::Mat &x);
};


#endif