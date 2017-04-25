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

  char img_entrada[] = "./img/a.jpg";

  dec_int(img_entrada, 2);
  dec_int(img_entrada, 4);
  dec_int(img_entrada, 8);
  dec_int(img_entrada, 16);
  dec_int(img_entrada, 32);
  dec_int(img_entrada, 64);
  dec_int(img_entrada, 128);
  dec_int(img_entrada, 256);
  dec_int(img_entrada, 512);

  return 0;
}
