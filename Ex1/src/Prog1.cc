/**
 * @file trabalho.cc
 * 
 * @brief Arquivo do primeiro exercício do primeiro trabalho de IPI UnB - 1/2017
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <dec_int.h>
#include <edge_improv.h>


/**
 * @brief Main do programa.
 *
 */
int main() {

  string img_entrada = "./img/test80.jpg";
  string img_quadriculada2 = "./img/ex1/test80_quadriculada_2.jpg";
  string img_quadriculada4 = "./img/ex1/test80_quadriculada_4.jpg";
  string img_quadriculada8 = "./img/ex1/test80_quadriculada_8.jpg";

  dec_int(img_entrada, 2);
  dec_int(img_entrada, 4);
  dec_int(img_entrada, 8);

  edge_improv(img_quadriculada2);
  edge_improv(img_quadriculada4);
  edge_improv(img_quadriculada8);

  Mat img_cvresize;
  img = imread(img_entrada, CV_LOAD_IMAGE_COLOR);

  return 0;

}
