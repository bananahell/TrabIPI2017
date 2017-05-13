#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <funcoes.hpp>

using namespace std;
using namespace cv;

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

	/*int fator;
	cout << "Digite o fator" << endl;
	cin >> fator;
	getchar(); 
*/
	geraHisto(nomeFoto);

/*
	dec_int(nomeFoto,fator);

	
	for(double fator = 1;fator > 0 ; fator = fator - 0.1){

		power_law(nomeFoto,fator);

 	}

	geraHisto(nomeFoto);

	edge_imporv(dec_int(nomeFoto,fator));
*/
	destroyAllWindows();


	return 0;
}