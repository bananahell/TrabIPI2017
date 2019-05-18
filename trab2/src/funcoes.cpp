#include <funcoes.hpp>

using namespace std;
using namespace cv;

int rEstruturante = 1.5; 
int cEstruturante = 1.5;

//gera elemento estruturante
Mat elementoEstruturante(int opt){
	return getStructuringElement( opt == 1 ? MORPH_RECT : MORPH_ELLIPSE ,//para questao 1 use MORPH_RECT, para a 2 MORTH_ELLIPSE
								  Size( 2*rEstruturante + 1, 2*cEstruturante+1 ),
								  Point( rEstruturante, cEstruturante )); // elemento estruturante  retangulo 3x3

}

//cria a imagem com abertura
Mat Abertura(Mat img,int opt){
	Mat matAberta;

	Mat element = elementoEstruturante(opt);

	dilate(img,matAberta,element);
	erode(img,matAberta,element);

	return matAberta;
}

//cria a imagem com fechamento
Mat Fechamento(Mat img,int opt){
	Mat matFechada;

	Mat element = elementoEstruturante(opt);

	erode(img,matFechada,element);
	dilate(img,matFechada,element);

	return matFechada;
}

//cria a iamgem com erosao
Mat Erosao(Mat img,int opt){
	Mat erosao;

	Mat element = elementoEstruturante(opt);

	erode(img,erosao,element);

	return erosao;
}

//cria a imagem com dilatacao
Mat Dilatacao(Mat img,int opt){
	Mat dilatacao;

	Mat element = elementoEstruturante(opt);

	dilate(img,dilatacao,element);

	return dilatacao;
}
