/**
 * @file trabalho.cc
 * 
 * @brief Arquivo que vai rodar opencv lindão
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <dec_int.h>


/**
 * @brief Main do programa.
 *
 * Mostra imagens bonitinhas.
 *
 */
int main() {

  char img_entrada[] = "./img/test80.jpg";
  char img_entrada2[] = "./img/test82.jpg";

  dec_int(img_entrada, 2);
  dec_int(img_entrada, 4);
  dec_int(img_entrada, 8);
  dec_int(img_entrada, 16);
  dec_int(img_entrada2, 80);

  return 0;
}
