#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat elementoEstruturante(int opt);

cv::Mat Abertura(cv::Mat img, int opt);

cv::Mat Fechamento(cv::Mat img, int opt);

cv::Mat Erosao(cv::Mat img, int opt);

cv::Mat Dilatacao(cv::Mat img, int opt);
