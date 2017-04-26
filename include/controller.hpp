#include <opencv2/opencv.hpp>
using namespace cv;

class controller{
public:
	controller();

	void setimgOriginal(Mat imgEntrada);
	void setimgDim(Mat imgEntrada);
	void setimgInt(Mat imgEntrada);
	Mat getimgOriginal();
	Mat getimgDim();
	Mat getimgInterpolada();

private:
	Mat imgOriginal;
	Mat imgDim;
	Mat imgInterpolada;
};