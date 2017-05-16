#include <string>

using namespace std;
using namespace cv;


//funcao que gra o filtro notch
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

//funcao que rearranja o espectro transformado
Mat shift(Mat magI){
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

    return magI;
}

//funcao que faz a transformada de Fourier e a inversa
void DFTtoIDFT(string nomeFoto){

  string local = "./img/" + nomeFoto;
  Mat I = imread(local, CV_LOAD_IMAGE_GRAYSCALE);

    if( I.empty()){

        printf("imagem nao encontrada\n");
      exit(0);
    }

    Mat planes[] = {Mat_<float>(I), Mat::zeros(I.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

  split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude

  Mat magI = planes[0];
    
  Mat final(I.rows,I.cols,CV_LOAD_IMAGE_GRAYSCALE,Scalar(0,0,0));
  Mat filtro1 = Filtro(local);

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    magI = shift(magI);

    normalize(magI, magI, 0, 1, CV_MINMAX); 

    /*Mat nova(I.rows,I.cols,CV_8U,Scalar(0,0,0));

    for(int i = 0;i < final.rows; i++){
    for(int j = 0; j < final.cols; j++){
      final.at<uchar>(i,j) = complexI.at<uchar>(i,j) * filtro1.at<uchar>(i,j);
    }
  }

   
    nova = shift(nova); */

    imshow("Imagem Original"       , I   );    // Show the result
    imshow("filtro",filtro1);
    imshow("Espectro Tranformada", magI);

    //calculating the idft
    cv::Mat inverseTransform;
    cv::dft(complexI, inverseTransform,cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
    normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
    //inverseTransform.convertTo(inverseTransform, CV_8U);
    imshow("Inversa", inverseTransform);
    waitKey();


    imwrite("./img/filtro.png",filtro1);
    imwrite("./img/espectroTransformado.png",magI);

}


int main() {
	DFTtoIDFT("moire.tif");
  return 0;
}