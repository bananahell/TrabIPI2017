/**
 * @file dec_int.cc
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <dec_int.h>


void dec_int(char *img_entrada,  int fator) {

  Mat imagem_orig;

  imagem_orig = imread(img_entrada, CV_LOAD_IMAGE_COLOR);

  if (!imagem_orig.data) {
    cout << "No image data" << endl;
    return;
  }

  namedWindow("Display Image1", WINDOW_AUTOSIZE);
  imshow("Display Image1", imagem_orig);


  Mat imagem_menor(imagem_orig.rows/fator, imagem_orig.cols/fator, CV_8UC3, Scalar(0,0,0));

  for( int y = 0; y < imagem_orig.rows; y+=fator ) {
    for( int x = 0; x < imagem_orig.cols; x+=fator ) {
      for (int c = 0; c < 3; ++c) {
        imagem_menor.at<Vec3b>(y/fator, x/fator) = imagem_orig.at<Vec3b>(y, x);
      }
    }
  }

  namedWindow("Display Image2", WINDOW_AUTOSIZE);
  imshow("Display Image2", imagem_menor);

  Mat imagem_result(imagem_menor.rows*fator, imagem_menor.cols*fator, CV_8UC3, Scalar(0,0,0));

  for (int y = 0; y < imagem_menor.rows; ++y ) {
    for( int x = 0; x < imagem_menor.cols; ++x ) {
      for (int c = 0; c < 3; ++c) {
        imagem_result.at<Vec3b>(y*fator, x*fator) = imagem_menor.at<Vec3b>(y, x);
        for (int rows_vazias = 0; rows_vazias < fator; ++rows_vazias) {
          for (int cols_vazias = 0; cols_vazias < fator; ++cols_vazias) {
            imagem_result.at<Vec3b>((y*fator) + rows_vazias, (x*fator) + cols_vazias) = imagem_result.at<Vec3b>(y*fator, x*fator);
          }
        }
      }
    }
  }

  namedWindow("Display Image3", WINDOW_AUTOSIZE);
  imshow("Display Image3", imagem_result);


  waitKey(0);
}