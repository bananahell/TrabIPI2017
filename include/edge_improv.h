/**
 * @file dec_int.h
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void edge_improv(char *img_entrada);

Mat filtro_custom(Mat img, int fator);
