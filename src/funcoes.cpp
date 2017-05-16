#include <funcoes.hpp>
#include <string>

using namespace std;
using namespace cv;

//funcao que gera o grafico da imagem com histograma equalizado
void geraGraficoHist(string nomeFoto){
  Mat src,dst;

  String local = "./img/" + nomeFoto;
  src = imread(local, CV_LOAD_IMAGE_COLOR);

  if( !src.data ){
    printf("imagem nao encontrada\n");
    exit(0); 
  }

  vector<Mat> bgr_planes;
  split(src,bgr_planes);

  int histSize = 256;

  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
  calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
  calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound((double)hist_w/histSize);

  Mat histImage(hist_h,hist_w,CV_8UC3, Scalar(255,255,255));

  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  for( int i = 1; i < histSize; i++ ){
  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
           Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
           Scalar( 255, 0, 0), 2, 8, 0  );
  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
           Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
           Scalar( 0, 255, 0), 2, 8, 0  );
  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
           Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
           Scalar( 0, 0, 255), 2, 8, 0  );
  }

  namedWindow(nomeFoto, CV_WINDOW_AUTOSIZE );
  imshow(nomeFoto, histImage );

  string localgrf = "./img/grf" + nomeFoto;  
  imwrite(localgrf,histImage);

  waitKey(0);
}

//funcao que diminui e interpola a imagem
Mat dec_int(string nomeFoto,int fator){
  stringstream stream;
  string strfator;
  string localFoto = "./img/" + nomeFoto;
  
  Mat imgOri = imread(localFoto, CV_LOAD_IMAGE_COLOR);


  if (!imgOri.data) {
    cout << "imagem nao encontrada" << endl;
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

void edge_imporv(string nomeFoto){
  srand(time(NULL));

  string localFoto = "./img/" + nomeFoto;
  Mat imgOriginal = imread(localFoto, CV_LOAD_IMAGE_COLOR);


  if (!imgOriginal.data) {
    cout << "Imagem nao encontrada" << endl;
    exit(0);//refazer isso aqui, eh feio
  }

  namedWindow("Original",WINDOW_AUTOSIZE);
  imshow("Original",imgOriginal);

  Mat imgfiltro(3,3,CV_8UC3,Scalar(0,0,0));

  for(int i = 0;i < imgfiltro.rows; i++){
    for(int j = 0;j < imgfiltro.cols; j++){  
      for(int c = 0; c < 3;c++){

        imgfiltro.at<Vec3b>(i,j)[c] = 1;
      
      }
    }
  }
  namedWindow("filtro",WINDOW_AUTOSIZE);
  imshow("filtro",imgfiltro);

  Mat imgfiltrada(imgOriginal.rows,imgOriginal.cols,CV_8UC3,Scalar(0,0,0));

 //origem do filtro (1,1)
  for(int i = 0; i < imgOriginal.rows; i++){
    for(int j = 0; j < imgOriginal.cols; j++){
      for(int c = 0; c < 3; c++){
        if(i == 0 && j == 0){

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j+1)[c]      +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i+1,j)[c]      +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i+1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;
        
        }else if(i == 0 && j!= imgOriginal.cols - 1){//se ja passou do primeiro pixel,mas mantem na primeira linha

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j+1)[c]      +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i+1,j)[c]      +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i+1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j-1)[c]      +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;
          
        }else if(i == 0 && j == imgOriginal.cols - 1){//no ultimo pixel da primeira linha

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j-1)[c]      +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i+1,j)[c]      +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i+1,j-1)[c]    +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;
          
        }else if(j == 0 && i != imgOriginal.rows - 1){//toda a primeira coluna antes da primeira e ultima linha

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i-1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i-1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j+1)[c]      +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i+1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i+1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;

        }else if(i != 0 && i != imgOriginal.rows -1 && j == imgOriginal.cols - 1){//toda a ultima coluna antes da primeira e da ultima linha

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i-1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i-1,j-1)[c]    +
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j-1)[c]      +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i+1,j-1)[c]    +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i+1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;

        }else if(i == imgOriginal.rows - 1 && j == 0){//ultima linha e primeira coluna

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i-1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i-1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j+1)[c]      +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;

        }else if(i == imgOriginal.rows - 1 && j != imgOriginal.cols - 1 && j != 0){//toda a ultima linha antes da ultima coluna e depois da primeira

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j-1)[c]      +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i-1,j-1)[c]    +
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i-1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i-1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j+1)[c]      +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;

        }else if(i == imgOriginal.rows -1 && j == imgOriginal.cols - 1){//ultima linha e ultima coluna

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i-1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i-1,j-1)[c]    +
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j-1)[c]      +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i,j)[c]        +
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i,j)[c])/9;

        }else{//quando nao eh nenhum dos outros casos

          imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgOriginal.at<Vec3b>(i,j)[c] + 
                                          imgfiltro.at<Vec3b>(0,0)[c] * imgOriginal.at<Vec3b>(i-1,j-1)[c]    +
                                          imgfiltro.at<Vec3b>(0,1)[c] * imgOriginal.at<Vec3b>(i-1,j)[c]      +
                                          imgfiltro.at<Vec3b>(0,2)[c] * imgOriginal.at<Vec3b>(i-1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(1,0)[c] * imgOriginal.at<Vec3b>(i,j-1)[c]      +
                                          imgfiltro.at<Vec3b>(1,2)[c] * imgOriginal.at<Vec3b>(i,j+1)[c]      +
                                          imgfiltro.at<Vec3b>(2,0)[c] * imgOriginal.at<Vec3b>(i+1,j+1)[c]    +
                                          imgfiltro.at<Vec3b>(2,1)[c] * imgOriginal.at<Vec3b>(i+1,j)[c]      + 
                                          imgfiltro.at<Vec3b>(2,2)[c] * imgOriginal.at<Vec3b>(i+1,j+1)[c])/9;

        }
      }
    }
  }

  namedWindow("Filtrada",WINDOW_AUTOSIZE);
  imshow("Filtrada",imgfiltrada);

  string localfilt = "./img/Filt" + nomeFoto;
  imwrite(localfilt,imgfiltrada);

  waitKey(0);
}

