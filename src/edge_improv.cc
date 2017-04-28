/**
 * @file dec_int.cc
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <edge_improv.h>


void edge_improv(char *img_entrada) {
  
  Mat imagem_borrada;
  imagem_borrada = imread(img_entrada, CV_LOAD_IMAGE_COLOR);

  if (!imagem_borrada.data) {
    cout << "No image data" << endl;
    return;
  }

  Mat imagem_filtrada_custom(imagem_borrada.rows, imagem_borrada.cols, CV_8UC3, Scalar(0,0,0));

  imagem_filtrada_custom = filtro_custom(imagem_borrada, 3);

  imwrite("./img/filtrada.jpg", imagem_filtrada_custom);
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_filtrada_custom);

  waitKey(0);

  imagem_filtrada_custom = filtro_custom(imagem_borrada, 5);

  imwrite("./img/filtrada.jpg", imagem_filtrada_custom);
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_filtrada_custom);

  waitKey(0);

  imagem_filtrada_custom = filtro_custom(imagem_borrada, 7);

  imwrite("./img/filtrada.jpg", imagem_filtrada_custom);
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_filtrada_custom);

  waitKey(0);

  imagem_filtrada_custom = filtro_custom(imagem_borrada, 9);

  imwrite("./img/filtrada.jpg", imagem_filtrada_custom);
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_filtrada_custom);

  waitKey(0);

  imagem_filtrada_custom = filtro_custom(imagem_borrada, 11);

  imwrite("./img/filtrada.jpg", imagem_filtrada_custom);
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_filtrada_custom);

  waitKey(0);

  imagem_filtrada_custom = filtro_custom(imagem_borrada, 2);

  imwrite("./img/filtrada.jpg", imagem_filtrada_custom);
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_filtrada_custom);

  waitKey(0);
  
}

Mat filtro_custom(Mat img, int fator) {

  int row, col;
  int chn;
  double pixel_aux;
  int quant_pixels;
  int i, j;
  bool row_menor_aval, row_maior_aval;
  bool col_menor_aval, col_maior_aval;
  Mat imagem_nova(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));
  if (fator %  2 == 0) {
    return imagem_nova;
  }

  for (row = 0; row < img.rows; ++row) {
    for (col = 0; col < img.cols; ++col) {
      for (chn = 0; chn < 3; ++chn) {

        pixel_aux = 0;
        quant_pixels = 0;

        pixel_aux += img.at<Vec3b>(row, col)[chn];
        quant_pixels++;
        
        row_menor_aval = true;
        row_maior_aval = true;
        for (i = 1; i <= fator/2; ++i) {

          if (row != i-1 && row_menor_aval) {
            pixel_aux += img.at<Vec3b>(row - i, col)[chn];
            quant_pixels++;
            col_menor_aval = true;
            col_maior_aval = true;
            for (j = 1; j <= fator/2; ++j) {
              if (col != j-1 && col_menor_aval) {
                pixel_aux += img.at<Vec3b>(row - i, col - j)[chn];
                quant_pixels++;
              } else {
                col_menor_aval = false;
              }
              if (col != img.cols-j && col_maior_aval) {
                pixel_aux += img.at<Vec3b>(row - i, col + j)[chn];
                quant_pixels++;
              } else {
                col_maior_aval = false;
              }
            }
          } else {
            row_menor_aval = false;
          }
    
          if (row != img.rows-i && row_maior_aval) {
            pixel_aux += img.at<Vec3b>(row + i, col)[chn];
            quant_pixels++;
            col_menor_aval = true;
            col_maior_aval = true;
            for (j = 1; j <= fator/2; ++j) {
              if (col != j-1 && col_menor_aval) {
                pixel_aux += img.at<Vec3b>(row + i, col - j)[chn];
                quant_pixels++;
              } else {
                col_menor_aval = false;
              }
              if (col != img.cols-j && col_maior_aval) {
                pixel_aux += img.at<Vec3b>(row + i, col + j)[chn];
                quant_pixels++;
              } else {
                col_maior_aval = false;
              }
            }
          } else {
            row_maior_aval = false;
          }
          
          if (col != i-1 && col_menor_aval) {
            pixel_aux += img.at<Vec3b>(row, col - i)[chn];
            quant_pixels++;
          } else {
            col_menor_aval = false;
          }
          if (col != img.cols-i && col_maior_aval) {
            pixel_aux += img.at<Vec3b>(row, col + i)[chn];
            quant_pixels++;
          } else {
            col_maior_aval = false;
          }

        }

        imagem_nova.at<Vec3b>(row, col)[chn] = (int)(pixel_aux/quant_pixels);

      }
    }
  }

  return imagem_nova;

}
