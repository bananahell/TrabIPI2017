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
#include <funcoes.hpp>
#include <ctime>
#include <cmath>

using namespace std;
using namespace cv;
/*
int main() {
  int cont = 0;
  int opt;
  string nomeFoto;
  int fator = 0;

  do{
    system("clear");

    cout << "+---------------------------------------+" << endl
         << "| Trabalho de IPI                       |" << endl
         << "| Aluno : João Pedro de Salles Braga    |" << endl
         << "|                                       |" << endl
         << "|Escolha uma opcao:                     |" << endl
         << "|1-Diminuir imagem/Interpolar           |" << endl
         << "|2-Diminuir/Interpolar em sequencia     |" << endl
         << "|3-filtrar imagem                       |" << endl
         << "|4-filtrar imagem diminuida/interpolada |" << endl
         << "|5-filtro media                         |" << endl
         << "|6-sair                                 |" << endl
         << "+---------------------------------------+" << endl;

    cin >> opt;
    getchar();

    switch(opt){
      case 1:

        cout << "Digite o nome da imagem" << endl;
        cin >> nomeFoto;

        cout << "Digite o fator:" << endl;
        cin >> fator;

        dec_int(nomeFoto,fator,cont);
        cont++;

        destroyAllWindows();      
      break;
      case 2:
        cout << "Gerando sequencia"<< endl;

        cout << "Digite o nome da imagem" << endl;
        cin >> nomeFoto;

        for(int i = 2; i < 32;i = i * 2){
          cout << i << endl;
            dec_int(nomeFoto,i,i);
        }

        destroyAllWindows();

      break;
      case 3:

        cout << "Digite o nome da imagem" << endl;
        cin >> nomeFoto;
      
        filtro(nomeFoto);

        destroyAllWindows();
      break;
      case 4:
        cout << "Digite o nome da imagem" << endl;
        cin >> nomeFoto;

        cout << "Digite o fator:" << endl;
        cin >> fator;

        cont++;

        filtro(dec_int(nomeFoto,fator,cont));
        destroyAllWindows(); 
      break;
      case 5:
        
        cout << "Digite o nome da imagem" << endl;
        cin >> nomeFoto;

        filtro(nomeFoto);
        destroyAllWindows(); 

      break;
      case 6:
        cout << "Tchau!" << endl;
      break;
      default:
        cout << "Opcao invalida"<< endl;
      break;
    }

  }while(opt != 6);

  return 0;
}
*/

int main(){
  
  cout << "comecando o programa" << endl;
  getchar();

  cout << "usando dec_int para imagem test80.jpg" << endl;

  for(int i = 2; i < 16;i = i * 2){
    cout << "Fator " << i << endl;
    dec_int("test80.jpg",i,i);
  }

  cout << "Filtro para imagem ja reduzida" << endl;
  getchar();

  Mat imgTest80 = imread("./img/test80.jpg", CV_LOAD_IMAGE_COLOR);


  if (!imgTest80.data) {
    cout << "No image data" << endl;
    exit(0);//refazer isso aqui, eh feio
  }

  Mat imgMenor(imgTest80.rows/4,imgTest80.cols/4,CV_LOAD_IMAGE_COLOR,Scalar(0,0,0));

  resize(imgTest80,imgMenor,Size(imgTest80.rows,imgTest80.cols));

  filtro(imgMenor);



  return 0;
}