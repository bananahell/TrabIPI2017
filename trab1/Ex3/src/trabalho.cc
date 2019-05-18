/**
 * @file trabalho.cc
 * 
 * @brief Arquivo que contém a implementação do filtro de Butterworth
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <cmath>

using namespace std;
using namespace cv;


/**
 * @brief Filtro de Butterworth
 * 
 * Aplicação do filtro de Butterworth no espectro da frequência da imagem.
 *
 * @param local - Nome da imagem a ser analisada
 *
 */
Mat Filtro(string local){
  int arrD[] = {10,10,5,5};
  vector<int> D(arrD,arrD + sizeof(arrD)/sizeof(arrD[0]));

  int arrUk[] = {39,-39,78,-78};
  vector<int> Uk(arrUk,arrUk + sizeof(arrUk)/sizeof(arrUk[0]));

  int arrVk[] = {30,30,30,30};
  vector<int> Vk(arrVk,arrVk + sizeof(arrVk)/sizeof(arrVk[0]));

  double D1,D2;

  Mat img = imread(local,CV_LOAD_IMAGE_GRAYSCALE);
  if(!img.data){
    cout << "Imagem " << local << " não encontrada. Retornando com erro." << endl;
    exit(0);
  }

  Mat filtro(img.rows,img.cols,CV_LOAD_IMAGE_GRAYSCALE,Scalar(0,0,0));

  int linha = img.rows/2;
  int coluna = img.cols/2;

  for(int i = 0;i < filtro.rows; i++){
    for(int j = 0; j < filtro.cols; j++){
      filtro.at<uchar>(i,j) = 255;
    }
  }  

  for(int cont = 0;cont < 4;cont++){
    for(int i = 0;i < filtro.rows;i++){
      for(int j = 0;j < filtro.cols;j++){

        D1 = sqrt(pow(i-linha - Uk.at(cont),2) + pow(j-coluna-Vk.at(cont),2));
        D2 = sqrt(pow(i-linha + Uk.at(cont),2) + pow(j-coluna+Vk.at(cont),2));

        filtro.at<uchar>(i,j) = filtro.at<uchar>(i,j) * (1/(1+pow(D.at(cont)/D1,2*4))
            * (1/(1+pow(D.at(cont)/D2,2*4))));
      }
    }
  }

  return filtro;
}

/**
 * @brief Função shift da imagem
 * 
 * Função que rearranja os quadrantes da imagem para que ela possa ser melhor analisada no espectro da frequência.
 *
 * @param mafI - Imagem analisada
 *
 */
Mat shift(Mat mag_i){
  mag_i = mag_i(Rect(0, 0, mag_i.cols & -2, mag_i.rows & -2));

  int cx = mag_i.cols/2;
  int cy = mag_i.rows/2;

  Mat q0(mag_i, Rect(0, 0, cx, cy));  
  Mat q1(mag_i, Rect(cx, 0, cx, cy)); 
  Mat q2(mag_i, Rect(0, cy, cx, cy)); 
  Mat q3(mag_i, Rect(cx, cy, cx, cy));

  Mat tmp;
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
 * Função que faz a transformação de Fourier na imagem para obter seu espectro na frequência.
 *
 * @param nome_foto - Nome da foto
 *
 */
void DFTtoIDFT(string nome_foto){

  string local = "./img/" + nome_foto;
  Mat I = imread(local, CV_LOAD_IMAGE_GRAYSCALE);

  if( I.empty()){
    cout << "Imagem " << nome_foto << " não encontrada. Retornando com erro." << endl;
    exit(0);
  }

  Mat planes[] = {Mat_<float>(I), Mat::zeros(I.size(), CV_32F)};
  Mat complex_i;
  merge(planes, 2, complex_i);

  dft(complex_i, complex_i);

  split(complex_i, planes);
  magnitude(planes[0], planes[1], planes[0]);

  Mat mag_i = planes[0];

  Mat final(I.rows,I.cols,CV_LOAD_IMAGE_GRAYSCALE,Scalar(0,0,0));
  Mat filtro1 = Filtro(local);

  mag_i += Scalar::all(1);
  log(mag_i, mag_i);

  mag_i = shift(mag_i);

  normalize(mag_i, mag_i, 0, 1, CV_MINMAX);

  imshow("Imagem Original", I);
  imshow("filtro",filtro1);
  imshow("Espectro Tranformada", mag_i);

  cv::Mat inverse_transform;
  cv::dft(complex_i, inverse_transform,cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
  normalize(inverse_transform, inverse_transform, 0, 1, CV_MINMAX);

  imshow("Inversa", inverse_transform);
  waitKey();


  imwrite("./img/filtro.png",filtro1);
  imwrite("./img/espectroTransformado.png",mag_i);

}


/**
 * @brief Main do programa.
 *
 */
int main() {
  DFTtoIDFT("moire.tif");
  return 0;
}