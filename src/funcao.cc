#include <funcao.h>

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
      for(int c = 0;c < 3; c++){

        img2.at<Vec3b>(i/fator,j/fator) = image.at<Vec3b>(i,j); 
      }
     
    }
  }

  //interpolacao da imagem a partir da foto diminuida
  for(int i = 0; i < image.rows; i+=fator){
    for(int j = 0; j < image.cols; j+=fator){
      for(int cor = 0; cor < 3; cor++){
        for(int d = 0; d < fator; d++){
          for(int c = 0;c < fator; c++){          

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

  waitKey(0);

  destroyWindow("Dim");
  destroyWindow("Interpolada");
  destroyWindow("Original");

  destroyAllWindows(); 
}