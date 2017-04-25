/**
 * @file trabalho.cc
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

void dec_int(){
  
  int fator = 0;
  Mat image;

  string args;

  cout << "Digite o endereco" << endl;
  cin >> args;

  string img_entrada = "./img/" + args;
  image = imread(img_entrada, CV_LOAD_IMAGE_COLOR);

  if (!image.data) {
    cout << "No image data" << endl;
    exit(0);
  }

  cout << "Digite o fator:" << endl;
  cin >> fator;
  
  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", image);


  Mat img2(image.rows/fator,image.cols/fator,CV_8UC3,Scalar(0,0,0));
  Mat img3(image.rows,image.cols,CV_8UC3,Scalar(0,0,0));
  
  for (int i = 0; i < image.rows; i+=fator){
    
    for (int j = 0; j < image.cols; j+=fator){
      for(int c = 0;c < 3;c++){

        img2.at<Vec3b>(i/fator,j/fator) = image.at<Vec3b>(i,j); 
      }
     
    }
  }


  for (int i = 0; i < image.rows; i+=fator){
    
    for (int j = 0; j < image.cols; j+=fator){
      for (int cor = 0; cor < 3; cor++){
        for (int d = 0; d < fator; d++){
          for(int c = 0;c < fator ; c++){          

            img3.at<Vec3b>(i+d,j+d) = img2.at<Vec3b>(i/fator,j/fator); 
            img3.at<Vec3b>(i+d,j+c+d) = img2.at<Vec3b>(i/fator,j/fator); 
            img3.at<Vec3b>(i+c+d,j+d) = img2.at<Vec3b>(i/fator,j/fator); 
            img3.at<Vec3b>(i+c+d,j+c+d) = img2.at<Vec3b>(i/fator,j/fator); 
         
          }
        }
      }
    }
  }

  namedWindow("Dim", WINDOW_AUTOSIZE);
  imshow("Dim", img2);

  namedWindow("Interpolada", WINDOW_AUTOSIZE);
  imshow("Interpolada", img3);

}

int main() {

  dec_int();

  waitKey(0);

  return 0;
}
