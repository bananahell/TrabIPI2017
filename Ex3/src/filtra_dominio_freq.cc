/**
 * @file dec_int.cc
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <filtra_dominio_freq.h>


void filtra_dominio_freq(char *img_entrada) {
  
  Mat imagem_entrada;
  imagem_entrada = imread(img_entrada, CV_LOAD_IMAGE_COLOR);

  if (!imagem_entrada.data) {
    cout << "No image data" << endl;
    return;
  }

  Mat imagem_filtrada_custom(imagem_borrada.rows, imagem_borrada.cols, CV_8UC3, Scalar(0,0,0));

  imwrite("./img/filtrada.jpg", imagem_filtrada_custom);
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_filtrada_custom);

  waitKey(0);
  
}

Mat fourier_trans(Mat imagem_entrada) {

  Mat imagem_trans(imagem_borrada.rows, imagem_borrada.cols, CV_8UC3, Scalar(0,0,0));
  return imagem_trans;
  
}
