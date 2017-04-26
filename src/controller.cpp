#include <controller.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

controller::void setimgOriginal(Mat imgEntrada){
	this.imgOriginal = imgEntrada;
}

controller::void setimgDim(Mat imgEntrada){
	this.imgDim = imgEntrada;
}

controller::void setimgInt(Mat imgEntrada){
	this.imgInterpolada = imgEntrada;
}

controller::Mat getimInt(){
	return this.imgInterpolada;
}

controller::Mat getimDim(){
	return this.imgDim;
}

controller::Mat getimOriginal(){
	return this.imgOriginal;
}