//funcao que gera a imagem com um filtro passa-baixas
void edge_imporv(Mat imgEntrada){
  srand(time(NULL));

  namedWindow("Original",WINDOW_AUTOSIZE);
  imshow("Original",imgEntrada);

  Mat imgfiltro(3,3,CV_8UC3,Scalar(0,0,0));

  //inicializa a matriz filtro
  for(int i = 0;i < 3;i++){
    for(int j = 0; j < 3; j++){
      for(int c = 0; c < 3;c++){

        imgfiltro.at<Vec3b>(i,j)[c] = 1;
      }
    }  
  }

  Mat imgfiltrada(imgEntrada.rows,imgEntrada.cols,CV_8UC3,Scalar(0,0,0));

  //origem do filtro (1,1)
  for(int i = 0; i < imgEntrada.rows; i++){
    for(int j = 0; j < imgEntrada.cols; j++){
      for(int c = 0; c < 3; c++){
        // for(int x = imgfiltro.rows; x > 0; x--){
        //   for(int y = imgfiltro.cols; y > 0; y--){

            if(i == 0 && j == 0){

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j+1)[c]      +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i+1,j)[c]      +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i+1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;
            
            }else if(i == 0 && j!= imgEntrada.cols - 1){//se ja passou do primeiro pixel,mas mantem na primeira linha

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j+1)[c]      +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i+1,j)[c]      +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i+1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j-1)[c]      +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;
              
            }else if(i == 0 && j == imgEntrada.cols - 1){//no ultimo pixel da primeira linha

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j-1)[c]      +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i+1,j)[c]      +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i+1,j-1)[c]    +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;
              
            }else if(j == 0 && i != imgEntrada.rows - 1){//toda a primeira coluna antes da primeira e ultima linha

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i-1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i-1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j+1)[c]      +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i+1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i+1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;

            }else if(i != 0 && i != imgEntrada.rows -1 && j == imgEntrada.cols - 1){//toda a ultima coluna antes da primeira e da ultima linha

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i-1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i-1,j-1)[c]    +
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j-1)[c]      +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i+1,j-1)[c]    +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i+1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;

            }else if(i == imgEntrada.rows - 1 && j == 0){//ultima linha e primeira coluna

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i-1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i-1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j+1)[c]      +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;

            }else if(i == imgEntrada.rows - 1 && j != imgEntrada.cols - 1 && j != 0){//toda a ultima linha antes da ultima coluna e depois da primeira

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j-1)[c]      +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i-1,j-1)[c]    +
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i-1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i-1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j+1)[c]      +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;

            }else if(i == imgEntrada.rows -1 && j == imgEntrada.cols - 1){//ultima linha e ultima coluna

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i-1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i-1,j-1)[c]    +
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j-1)[c]      +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i,j)[c]        +
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i,j)[c])/9;

            }else{//quando nao eh nenhum dos outros casos

              imgfiltrada.at<Vec3b>(i,j)[c] = (int) (imgfiltro.at<Vec3b>(1,1)[c] * imgEntrada.at<Vec3b>(i,j)[c] + 
                                              imgfiltro.at<Vec3b>(0,0)[c] * imgEntrada.at<Vec3b>(i-1,j-1)[c]    +
                                              imgfiltro.at<Vec3b>(0,1)[c] * imgEntrada.at<Vec3b>(i-1,j)[c]      +
                                              imgfiltro.at<Vec3b>(0,2)[c] * imgEntrada.at<Vec3b>(i-1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(1,0)[c] * imgEntrada.at<Vec3b>(i,j-1)[c]      +
                                              imgfiltro.at<Vec3b>(1,2)[c] * imgEntrada.at<Vec3b>(i,j+1)[c]      +
                                              imgfiltro.at<Vec3b>(2,0)[c] * imgEntrada.at<Vec3b>(i+1,j+1)[c]    +
                                              imgfiltro.at<Vec3b>(2,1)[c] * imgEntrada.at<Vec3b>(i+1,j)[c]      + 
                                              imgfiltro.at<Vec3b>(2,2)[c] * imgEntrada.at<Vec3b>(i+1,j+1)[c])/9;

            }
        //   }
        // }
      }
    }
  }

  namedWindow("Filtrada",WINDOW_AUTOSIZE);
  imshow("Filtrada",imgfiltrada);

  waitKey(0);

}

