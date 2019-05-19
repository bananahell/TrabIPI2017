/**
 * @file dec_int.cc
 *
 * @brief Arquivo que expande os pixels de uma imagem
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <dec_int.h>

cv::Mat dec_int(std::string img_entrada, int fator) {
  cv::Mat imagem_orig;
  imagem_orig = cv::imread(img_entrada, cv::IMREAD_COLOR);

  cv::Mat imagem_menor(imagem_orig.rows / fator, imagem_orig.cols / fator,
                       CV_8UC3, cv::Scalar(0, 0, 0));

  if (!imagem_orig.data) {
    std::cout << "Imagem " << img_entrada
              << " não encontrada. Retornando com erro." << std::endl;
    return imagem_menor;
  }

  if (fator % 2 != 0) {
    std::cout
        << "Fator de expansão da imagem deve ser par. Retornando com erro."
        << std::endl;
    return imagem_menor;
  }

  std::stringstream stream;
  stream << fator;
  std::string fator_string = stream.str();

  cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
  imshow("Original", imagem_orig);

  for (int y = 0; y < imagem_orig.rows; y += fator) {
    for (int x = 0; x < imagem_orig.cols; x += fator) {
      imagem_menor.at<cv::Vec3b>(y / fator, x / fator) =
          imagem_orig.at<cv::Vec3b>(y, x);
    }
  }

  std::string img_menor = img_entrada;
  img_menor.insert(img_menor.find_last_of('/'), "/ex1");
  img_menor.insert(img_menor.find_last_of('.'), "_menor_");
  img_menor.insert(img_menor.find_last_of('.'), fator_string);
  imwrite(img_menor, imagem_menor);
  std::cout << img_menor << std::endl;
  cv::namedWindow("Menor", cv::WINDOW_AUTOSIZE);
  imshow("Menor", imagem_menor);

  cv::Mat imagem_result(imagem_menor.rows * fator, imagem_menor.cols * fator,
                        CV_8UC3, cv::Scalar(0, 0, 0));

  for (int y = 0; y < imagem_menor.rows; ++y) {
    for (int x = 0; x < imagem_menor.cols; ++x) {
      imagem_result.at<cv::Vec3b>(y * fator, x * fator) =
          imagem_menor.at<cv::Vec3b>(y, x);
      for (int rows_vazias = 0; rows_vazias < fator; ++rows_vazias) {
        for (int cols_vazias = 0; cols_vazias < fator; ++cols_vazias) {
          imagem_result.at<cv::Vec3b>((y * fator) + rows_vazias,
                                      (x * fator) + cols_vazias) =
              imagem_result.at<cv::Vec3b>(y * fator, x * fator);
        }
      }
    }
  }

  std::string img_result = img_entrada;
  img_result.insert(img_result.find_last_of('/'), "/ex1");
  img_result.insert(img_result.find_last_of('.'), "_quadriculada_");
  img_result.insert(img_result.find_last_of('.'), fator_string);

  imwrite(img_result, imagem_result);
  std::cout << img_result << std::endl;
  cv::namedWindow("Quadriculada", cv::WINDOW_AUTOSIZE);
  imshow("Quadriculada", imagem_result);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return imagem_result;
}
