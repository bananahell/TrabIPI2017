#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat dec_int(string nomeFoto,int fator);

void edge_imporv(string nomearq);

void edge_imporv(Mat imgEntrada);

void geraGrfHisto(Mat fotoHisto,string nomeFoto);

void geraHisto(string nomefoto);

void powerLaw(string img, double fator);

void geraGraficoHist(string nomeFoto);

void DFTtoIDFT(string nomeFoto);

Mat shift(Mat magI);

Mat Filtro(string local);