#include <funcoes.hpp>
#include <string>

using namespace std;

Mat dec_int(string nomeFoto,int fator,int cont){
  stringstream stream;
  string strcont;
  string localFoto = "./img/" + nomeFoto;
  
  Mat imgOri = imread(localFoto, CV_LOAD_IMAGE_COLOR);


  if (!imgOri.data) {
    cout << "No image data" << endl;
    exit(0);//refazer isso aqui, eh feio
  }
 
  stream << cont;
  strcont = stream.str();
  string nomeOriginal = "Original " + strcont;
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

  namedWindow("Interpolada",WINDOW_AUTOSIZE);
  imshow("Interpolada",imgInt);


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
