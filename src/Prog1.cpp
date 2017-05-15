#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <funcoes.hpp>
#include <cmath>

using namespace std;
using namespace cv;



double pixelDistance(double u, double v)
{
    return cv::sqrt(u*u + v*v);
}

double gaussianCoeff(double u, double v, double d0)
{
    double d = pixelDistance(u, v);
    return 1.0 - cv::exp((-d*d) / (2*d0*d0));
}

cv::Mat createGaussianHighPassFilter(cv::Size size, double cutoffInPixels)
{
    Mat ghpf(size, CV_64F);

    cv::Point center(size.width / 2, size.height / 2);

    for(int u = 0; u < ghpf.rows; u++)
    {
        for(int v = 0; v < ghpf.cols; v++)
        {
            ghpf.at<double>(u, v) = gaussianCoeff(u - center.y, v - center.x, cutoffInPixels);
        }
    }

    return ghpf;
}

cv::Mat bandpass(double d0, double n, int wy, int wx, int cx, int cy)
{
    cv::Mat_<cv::Vec2f> pf(wy, wx);
    for(int y = 0; y < wy; ++y) {
        for(int x = 0; x < wx; ++x) {
            // Real part
            for(int i = 0; i < 3 ; i++)
            {
                const double d = std::sqrt( double((x-cx)*(x-cx)) + double((y-cy)*(y-cy)) );
                const double d_k = std::sqrt(std::pow(x-cx-(cx+100),2.0) + std::pow(y-cy-(cy+100),2.0));
                const double d_mk = std::sqrt(std::pow(x-cx+(cx+0),2.0) + std::pow(y-cy+(cy+0),2.0));
                if(d==0) // Avoid division by zero
                    pf(y,x)[0] = 0;
                else
                    //  pf(y,x)[0] = 1.0 / (1.0 + std::std::pow(d0/d, 2.0*n));
                    pf(y,x)[0] *= (1.0/(1+std::pow((d0/d_k),2.0*n)))*(1.0/(1+std::pow((d0/d_mk),2.0*n)));
                    // IcomplexInary part
                    pf(y,x)[1] = 0;
            }
        }
    }
    return pf;
}

