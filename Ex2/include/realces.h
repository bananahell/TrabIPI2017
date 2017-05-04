/**
 * @file dec_int.h
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

Mat trans_power_law(char *img, double fator);

Mat trans_hist(char *img);
