/**
 * @file trabalho.cc
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <realce_power_law.h>


/**
 * @brief Main do programa.
 *
 * Mostra imagens bonitinhas.
 *
 */
int main() {

//  char img_entrada[] = "./img/test80.jpg";
//  char img_borrada[] = "./img/borrada.jpg";
//  char img_entrada[] = "./img/test82.jpg";
//  char img_entrada[] = "./img/car.png";
//  char img_entrada[] = "./img/crowd.png";
  char img_entrada[] = "./img/university.png";
//  char img_entrada[] = "./img/jureg.jpg";

//  dec_int(img_entrada, 2);
  trans_power_law(img_entrada, 0);
  trans_power_law(img_entrada, 1);
  trans_power_law(img_entrada, 2);
  trans_power_law(img_entrada, 3);
  trans_power_law(img_entrada, 6);
  trans_power_law(img_entrada, 7);
  trans_power_law(img_entrada, 8);
  trans_power_law(img_entrada, 10);
  trans_power_law(img_entrada, 20);
  trans_power_law(img_entrada, 30);
  trans_power_law(img_entrada, 0.9);
  trans_power_law(img_entrada, 0.8);
  trans_power_law(img_entrada, 0.7);
  trans_power_law(img_entrada, 0.5);
  trans_power_law(img_entrada, 0.4);
  trans_power_law(img_entrada, 0.3);
  trans_power_law(img_entrada, 0.09);
  trans_power_law(img_entrada, 0.08);
  trans_power_law(img_entrada, 0.07);
//  dec_int(img_entrada, 8);
//  dec_int(img_entrada, 16);
//  dec_int(img_entrada, 80);

//  edge_improv(img_borrada);

  return 0;

}
