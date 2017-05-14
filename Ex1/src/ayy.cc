#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
#include <funcoes.hpp>

using namespace std;
using namespace cv;

//funcao que diminui e interpola a imagem
Mat dec_int(string nomeFoto,int fator){
  stringstream stream;
  string strfator;
  string localFoto = "./img/" + nomeFoto;
  
  Mat imgOri = imread(localFoto, CV_LOAD_IMAGE_COLOR);


  if (!imgOri.data) {
    cout << "No image data" << endl;
    exit(0);//refazer isso aqui, eh feio
  }
 
  stream << fator;
  strfator = stream.str();
  string nomeOriginal = "Original " + strfator;
  namedWindow(nomeOriginal,WINDOW_AUTOSIZE);
  imshow(nomeOriginal,imgOri);

  Mat imgDim(imgOri.rows/fator,imgOri.cols/fator,CV_8UC3,Scalar(0,0,0));
  Mat imgInt(imgOri.rows,imgOri.cols,CV_8UC3,Scalar(0,0,0));

  //diminuicao da imagem
  for(int i = 0; i < imgOri.rows; i+=fator){
    for(int j = 0; j < imgOri.cols; j+=fator){
      for(int c = 0;c < 3; c++){

        imgDim.at<Vec3b>(i/fator,j/fator) = imgOri.at<Vec3b>(i,j); 
      }
    }
  }

  //interpolacao da imagem a partir da foto diminuida
  for(int i = 0; i < imgOri.rows; i+=fator){
    for(int j = 0; j < imgOri.cols; j+=fator){
      for(int cor = 0; cor < 3; cor++){
        for(int d = 0; d < fator; d++){
          for(int c = 0;c < fator; c++){          

            imgInt.at<Vec3b>(i+d,j+c) = imgDim.at<Vec3b>(i/fator,j/fator); 
            
          }
        }
      }
    }
  }

  namedWindow("Dim",WINDOW_AUTOSIZE);
  imshow("Dim",imgDim);

  stream.str(" ");

  stream << fator;
  strfator = stream.str();
  string nomeInt = "interpolada" + strfator;
  namedWindow(nomeInt,WINDOW_AUTOSIZE);
  imshow(nomeInt,imgInt);

  string localInt = "./img/Int" + strfator + nomeFoto;
  string localDim = "./img/Dim" + strfator + nomeFoto;  

  imwrite(localInt,imgInt);
  imwrite(localDim,imgDim);

  waitKey(0);

  destroyAllWindows();
  return imgInt;
}

	Mat imgMenor(imgTest80.rows,imgTest80.cols,CV_LOAD_IMAGE_COLOR,Scalar(0,0,0));

	resize(dec_int("test80.jpg",4),imgMenor,Size(imgTest80.cols,imgTest80.rows),CV_INTER_CUBIC);

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
        const double d_k = std::sqrt(pow(x-cx-(cx+100),2.0) + pow(y-cy-(cy+100),2.0));
        const double d_mk = std::sqrt(pow(x-cx+(cx+0),2.0) + pow(y-cy+(cy+0),2.0));
        if(d==0) // Avoid division by zero
          pf(y,x)[0] = 0;
        else
          //  pf(y,x)[0] = 1.0 / (1.0 + std::pow(d0/d, 2.0*n));
          pf(y,x)[0] *= (1.0/(1+pow((d0/d_k),2.0*n)))*(1.0/(1+pow((d0/d_mk),2.0*n)));
        // Imaginary part
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
  Mat magI = planes[0];

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
  // viewable image form (float between values 0 and 1).

  imshow("Input Image"       , I   );    // Show the result
  imshow("spectrum magnitude", magI);
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
      radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
      tmp.at<float>(i,j) = (float)
          ( 1 / (1 + pow((double) (radius /  D), (double) (2 * n))));
    }
  }

  Mat toMerge[] = {tmp, tmp};
  merge(toMerge, 2, dft_Filter);
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
  cv::dft(complexI, inverseTransform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
  normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
  imshow("Reconstructed", inverseTransform);
  waitKey();

  Mat filter1;
  planes[0].copyTo(filter1);
  Mat filter2;
  planes[1].copyTo(filter2);

  for( int i = 0; i < filter1.rows; ++i)
  {
    for(int u=7;u<15;++u)
    {
      filter1.at<uchar>(i,u)=0;
      filter2.at<uchar>(i,u)=0;
    }

  }

  Mat inverse[] = {filter1,filter2};
  Mat filterspec;
  merge(inverse, 2, filterspec);

  //Mat filtro = bandpass(10,4,39,30,complexI.rows/2,complexI.cols/2);
  //createGaussianHighPassFilter(Size(128, 128), 16.0);

  //Mat inverseTransform;
  dft(filterspec, inverseTransform,cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);

  Mat finalImage;
  inverseTransform.convertTo(finalImage, CV_8U);

  imshow("tentativa",finalImage);
  waitKey(0);

}

int main(){

  /*  cout << "comecando o programa" << endl;
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

  DFTtoIDFT("moire.tif");

  //powerLaw("university.png",0.5);

  /*for(double fator = 2;fator > 0 ; fator -= 0.1){

    powerLaw(nomeFoto,fator);

  }*/

  //edge_imporv(dec_int(nomeFoto,fator));

  destroyAllWindows();


  return 0;
}