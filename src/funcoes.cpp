#include <funcoes.hpp>
#include <string>

using namespace std;

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

  /*for (int i = 0; i < imgDim.rows; ++i ) {
    for( int j = 0; j < imgDim.cols; ++j ) {
      for (int c = 0; c < 3; ++c) {
        imgInt.at<Vec3b>(i*fator, j*fator) = imgDim.at<Vec3b>(i, j);
        for (int d = 0; d < fator; ++d) {
          for (int c = 0; c < fator; ++c) {
            imgInt.at<Vec3b>((i*fator) + d, (j*fator) + c) = imgInt.at<Vec3b>(i*fator, j*fator);
          }
        }
      }
    }
  }
*/

  namedWindow("Dim",WINDOW_AUTOSIZE);
  imshow("Dim",imgDim);

  stream.str(" ");

  stream << fator;
  strfator = stream.str();
  string nomeInt = "Interpolada" + strfator;
  namedWindow(nomeInt,WINDOW_AUTOSIZE);
  imshow(nomeInt,imgInt);


  waitKey(0);

  /*destroyWindow("Dim");
  destroyWindow("Original");
  destroyWindow("Interpolada");

  imgOri.release();
  imgInt.release();
  imgDim.release();

*/
  destroyAllWindows();
  return imgInt;
}


void edge_imporv(string nomearq){
  srand(time(NULL));

  string localFoto = "./img/" + nomearq;
  Mat imgOriginal = imread(localFoto, CV_LOAD_IMAGE_COLOR);


  if (!imgOriginal.data) {
    cout << "No image data" << endl;
    exit(0);//refazer isso aqui, eh feio
  }

  namedWindow("Original",WINDOW_AUTOSIZE);
  imshow("Original",imgOriginal);

  Mat imgfiltro(3,3,CV_8UC3,Scalar(0,0,0));

  //essa iimagem transforma em uma parada mto legal 
  /*for(int c = 0; c < 3;c++){

    imgfiltro.at<Vec3b>(0,0)[c] =  9;
    imgfiltro.at<Vec3b>(0,1)[c] =  1;
    imgfiltro.at<Vec3b>(0,2)[c] =  9;
    imgfiltro.at<Vec3b>(1,0)[c] =  1;
    imgfiltro.at<Vec3b>(1,1)[c] =  9;
    imgfiltro.at<Vec3b>(1,2)[c] =  1;
    imgfiltro.at<Vec3b>(2,0)[c] =  9;
    imgfiltro.at<Vec3b>(2,1)[c] =  1;
    imgfiltro.at<Vec3b>(2,2)[c] =  9; 
  }*/

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

  waitKey(0);
}


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
          
      for(int j = (histoEQ.at(i)&80); j > 0 ;j--){
        grfHistoEQ.at<uchar>(155 - j,i) = 0;
      }
    }
  }

  //gera "imagem" no terminal do grafico
  /*for(int i = 0;i < 256; i++){
    if(histoEQ.at(i) != 0){
      
      if( i >= 100){
        cout << i << " - ";
      }else{
        if(i >= 10){

          cout << i<< "  - ";
        }else{

          cout << i<< "   - ";
        }
      }
          
      for(int j = (histoEQ.at(i)&80); j > 1 ; j--){
        cout << "*";
      }
      cout << endl;
    }
  }*/

  string nomeWindow = "Histo EQ " + nomeFoto;
  namedWindow(nomeWindow, WINDOW_AUTOSIZE);
  imshow(nomeWindow, grfHistoEQ);

}

void geraHisto(string nomefoto){
  vector<int> Rk;
  vector<double> media;
  vector<float> fdaFoto;
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

  for(int linha = 0; linha < fotoHisto.rows;linha++){
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

  //geracao do grafico do hsitograma para o terminal
  geraGrfHisto(fotoHisto,"Novo");
  geraGrfHisto(foto,"Original");

  //geracao funcao de densidade acumulada ->nao funciona ainda :/
  for(int i = 0;i < 256; i++){

    if( i >= 100){
      cout << i << " - ";
    }else{
      if(i >= 10){

        cout << i<< "  - ";
      }else{

        cout << i<< "   - ";
      }
    }
    printf("%.7f\n",fdaFoto.at(i));
  }

  waitKey(0);
}

void power_law(string img, double fator) {

  string local ="./img/" + img;

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

  waitKey(0);
}
