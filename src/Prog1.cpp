#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <funcoes.hpp>

using namespace std;
using namespace cv;


void geraHisto(string nomefoto){
	int Rk[255];
	vector<float> media;
	string localFoto = "./img/" + nomefoto;

	Mat foto = imread(localFoto,CV_LOAD_IMAGE_GRAYSCALE );

	if(!foto.data){
		cout << "imagem nao encontrada" << endl;
		exit(0);
	}


	for(int i = 0;i <= 255; i++){
		Rk[i] = 0;
	}

	Mat fotoHisto(foto.rows,foto.cols,CV_LOAD_IMAGE_GRAYSCALE ,Scalar(0,0,0));

	for(int i = 0;i < foto.rows;i++){
		for(int j = 0; j < foto.cols;j++){
			Rk[foto.at<uchar>(i,j)] += 1;
		}
	}

	for(int i = 0;i < 255;i++){
		if(Rk[i] != 0){

		media.push_back(Rk[i]/(foto.rows*foto.cols));
			
		//	cout << i << " - " << media[i] << " - " << Rk[i] << endl;

		}
	}

	int mediaAux = 0;

	for(int linha = 0;linha < foto.rows;linha++){
		for(int coluna = 0; coluna < foto.cols;coluna++){

			mediaAux = 0;
			for(size_t i = 0;i < media.size();i++){
				if(Rk[i] != 0){

					for(int j = 0;j < 255;j++){

						mediaAux += media[i]*Rk[j];  
					}
				}
			}
/*			cout << mediaAux << endl;
*/			fotoHisto.at<uchar>(linha,coluna) = (int) mediaAux;
		}
	}

	cout <<" saiu carai" << endl;

	namedWindow("Original",CV_WINDOW_AUTOSIZE);
	imshow("Original",foto);

	namedWindow("fotohisto",CV_WINDOW_AUTOSIZE);
	imshow("fotohisto",fotoHisto);

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

	geraHisto(nomeFoto);

	destroyAllWindows();


	return 0;
}