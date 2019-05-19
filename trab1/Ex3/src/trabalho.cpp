/**
 * @file trabalho.cc
 *
 * @brief Arquivo que contém a implementação do filtro de Butterworth
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <cmath>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

/**
 * @brief Filtro de Butterworth
 *
 * Aplicação do filtro de Butterworth no espectro da frequência da imagem.
 *
 * @param local - Nome da imagem a ser analisada
 *
 */
cv::Mat Filtro(std::string local) {
  int arrD[] = {10, 10, 5, 5};
  std::vector<int> D(arrD, arrD + sizeof(arrD) / sizeof(arrD[0]));

  int arrUk[] = {39, -39, 78, -78};
  std::vector<int> Uk(arrUk, arrUk + sizeof(arrUk) / sizeof(arrUk[0]));

  int arrVk[] = {30, 30, 30, 30};
  std::vector<int> Vk(arrVk, arrVk + sizeof(arrVk) / sizeof(arrVk[0]));

  double D1, D2;

  cv::Mat img = cv::imread(local, cv::IMREAD_GRAYSCALE);
  if (!img.data) {
    std::cout << "Imagem " << local << " não encontrada. Retornando com erro."
              << std::endl;
    exit(0);
  }

  cv::Mat filtro(img.rows, img.cols, cv::IMREAD_GRAYSCALE, cv::Scalar(0, 0, 0));

  int linha = img.rows / 2;
  int coluna = img.cols / 2;

  for (int i = 0; i < filtro.rows; i++) {
    for (int j = 0; j < filtro.cols; j++) {
      filtro.at<uchar>(i, j) = 255;
    }
  }

  for (int cont = 0; cont < 4; cont++) {
    for (int i = 0; i < filtro.rows; i++) {
      for (int j = 0; j < filtro.cols; j++) {
        D1 = sqrt(pow(i - linha - Uk.at(cont), 2) +
                  pow(j - coluna - Vk.at(cont), 2));
        D2 = sqrt(pow(i - linha + Uk.at(cont), 2) +
                  pow(j - coluna + Vk.at(cont), 2));

        filtro.at<uchar>(i, j) =
            filtro.at<uchar>(i, j) * (1 / (1 + pow(D.at(cont) / D1, 2 * 4)) *
                                      (1 / (1 + pow(D.at(cont) / D2, 2 * 4))));
      }
    }
  }

  return filtro;
}

/**
 * @brief Função shift da imagem
 *
 * Função que rearranja os quadrantes da imagem para que ela possa ser melhor
 * analisada no espectro da frequência.
 *
 * @param mafI - Imagem analisada
 *
 */
cv::Mat shift(cv::Mat mag_i) {
  mag_i = mag_i(cv::Rect(0, 0, mag_i.cols & -2, mag_i.rows & -2));

  int cx = mag_i.cols / 2;
  int cy = mag_i.rows / 2;

  cv::Mat q0(mag_i, cv::Rect(0, 0, cx, cy));
  cv::Mat q1(mag_i, cv::Rect(cx, 0, cx, cy));
  cv::Mat q2(mag_i, cv::Rect(0, cy, cx, cy));
  cv::Mat q3(mag_i, cv::Rect(cx, cy, cx, cy));

  cv::Mat tmp;
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);

  q1.copyTo(tmp);
  q2.copyTo(q1);
  tmp.copyTo(q2);

  return mag_i;
}

/**
 * @brief Função DFTtoIDFT
 *
 * Função que faz a transformação de Fourier na imagem para obter seu espectro
 * na frequência.
 *
 * @param nome_foto - Nome da foto
 *
 */
void DFTtoIDFT(std::string nome_foto) {
  std::string local = "../img/" + nome_foto;
  cv::Mat I = cv::imread(local, cv::IMREAD_GRAYSCALE);

  if (I.empty()) {
    std::cout << "Imagem " << nome_foto
              << " não encontrada. Retornando com erro." << std::endl;
    exit(0);
  }

  cv::Mat planes[] = {cv::Mat_<float>(I), cv::Mat::zeros(I.size(), CV_32F)};
  cv::Mat complex_i;
  merge(planes, 2, complex_i);

  dft(complex_i, complex_i);

  split(complex_i, planes);
  magnitude(planes[0], planes[1], planes[0]);

  cv::Mat mag_i = planes[0];

  cv::Mat final(I.rows, I.cols, cv::IMREAD_GRAYSCALE, cv::Scalar(0, 0, 0));
  cv::Mat filtro1 = Filtro(local);

  mag_i += cv::Scalar::all(1);
  log(mag_i, mag_i);

  mag_i = shift(mag_i);

  normalize(mag_i, mag_i, 0, 1, cv::NORM_MINMAX);

  imshow("Imagem Original", I);
  imshow("filtro", filtro1);
  imshow("Espectro Tranformada", mag_i);

  cv::Mat inverse_transform;
  cv::dft(complex_i, inverse_transform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
  normalize(inverse_transform, inverse_transform, 0, 1, cv::NORM_MINMAX);

  imshow("Inversa", inverse_transform);
  cv::waitKey();

  imwrite("../img/filtro.png", filtro1);
  imwrite("../img/espectroTransformado.png", mag_i);
}

/**
 * @brief Main do programa.
 *
 */
int main() {
  DFTtoIDFT("moire.tif");
  return 0;
}
