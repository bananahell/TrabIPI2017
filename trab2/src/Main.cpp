#include <iostream>
#include <opencv2/opencv.hpp>
#include <funcoes.hpp>

using namespace std;
using namespace cv;

class WatershedSegmenter {
private:
	cv::Mat markers;
public:
	void setMarkers(const cv::Mat& markerImage) {
 // Convert to image of ints
		markerImage.convertTo(markers,CV_32S);
 	}
 	cv::Mat process(const cv::Mat &image) {
 // Apply watershed
 		cv::watershed(image,markers);
 		return markers;
	}
 };

int main(){
	/*Mat img = imread("./img/morf_test.png",CV_LOAD_IMAGE_GRAYSCALE);

	if (!img.data) {
		cout << "Imagem nao encontrada" << endl;
		exit(0);
	}

	Mat matFechada = Fechamento(img);

	Mat resultadobottom = matFechada - img; //Transformada Bottom-hat
	
	threshold(resultadobottom,resultadobottom,50,255,CV_THRESH_BINARY);//binarizacao da imagem

	Mat resultadofinal;
	medianBlur(resultadobottom,resultadofinal,1);//filtragem mediana da imagem

	threshold(resultadofinal,resultadofinal,50,255,CV_THRESH_BINARY_INV);//binarizacao inversa da imagem

	imshow("Original",img);
	imshow("Fundo",matFechada);
	imshow("bottom-hat",resultadobottom);
	imshow("Output Imagem binaria invertida",resultadofinal);
	waitKey(0);
	imwrite("imageFinal.png",resultadofinal);
	imwrite("imageBottomHat.png",resultadobottom);
	imwrite("fundo.png",matFechada);
	*/

/*	Mat img = imread("./img/img_cells.jpg",CV_LOAD_IMAGE_GRAYSCALE);

	Mat SimgX,SimgY,Simg;

	Sobel(img,SimgX,CV_16S,1,0,3,1,0,BORDER_DEFAULT);
	convertScaleAbs(SimgX,SimgX); 

	Sobel(img,SimgY,CV_16S,0,1,3,1,0,BORDER_DEFAULT);
	convertScaleAbs(SimgY,SimgY); 

	addWeighted( SimgX, 0.5,SimgY, 0.5, 0, Simg );

	imshow("Sobel Gradiente",Simg);

	medianBlur(img,img,1);
	
	Mat matFechada = Fechamento(img,2);

//	threshold(img,thimg,50,255,CV_THRESH_BINARY);

	Mat matAbertura = Abertura(img,2);

//	threshold(matAbertura,matAbertura,50,255,CV_THRESH_BINARY_INV);
	threshold(matAbertura,matAbertura,50,255,CV_THRESH_BINARY_INV+CV_THRESH_OTSU);

	Mat nova(img.rows,img.cols,CV_32SC1,Scalar(0,0,0));

	distanceTransform(matAbertura,nova,CV_DIST_L2,3,DIST_LABEL_PIXEL);

	//Mat final = matFechada - img;
	//Mat final = img - matAbertura;

	imshow("original",img);
	imshow("aberta",matAbertura);
	imshow("nova",nova);
	waitKey(0);

	//tentativa com o tutorial http://docs.opencv.org/trunk/d3/db4/tutorial_py_watershed.html
*/

	/*Mat img = imread("./img/img_cells.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	medianBlur(img,img,1);
	Mat bin;

	imshow("original",img);
	waitKey(0);

	threshold(img,bin,50,255,CV_THRESH_BINARY_INV+CV_THRESH_OTSU);
	imshow("bin",bin);
	waitKey(0);

	Mat foreground;
 	Mat element5 (3,3, CV_8U, cv:: Scalar (1));

 	morphologyEx(bin,foreground, MORPH_CLOSE, element5, Point(-1, -1), 1); 

 	imshow("foreground",foreground);
 	waitKey(0);

 	Mat background;

 	dilate(bin,background,element5,Point(-1,-1),4);
 	imshow("background",background);
 	waitKey(0);

 	Mat marker = foreground + background;

 	imshow("marker",marker);
 	waitKey(0);*/

 	Mat img = imread("./img/img_cells.jpg",CV_LOAD_IMAGE_GRAYSCALE);

 	medianBlur(img,img,1);

 	Mat bin;
 	threshold(img,bin,55,255,CV_THRESH_BINARY + CV_THRESH_OTSU);

 	Mat element5 (2,2, CV_8U, cv:: Scalar (1));
 	Mat close;
 	Mat open;
 	Mat fundo;
 	Mat frente;
 	Mat distance;

 	morphologyEx(bin,close, MORPH_CLOSE, element5, Point(-1, -1), 1); 
 	morphologyEx(bin,open, MORPH_OPEN, element5, Point(-1, -1), 1); 

 	dilate(close,fundo,element5);
 	distanceTransform(close,distance,DIST_L2,3);

 	threshold(distance,frente,255,0,CV_THRESH_BINARY);

 	imshow("img",img);
 	imshow("close",close);
 	imshow("open",open);
 	imshow("fundo",fundo);
 	imshow("frente",frente);
 	imshow("distance",distance);
 	waitKey(0);
/*
 	Mat3b src = imread("./img/img_cells.jpg");

    Mat1b gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);

    Mat1b thresh;
    threshold(gray, thresh, 55, 255, THRESH_BINARY + THRESH_OTSU);
    imshow("threshold",thresh);
    waitKey(0);

    // noise removal
    Mat1b kernel = getStructuringElement(MORPH_ELLIPSE, Size(3,3));
    Mat1b opening;
    morphologyEx(thresh, opening, MORPH_OPEN, kernel, Point(-1, -1), 3);
    imshow("open",opening);
    waitKey(0);

    Mat1b kernelb = getStructuringElement(MORPH_ELLIPSE, Size(3,3));
    Mat1b background;
    morphologyEx(thresh, background, MORPH_DILATE, kernelb);
    //background = ~background;
    imshow("background",background);
    waitKey(0);

    // Perform the distance transform algorithm
    Mat1f dist_transform;
    distanceTransform(opening, dist_transform, CV_DIST_L2, 3);

    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist_transform, dist_transform, 0, 1., NORM_MINMAX);

    imshow("dist_transform",dist_transform);
    waitKey(0);
    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    Mat1f dist_thresh;
    threshold(dist_transform, dist_thresh, 0.5, 1., CV_THRESH_BINARY);

    Mat1b dist_8u;
    dist_thresh.convertTo(dist_8u, CV_8U);

    // Find total markers
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // Create the marker image for the watershed algorithm
    Mat1i markers(dist_thresh.rows, dist_thresh.cols, int(1));

    // Background as 1
    Mat1i one(markers.rows, markers.cols, int(0));
    bitwise_or(one, markers, markers, background);

    // Draw the foreground markers (from 2 up)
    for (int i = 0; i < int(contours.size()); i++)
        drawContours(markers, contours, i, Scalar(i+2), -1);

    // Perform the watershed algorithm
    Mat3b dbg;
    cvtColor(opening, dbg, COLOR_GRAY2BGR);
    watershed(dbg, markers);

    Mat res;
    markers.convertTo(res, CV_8U);
    normalize(res, res, 0, 255, NORM_MINMAX);

    imshow("markers",res);
    waitKey(0);

*/
	return 0;
}