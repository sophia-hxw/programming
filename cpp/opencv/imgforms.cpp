/* 	
���ܣ�ͼ���ʽ��ת��
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <string>

using namespace std;

int main()
{
	IplImage* src;
	
	string filename =  "lena.jpg" ;
	string formatfile0 = "lena_format.bmp";
	string formatfile1 = "lena_format.tif";
	string formatfile2 = "lena_format.png";
	string formatfile3 = "lena_format.ppm";

	if ((strstr(filename.c_str(), ".jpg") == NULL
		&& strstr(filename.c_str(), ".bmp") == NULL
		&& strstr(filename.c_str(), ".tif") == NULL
		&& strstr(filename.c_str(), ".png") == NULL
		&& strstr(filename.c_str(), ".ppm") == NULL)
		|| (strstr(formatfile0.c_str(), ".bmp") == NULL
		&& strstr(formatfile1.c_str(), ".tif") == NULL
		&& strstr(formatfile2.c_str(), ".png") == NULL
		&& strstr(formatfile3.c_str(), ".ppm") == NULL))
	{
		printf("WARNING: CONV only support JPG,BMP,TIF,PPM,TGA and PPM\n");
	}
	else {
		if ((src = cvLoadImage(filename.c_str(), -1)) != 0) {
			cvSaveImage(formatfile0.c_str(), src);
			cvSaveImage(formatfile1.c_str(), src);
			cvSaveImage(formatfile2.c_str(), src);
			cvSaveImage(formatfile3.c_str(), src);
			cvReleaseImage(&src);
			printf("\n Convert successfully.\n");
		}
		else
		{
			printf("\n*** Read or write image fails *** \n");
		}
	}
	system("Pause");
	return 0;
}