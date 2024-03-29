/**
 * @file dec_int.h
 *
 * @brief Arquivo que contém a biblioteca de edge_improv.cc
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

/**
 * @brief Interface para o usuário ter acesso às ferramentas de melhoria
 * subjetiva de uma imagem
 *
 * Somente contém as opções de filtros que o usuário pode utilizar na imagem.
 * Sujeito a expansão.
 *
 * @param img_entrada - Imagem a ser manipulada
 *
 */
void edge_improv(std::string img_entrada);

/**
 * @brief Filtro de média
 *
 * Aplicação do filtro de média de imagens.
 *
 * @param img - Imagem a ser filtrada
 * @param fator - Dimensões do kernel quadrado a ser utilizado na filtragem
 *
 */
void filtro_media(std::string img_entrada, int fator);
