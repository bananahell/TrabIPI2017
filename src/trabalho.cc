/**
 * @file trabalho.cc
 * 
 *
 * @author João Pedro de Salles Braga- 13/0143049
 */

#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string>
//#include <controller.hpp>

using namespace std;
using namespace cv;


void dec_int(string nomeFoto,int fator){
  
  string localFoto = "./img/" + nomeFoto;
  
  Mat image = imread(localFoto, CV_LOAD_IMAGE_COLOR);


  if (!image.data) {
    cout << "No image data" << endl;
    exit(0);//refazer isso aqui, eh feio
  }
 
  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", image);


  Mat img2(image.rows/fator,image.cols/fator,CV_8UC3,Scalar(0,0,0));
  Mat img3(image.rows,image.cols,CV_8UC3,Scalar(0,0,0));

  //diminuicao da imagem
  for(int i = 0; i < image.rows; i+=fator){
    for(int j = 0; j < image.cols; j+=fator){
      for(int c = 0;c < 3;c++){

        img2.at<Vec3b>(i/fator,j/fator) = image.at<Vec3b>(i,j); 
      }
     
    }
  }


  //interpolacao da imagem a partir da foto diminuida
  for(int i = 0; i < image.rows; i+=fator){
    for(int j = 0; j < image.cols; j+=fator){
      for(int cor = 0; cor < 3; cor++){
        for(int d = 0; d < fator; d++){
          for(int c = 0;c < fator ; c++){          

            img3.at<Vec3b>(i+d,j+d)     = img2.at<Vec3b>(i/fator,j/fator); 
            img3.at<Vec3b>(i+d,j+c+d)   = img2.at<Vec3b>(i/fator,j/fator); 
            img3.at<Vec3b>(i+c+d,j+d)   = img2.at<Vec3b>(i/fator,j/fator); 
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


  image.release();
  img2.release();
  img3.release();
 
  waitKey(0);
}

int main() {
  int opt;
  string nomeFoto;
  int fator = 0;

  do{

    cout << "+------------------------------------+" << endl
         << "| Trabalho de IPI                    |" << endl
         << "|                                    |" << endl
         << "| Aluno : João Pedro de Salles Braga |" << endl
         << "|                                    |" << endl
         << "|Escolha uma opcao:                  |" << endl
         << "|1-Diminuir imagem/Interpolar        |" << endl
         << "|2-resto do projeto                  |" << endl
         << "|3-sair                              |" << endl
         << "+------------------------------------+" << endl;

    cin >> opt;

    switch(opt){
      case 1:

        cout << "Digite o endereco" << endl;
        cin >> nomeFoto;

        cout << "Digite o fator:" << endl;
        cin >> fator;

        dec_int(nomeFoto,fator);

        destroyAllWindows();      
      break;
      case 2:
        cout << "Em construcao"<< endl;


        dec_int("test80.jpg",2);
        dec_int("test80.jpg",4);
        dec_int("test80.jpg",8);
        dec_int("test80.jpg",16);

      break;
      case 3:
        cout << "Tchau!" << endl;
      break;
      default:
        cout << "Opcao invalida"<< endl;
      break;
    }
    system("clear");

   }while(opt != 3);

  return 0;
}
