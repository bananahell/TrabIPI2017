#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <funcoes.h>
#include <ctime>
#include <cmath>

using namespace std;
using namespace cv;

int main(){
	
	cout << "comecando o programa" << endl;
	getchar();

	cout << "usando dec_int para imagem test80.jpg" << endl;

	dec_int("./img/test80.jpg",2);
	dec_int("./img/test80.jpg",4);
	dec_int("./img/test80.jpg",8);


	cout << "Filtro para imagem ja reduzida" << endl;
	getchar();

  	Mat imgTest80 = imread(",/img/test80/jpg", CV_LOAD_IMAGE_COLOR);

	Mat imgMenor(imgTest80.rows/4,imgTest80.cols/4,CV_LOAD_IMAGE_COLOR,Scalar(0,0,0));

	resize( imgTest80, imgMenor , Size(imgTest80.rows/4,imgTest80.cols/4));

	filtro(imgMenor);

	return 0;
}