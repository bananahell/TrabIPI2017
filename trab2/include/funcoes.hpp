#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat elementoEstruturante(int opt);

Mat Abertura(Mat img,int opt);

Mat Fechamento(Mat img,int opt);

Mat Erosao(Mat img,int opt);

Mat Dilatacao(Mat img,int opt);
