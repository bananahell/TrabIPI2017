#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat dec_int(cv::Mat imgOri, int fator);

void edge_imporv(std::string nomearq);

cv::Mat edge_imporv(cv::Mat imgEntrada);

void geraGrfHisto(cv::Mat fotoHisto, std::string nomeFoto);

void geraHisto(std::string nomefoto);

void powerLaw(std::string img, double fator);

void geraGraficoHist(std::string nomeFoto);

void DFTtoIDFT(std::string nomeFoto);

cv::Mat shift(cv::Mat magI);

cv::Mat Filtro(std::string local);
