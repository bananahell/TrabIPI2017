#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <funcoes.hpp>

using namespace std;
using namespace cv;

void geraGrfHisto(Mat fotoHisto,string nomeFoto){

	vector<int> histoEQ;

	for(int i = 0;i < 256; i++){
		histoEQ.push_back(0);
	}

	for(int linha = 0; linha < fotoHisto.rows;linha++){
		for(int coluna = 0;coluna < fotoHisto.cols;coluna++){

			histoEQ.at(fotoHisto.at<uchar>(linha,coluna))++;
		}
	}

	Mat grfHistoEQ(157,257,CV_LOAD_IMAGE_GRAYSCALE,Scalar(255,255,255));
	
	//gerar imagem do grafico
	for(int i = 255; i > 1; i--){
		if(histoEQ.at(i) != 0){
					
			for(int j = (histoEQ.at(i)&80); j > 0 ;j--){
				grfHistoEQ.at<uchar>(155 - j,i) = 0;
			}
		}
	}

	//gera "imagem" no terminal do grafico
	for(int i = 0;i < 256; i++){
		if(histoEQ.at(i) != 0){
			
			if( i >= 100){
				cout << i << " - ";
			}else{
				if(i >= 10){

					cout << i<< "  - ";
				}else{

					cout << i<< "   - ";
				}
			}
					
			for(int j = (histoEQ.at(i)&80); j > 1 ; j--){
				cout << "*";
			}
			cout << endl;
		}
	}

	string nomeWindow = "Histo EQ " + nomeFoto;
	namedWindow(nomeWindow, WINDOW_AUTOSIZE);
	imshow(nomeWindow, grfHistoEQ);

}

void geraHisto(string nomefoto){
	vector<int> Rk;
	vector<double> media;
	vector<double> fdaFoto;
	string localFoto = "./img/" + nomefoto;

	Mat foto = imread(localFoto,CV_LOAD_IMAGE_GRAYSCALE );

	if(!foto.data){
		cout << "imagem nao encontrada" << endl;
		exit(0);
	}

	Mat fotoHisto(foto.rows,foto.cols,CV_LOAD_IMAGE_GRAYSCALE);
	
	for(int i = 0;i < 256; i++){
		Rk.push_back(0);
		media.push_back(0);
		fdaFoto.push_back(0);
	}


	for(int linha = 0; linha < fotoHisto.rows;linha++){
		for(int coluna = 0;coluna < fotoHisto.cols;coluna++){

			Rk.at(foto.at<uchar>(linha,coluna))++;
		}
	}

	for(int i = 0; i < 256; i++){
	    if(i == 0) {
	      media.at(i) = (int)(((Rk.at(i)*255)/foto.total()));
	    }else{
	      media.at(i) = (int)(((Rk.at(i)*255)/foto.total()) + media.at(i-1));
	    }

      fdaFoto.at(i) = (Rk.at(i)/foto.total());
  	}



	for(int linha = 0;linha < foto.rows;linha++){
		for(int coluna = 0; coluna < foto.cols;coluna++){

			fotoHisto.at<uchar>(linha,coluna) = media.at(foto.at<uchar>(linha,coluna));
		}
	}

	cout <<" saiu carai" << endl;

	namedWindow("Original",CV_WINDOW_AUTOSIZE);
	imshow("Original",foto);

	namedWindow("fotohisto",CV_WINDOW_AUTOSIZE);
	imshow("fotohisto",fotoHisto);

	//geracao do grafico do hsitograma para o terminal
	geraGrfHisto(fotoHisto,"Novo");
	geraGrfHisto(foto,"Original");

	for(int i = 0;i < 256; i++){

		if( i >= 100){
			cout << i << " - ";
		}else{
			if(i >= 10){

				cout << i<< "  - ";
			}else{

				cout << i<< "   - ";
			}
		}
		cout << fdaFoto.at(i) << endl;
	}

	waitKey(0);
}

void trans_power_law(string img, double fator) {

	string local ="./img/" + img;

	Mat imagem_nova = imread(local, CV_LOAD_IMAGE_GRAYSCALE);
	double pixel;
	int linha, coluna;
	double corretor = pow(255, fator-1);

	if(!imagem_nova.data){
		cout << "imagem nao encontrada" << endl;
		exit(0);
	}

	namedWindow("Original GrayScale", WINDOW_AUTOSIZE);
	imshow("Original GrayScale", imagem_nova);

	for (linha = 0; linha < imagem_nova.rows; ++linha) {
		for (coluna = 0; coluna < imagem_nova.cols; ++coluna) {

		  pixel = pow(imagem_nova.at<uchar>(linha, coluna), fator)/corretor;

		  imagem_nova.at<uchar>(linha, coluna) = (int)(pixel);

		}
	}

	namedWindow("Power Law", WINDOW_AUTOSIZE);
	imshow("Power Law", imagem_nova);

	waitKey(0);

}

int main(){
	
/*	cout << "comecando o programa" << endl;
	getchar();

	cout << "usando dec_int para imagem test80.jpg" << endl;

	dec_int("test80.jpg",2);
	dec_int("test80.jpg",4);
	dec_int("test80.jpg",8);

	cout << "edge_imporv para imagem ja reduzida" << endl;
	getchar();

  	Mat imgTest80 = imread("./img/test80.jpg",CV_LOAD_IMAGE_COLOR);

	Mat imgMenor(imgTest80.rows,imgTest80.cols,CV_LOAD_IMAGE_COLOR,Scalar(0,0,0));

	resize(dec_int("test80.jpg",4),imgMenor,Size(imgTest80.cols,imgTest80.rows),CV_INTER_CUBIC);

	edge_imporv(imgMenor);
*/

	string nomeFoto;
	cout << "Digite o nome da imagem" << endl;
	cin >> nomeFoto;
	getchar(); 

	int fator;
	cout << "Digite o fator" << endl;
	cin >> fator;
	getchar(); 

	geraHisto(nomeFoto);

/*
	dec_int(nomeFoto,fator);

	
	for(double fator = 1;fator > 0 ; fator = fator - 0.1){

		trans_power_law(nomeFoto,fator);

 	}

	geraHisto(nomeFoto);

	edge_imporv(dec_int(nomeFoto,fator));
*/
	destroyAllWindows();


	return 0;
}