/**
 * @file dec_int.cc
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <realce_power_law.h>


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
  
  vector<int> meu_vetor;
  
  
}
