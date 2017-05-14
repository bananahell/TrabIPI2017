/**
 * @file dec_int.h
 * 
 * @brief Arquivo que contém a biblioteca de dec_int.cc
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 * @brief Quadriculador de imagens
 * 
 * Multiplica as dimensões de cada pixel de uma imagem por um fator.
 *
 * @param img_entrada - Imagem a ser expandida
 * @param fator - Fator usado para multiplicar as dimensões dos pixels
 *
 */
void dec_int(string img_entrada, int fator);
