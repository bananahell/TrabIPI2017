#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <funcoes.hpp>

using namespace std;
using namespace cv;

void fourier(string nomeFoto){

	string local = "./img/" + nomeFoto;
	 Mat I = imread(local, CV_LOAD_IMAGE_GRAYSCALE);
    
    if( I.empty()){
    	printf("Imagem nao encontrada\n");
    	exit(0);
    }
       
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    imshow("Input Image"       , I   );    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey();
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
	/*string nomeFoto;
	cout << "Digite o nome da imagem" << endl;
	cin >> nomeFoto;
	getchar(); */
/*
	int fator;
	cout << "Digite o fator" << endl;
	cin >> fator;
	getchar(); 
*/
	//dec_int(nomeFoto,fator);
	//edge_imporv(nomeFoto);

	//geraHisto("university.png");
	//geraGraficoHist(nomeFoto);
	
	//string nomeHistFoto = "hist" + nomeFoto;
	
	//geraGraficoHist("university.png");
	//geraGraficoHist("histuniversity.png");
	//dec_int(nomeFoto,fator);

	fourier("moire.tif");

	//powerLaw("university.png",0.5);

	/*for(double fator = 2;fator > 0 ; fator -= 0.1){

		powerLaw(nomeFoto,fator);

 	}*/

	//edge_imporv(dec_int(nomeFoto,fator));

	destroyAllWindows();


	return 0;
}