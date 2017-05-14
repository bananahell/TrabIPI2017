/**
 * @file realces.cc
 * 
 * @brief Arquivo que contém os filtros de realce do contraste de imagens
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <realces.h>


Mat trans_power_law(char *img, double fator) {

  Mat imagem_nova = imread(img, CV_LOAD_IMAGE_GRAYSCALE);
  double pixel;
  int row, col;
  double corretor = pow(255, fator-1);

  namedWindow("Original GrayScale", WINDOW_AUTOSIZE);
  imshow("Original GrayScale", imagem_nova);

  // Os três for's que iteram os pixels da imagem recebida
  for (row = 0; row < imagem_nova.rows; ++row) {
    for (col = 0; col < imagem_nova.cols; ++col) {

      pixel = pow(imagem_nova.at<uchar>(row, col), fator)/corretor;

      imagem_nova.at<uchar>(row, col) = (int)(pixel);

    }
  }

  namedWindow("Power Law", WINDOW_AUTOSIZE);
  imshow("Power Law", imagem_nova);

  waitKey(0);

  return imagem_nova;

}

Mat trans_hist(char *img) {

  Mat imagem_nova = imread(img, CV_LOAD_IMAGE_GRAYSCALE);

  namedWindow("Original GrayScale", WINDOW_AUTOSIZE);
  imshow("Original GrayScale", imagem_nova);

  vector<double> canais;
  vector<int> porcents;
  int row, col, i;
  for (i = 0; i < 256; ++i) {
    canais.push_back(0);
    porcents.push_back(0);
  }
  
  for (row = 0; row < imagem_nova.rows; ++row) {
    for (col = 0; col < imagem_nova.cols; ++col) {
      ++canais.at(imagem_nova.at<uchar>(row, col));
    }
  }
  
  for (i = 0; i < 256; ++i) {
    if (i != 0) {
      porcents.at(i) = (int)(((canais.at(i)*255)/(imagem_nova.rows * imagem_nova.cols)) + porcents.at(i-1));
    } else {
      porcents.at(i) = (int)(((canais.at(i)*255)/(imagem_nova.rows * imagem_nova.cols)));
    }
  }
  
  for (row = 0; row < imagem_nova.rows; ++row) {
    for (col = 0; col < imagem_nova.cols; ++col) {
      imagem_nova.at<uchar>(row,col) = porcents.at(imagem_nova.at<uchar>(row,col));
    }
  }

  namedWindow("Histogramada", WINDOW_AUTOSIZE);
  imshow("Histogramada", imagem_nova);

  waitKey(0);

  return imagem_nova;
  
}
