#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

#include "HOGClassifier.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	string modelName = argv[1];

	HOGClassifier hog;

	vector<int> types;
	types.push_back(1);
	types.push_back(3);
	types.push_back(4);
	types.push_back(5);
	vector<string> txts;
	txts.push_back("1.txt");  //LVE
	txts.push_back("3.txt");  //LTR
	txts.push_back("4.txt");  //VAN
	txts.push_back("5.txt");  //BUS 

	for(int i=0; i<txts.size(); i++)
	{
		ifstream myfile (txts[i]);
		 string line;
		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				cout << line << '\n';
				hog.ingestByVehicleClass(line,types[i]);
			}
			myfile.close();
		}

	}

//	hog.train();
	hog.trainRBF();
	hog.saveRecognizer(modelName);

	return 1;
}