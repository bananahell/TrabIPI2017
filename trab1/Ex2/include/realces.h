/**
 * @file realces.h
 *
 * @brief Arquivo que contém a biblioteca de realces.cc
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>

/**
 * @brief Filtro de realce exponencial
 *
 * Aplicação do filtro de realce exponencial do constraste de imagens.
 *
 * @param img - Imagem a ser filtrada
 * @param fator - Potência a ser usada no filtro
 *
 */
cv::Mat trans_power_law(std::string img, double fator);

/**
 * @brief Filtro de histograma
 *
 * Aplicação do filtro de realce por histograma do constraste de imagens.
 *
 * @param img - Imagem a ser filtrada
 *
 */
cv::Mat trans_hist(std::string img);
