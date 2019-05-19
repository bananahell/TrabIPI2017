/**
 * @file dec_int.h
 *
 * @brief Arquivo que contém a biblioteca de dec_int.cc
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <opencv2/opencv.hpp>

/**
 * @brief Quadriculador de imagens
 *
 * Multiplica as dimensões de cada pixel de uma imagem por um fator.
 *
 * @param img_entrada - Imagem a ser expandida
 * @param fator - Fator usado para multiplicar as dimensões dos pixels
 *
 */
cv::Mat dec_int(std::string img_entrada, int fator);