Mat DFT(string nomeFoto){

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
    complexI = planes[0];

    complexI += Scalar::all(1);                    // switch to logarithmic scale
    log(complexI, complexI);

    // crop the spectrum, if it has an odd number of rows or columns
    complexI = complexI(Rect(0, 0, complexI.cols & -2, complexI.rows & -2));

    // rearrange the quadrants of DFT image  so that the origin is at the image center
    int cx = complexI.cols/2;
    int cy = complexI.rows/2;

    Mat q0(complexI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(complexI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(complexI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(complexI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(complexI, complexI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    imshow("Input Image"       , I   );    // Show the result
    imshow("spectrum magnitude", complexI);
    waitKey();

    return I;
}

void create_butterworth_lowpass_filter(Mat &dft_Filter, int D, int n)
{
	Mat tmp = Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

	Point centre = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
	double radius;

	// based on the forumla in the IP notes (p. 130 of 2009/10 version)
	// see also HIPR2 on-line

	for(int i = 0; i < dft_Filter.rows; i++)
	{
		for(int j = 0; j < dft_Filter.cols; j++)
		{
			radius = (double) sqrt(std::pow((i - centre.x), 2.0) + std::pow((double) (j - centre.y), 2.0));
			tmp.at<float>(i,j) = (float)
						( 1 / (1 + std::pow((double) (radius /  D), (double) (2 * n))));
		}
	}

    Mat toMerge[] = {tmp, tmp};
	merge(toMerge, 2, dft_Filter);
}

Mat notch(int D0,int Uk,int Vk,int p2,int q2){
	int resul;
	cout << Uk << endl;
	
	if(Uk < 0){ Uk = Uk * -1;}

	cout << Uk << endl;
	
	Mat D(Uk*2,Vk*2,CV_LOAD_IMAGE_GRAYSCALE,Scalar(0,0,0));


	for (int i = 0; i < Uk*2; ++i){
		for (int j = 0; j < Uk*2; ++j){
		
			D.at<uchar>(i,j) = resul = sqrt(pow((i-p2-Uk),2) + pow((j-q2-Vk),2));
		}
	}

	imshow("Dk",D);
	waitKey(0);

	return D;
}

Mat Filtro(string local){
	int arrD[] = {10,10,10,10};
	vector<int> D(arrD,arrD + sizeof(arrD)/sizeof(arrD[0]));

	int arrUk[] = {39,-39,78,-78};
	vector<int> Uk(arrUk,arrUk + sizeof(arrUk)/sizeof(arrUk[0]));
	
	int arrVk[] = {30,30,30,30};
	vector<int> Vk(arrVk,arrVk + sizeof(arrVk)/sizeof(arrVk[0]));

	double D1,D2;

	Mat img = imread(local,CV_LOAD_IMAGE_GRAYSCALE);
	if(!img.data){
		cout << "imagem nao encontrada" << endl;
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

	imshow("filtro",filtro);
	imshow("imagem",img);
	waitKey(0);

	for(int cont = 0;cont < 4;cont++){
		for(int i = 0;i < filtro.rows;i++){
			for(int j = 0;j < filtro.cols;j++){
				
				D1 = sqrt(pow(i-linha - Uk.at(cont),2) + pow(j-coluna-Vk.at(cont),2));
				D2 = sqrt(pow(i-linha + Uk.at(cont),2) + pow(j-coluna+Vk.at(cont),2));

				filtro.at<uchar>(i,j) = filtro.at<uchar>(i,j) * (1/(1+pow(D.at(cont)/D1,2*4))
															  * (1/(1+pow(D.at(cont)/D2,2*4))));
				cout << i << "," << j << "-" <<(int)filtro.at<uchar>(i,j) << endl;
			}
		}
	}

	imshow("filtro",filtro);
	waitKey(0);

	return filtro;
}

void DFTtoIDFT(string nomeFoto){

	string local = "./img/" + nomeFoto;
	Mat I = imread(local, CV_LOAD_IMAGE_GRAYSCALE);

    if( I.empty()){

        printf("Erro de Mat\n");
    	exit(0);
    }

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(I), Mat::zeros(I.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    /*complexI = planes[0];



 	for(int i = 0;i < final.rows; i++){
 		for(int j = 0; j < final.cols; j++){
 			final.at<uchar>(i,j) = complexI.at<uchar>(i,j) * filtro1.at<uchar>(i,j);
 		}
 	}*/

 	Mat magI = planes[0];
    
 	Mat final(I.rows,I.cols,CV_LOAD_IMAGE_GRAYSCALE,Scalar(0,0,0));
 	Mat filtro1 = Filtro(local);
    mulSpectrums(magI,filtro1,final,DFT_COMPLEX_OUTPUT);

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of DFT image  so that the origin is at the image center
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
    //normalize(phaseVals, phaseVals, 0, 1, CV_MINMAX);
                                            // viewable image form (float between values 0 and 1).

    imshow("Input Image"       , I   );    // Show the result
    imshow("Spectrum Magnitude", magI);
    waitKey();

    //calculating the idft
    cv::Mat inverseTransform;
    cv::dft(filtro1, inverseTransform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
    normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
    imshow("Reconstructed", inverseTransform);
    waitKey();

    /*imshow("FINal",final);
    waitKey(0);
*/
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

	/*geraHisto("jureg.jpg");
	geraGraficoHist("jureg.jpg");
	geraGraficoHist("histjureg.jpg");
	*/
	//Filtro();

/*	string nomeFoto = "university.png"
	string nomeHistFoto = "hist" + nomeFoto;
	
	geraGrfHisto(nomeFoto);
	geraGrfHisto(nomeHistFoto);
*/	
	//dec_int(nomeFoto,fator);

	DFTtoIDFT("moire.tif");
/*
	Mat top = imread("./img/moire.tif",CV_LOAD_IMAGE_GRAYSCALE);

	Mat Dk1  = notch(10,39,30,top.rows/2,top.cols/2);
	Mat Dnk1 = notch(10,-39,30,top.rows/2,top.cols/2);
	Mat Dk2  = notch(10,-39,30,top.rows/2,top.cols/2);
	Mat Dnk2 = notch(10,39,30,top.rows/2,top.cols/2);
	Mat Dk3  = notch(5,78,30,top.rows/2,top.cols/2);
	Mat Dnk3 = notch(5,-78,30,top.rows/2,top.cols/2);
	Mat Dk4  = notch(5,-78,30,top.rows/2,top.cols/2);
	Mat Dnk4 = notch(5,78,30,top.rows/2,top.cols/2);
*/
	/*Mat Notch;
	mulSpectrums(Dk1,Dnk1,Notch,0,false);
	mulSpectrums(Dk2,Dnk2,Notch,0,false);
	mulSpectrums(Dk3,Dnk3,Notch,0,false);
	mulSpectrums(Dk4,Dnk4,Notch,0,false);

	imshow("notch",Notch);
	waitKey(0);*/
	//std::powerLaw("university.png",0.5);

	/*for(double fator = 2;fator > 0 ; fator -= 0.1){

		std::powerLaw(nomeFoto,fator);

 	}*/

	//edge_imporv(dec_int(nomeFoto,fator));

	destroyAllWindows();


	return 0;
}