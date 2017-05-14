/**
 * @file realces.h
 * 
 * @brief Arquivo que contém a biblioteca de realces.cc
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

/**
 * @brief Filtro de realce exponencial
 * 
 * Aplicação do filtro de realce exponencial do constraste de imagens.
 *
 * @param img - Imagem a ser filtrada
 * @param fator - Potência a ser usada no filtro
 *
 */
Mat trans_power_law(char *img, double fator);

/**
 * @brief Filtro de histograma
 * 
 * Aplicação do filtro de realce por histograma do constraste de imagens.
 *
 * @param img - Imagem a ser filtrada
 *
 */
Mat trans_hist(char *img);