//funcao que eu criei para fazer o grafico
void geraGrfHisto(Mat fotoHisto,string nomeFoto){

  vector<int> histoEQ;

  for(int i = 0;i < 256; i++){
    histoEQ.push_back(0);
  }

  for(int linha = 0; linha < fotoHisto.rows;linha++){
    for(int coluna = 0;coluna < fotoHisto.cols;coluna++){

      histoEQ.at(fotoHisto.at<uchar>(linha,coluna))++;
    }
  }

  Mat grfHistoEQ(157,257,CV_LOAD_IMAGE_GRAYSCALE,Scalar(255,255,255));
  
  //gerar imagem do grafico
  for(int i = 255; i > 1; i--){
    if(histoEQ.at(i) != 0){
          
      for(int j = (histoEQ.at(i)%90); j > 0 ;j--){
        grfHistoEQ.at<uchar>(155 - j,i) = 0;
      }
    }
  }

  string nomeWindow = "Histo EQ " + nomeFoto;
  namedWindow(nomeWindow, WINDOW_AUTOSIZE);
  imshow(nomeWindow, grfHistoEQ);

}

//funcao que gera a imagem com o histograma equalizado
void geraHisto(string nomefoto){
  vector<int>     Rk;
  vector<double>  media;
  vector<float>   fdaFoto;

  string localFoto = "./img/" + nomefoto;

  Mat foto = imread(localFoto,CV_LOAD_IMAGE_GRAYSCALE );

  if(!foto.data){
    cout << "imagem nao encontrada" << endl;
    exit(0);
  }

  Mat fotoHisto(foto.rows,foto.cols,CV_LOAD_IMAGE_GRAYSCALE);
  
  for(int i = 0;i < 256; i++){
    Rk.push_back(0);
    media.push_back(0);
    fdaFoto.push_back(0.0);
  }

  for(int linha = 0;linha < fotoHisto.rows;linha++){
    for(int coluna = 0;coluna < fotoHisto.cols;coluna++){

      Rk.at(foto.at<uchar>(linha,coluna))++;
    }
  }

  for(int i = 0; i < 256; i++){
    if(i == 0) {
      media.at(i) = (int)(((Rk.at(i)*255)/foto.total()));
      fdaFoto.at(i) = (Rk.at(i)/foto.total());

    }else{
      media.at(i) = (int)(((Rk.at(i)*255)/foto.total()) + media.at(i-1));
      fdaFoto.at(i) = (float)((Rk.at(i)/foto.total()) + fdaFoto.at(i-1));
    }
  }

  for(int linha = 0;linha < foto.rows;linha++){
    for(int coluna = 0; coluna < foto.cols;coluna++){

      fotoHisto.at<uchar>(linha,coluna) = media.at(foto.at<uchar>(linha,coluna));
    }
  }

  namedWindow("Original",CV_WINDOW_AUTOSIZE);
  imshow("Original",foto);

  namedWindow("fotohisto",CV_WINDOW_AUTOSIZE);
  imshow("fotohisto",fotoHisto);

  string localNovaFoto = "./img/hist" + nomefoto; 

  imwrite(localNovaFoto,fotoHisto);
  waitKey(0);
}

//funcao que gera uma imagem com filtro powerlaw
void powerLaw(string nomeFoto, double fator) {

  string local ="./img/" + nomeFoto;

  Mat imagem_nova = imread(local, CV_LOAD_IMAGE_GRAYSCALE);
  double pixel;
  int linha, coluna;
  double corretor = pow(255, fator-1);

  if(!imagem_nova.data){
    cout << "imagem nao encontrada" << endl;
    exit(0);
  }

  namedWindow("Original GrayScale", WINDOW_AUTOSIZE);
  imshow("Original GrayScale", imagem_nova);

  for (linha = 0; linha < imagem_nova.rows; ++linha) {
    for (coluna = 0; coluna < imagem_nova.cols; ++coluna) {

      pixel = pow(imagem_nova.at<uchar>(linha, coluna), fator)/corretor;

      imagem_nova.at<uchar>(linha, coluna) = (int)(pixel);
    }
  }

  namedWindow("Power Law", WINDOW_AUTOSIZE);
  imshow("Power Law", imagem_nova);

  string localNovaFoto = "./img/PL" + nomeFoto; 

  imwrite(localNovaFoto,imagem_nova);

  waitKey(0);
}

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