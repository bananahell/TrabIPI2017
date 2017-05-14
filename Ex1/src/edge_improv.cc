/**
 * @file edge_improv.cc
 * 
 * @brief Arquivo que contém os filtros de melhoramento subjetivo de imagens e que os aplica
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <edge_improv.h>


void edge_improv(string img_entrada) {

  filtro_media(img_entrada, 5);

}

void filtro_media(string img_entrada, int fator) {

  if (fator %  2 == 0) {
    cout << "Fator de filtro de média da imagem deve ser ímpar. Retornando com erro." << endl;
    return;
  }

  Mat img;
  img = imread(img_entrada, CV_LOAD_IMAGE_COLOR);

  if (!img.data) {
    cout << "Imagem " << img_entrada << " não encontrada. Retornando com erro." << endl;
    return;
  }

  stringstream stream;
  stream << fator;
  string fator_string = stream.str();

  int row, col;
  int chn;
  double pixel_aux;
  int quant_pixels;
  int i, j;
  bool row_menor_aval, row_maior_aval;
  bool col_menor_aval, col_maior_aval;
  Mat imagem_nova(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));

  // Os três for's que iteram os pixels da imagem recebida
  for (row = 0; row < img.rows; ++row) {
    for (col = 0; col < img.cols; ++col) {
      for (chn = 0; chn < 3; ++chn) {

        pixel_aux = 0;
        quant_pixels = 0;

        pixel_aux += img.at<Vec3b>(row, col)[chn];
        quant_pixels++;

        row_menor_aval = true;
        row_maior_aval = true;
        // Iteração no "kernel" de filtragem (de 1 até a metade do fator passado, indo do centro do kernel até uma ponta - metade do kernel)
        for (i = 1; i <= fator/2; ++i) {

          // Dentro desse for, todos os if's são usados para avaliar se a kernel não vai tentar achar pixel fora da imagem
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

  string img_filtrada = img_entrada;
  img_filtrada.insert(img_filtrada.find_last_of('.'), "_filtro_media_");
  img_filtrada.insert(img_filtrada.find_last_of('.'), fator_string);
  imwrite(img_filtrada, imagem_nova);
  cout << img_filtrada << endl;
  namedWindow("Filtrada", WINDOW_AUTOSIZE);
  imshow("Filtrada", imagem_nova);

  waitKey(0);
  destroyAllWindows();

}
