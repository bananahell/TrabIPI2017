/**
 * @file realces.cc
 *
 * @brief Arquivo que contém os filtros de realce do contraste de imagens
 *
 * @author Pedro Nogueira - 14/0065032
 *
 */

#include <realces.h>

cv::Mat trans_power_law(std::string img, double fator) {
  cv::Mat imagem_nova = cv::imread(img, cv::IMREAD_COLOR);

  if (!imagem_nova.data) {
    std::cout << "Imagem " << img << " não encontrada. Retornando com erro."
              << std::endl;
    cv::Mat imagem_lixo(2, 2, CV_8UC3, cv::Scalar(0, 0, 0));
    return imagem_lixo;
  }

  double pixel;
  int row, col;
  double corretor = pow(255, fator - 1);

  std::stringstream stream;
  stream << fator;
  std::string fator_string = stream.str();

  cv::namedWindow("Original GrayScale", cv::WINDOW_AUTOSIZE);
  imshow("Original GrayScale", imagem_nova);

  // Os três for's que iteram os pixels da imagem recebida
  for (row = 0; row < imagem_nova.rows; ++row) {
    for (col = 0; col < imagem_nova.cols; ++col) {
      pixel = pow(imagem_nova.at<uchar>(row, col), fator) / corretor;

      imagem_nova.at<uchar>(row, col) = (int)(pixel);
    }
  }

  std::string img_realce = img;
  img_realce.insert(img_realce.find_last_of('/'), "/ex2");
  img_realce.insert(img_realce.find_last_of('.'), "_realce_power_");
  img_realce.insert(img_realce.find_last_of('.'), fator_string);

  imwrite(img_realce, imagem_nova);
  std::cout << img_realce << std::endl;
  cv::namedWindow("Power Law", cv::WINDOW_AUTOSIZE);
  imshow("Power Law", imagem_nova);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return imagem_nova;
}

cv::Mat trans_hist(std::string img) {
  cv::Mat imagem_orig = cv::imread(img, cv::IMREAD_GRAYSCALE);
  cv::Mat imagem_nova = imagem_orig;

  if (!imagem_orig.data || !imagem_nova.data) {
    std::cout << "Imagem " << img << " não encontrada. Retornando com erro."
              << std::endl;
    cv::Mat imagem_lixo(2, 2, CV_8UC3, cv::Scalar(0, 0, 0));
    return imagem_lixo;
  }

  cv::namedWindow("Original GrayScale", cv::WINDOW_AUTOSIZE);
  imshow("Original GrayScale", imagem_orig);

  std::vector<double> canais;
  std::vector<double> porcents_canais;
  std::vector<int> porcents;
  int row, col, i;
  for (i = 0; i < 256; ++i) {
    canais.push_back(0);
    porcents_canais.push_back(0);
    porcents.push_back(0);
  }

  for (row = 0; row < imagem_nova.rows; ++row) {
    for (col = 0; col < imagem_nova.cols; ++col) {
      ++canais.at(imagem_nova.at<uchar>(row, col));
    }
  }

  for (i = 0; i < 256; ++i) {
    if (i != 0) {
      porcents.at(i) =
          (int)(((canais.at(i) * 255) / (imagem_nova.rows * imagem_nova.cols)) +
                porcents.at(i - 1));
    } else {
      porcents.at(i) =
          (int)(((canais.at(i) * 255) / (imagem_nova.rows * imagem_nova.cols)));
    }
  }

  for (row = 0; row < imagem_nova.rows; ++row) {
    for (col = 0; col < imagem_nova.cols; ++col) {
      imagem_nova.at<uchar>(row, col) =
          porcents.at(imagem_nova.at<uchar>(row, col));
      ++porcents_canais.at(imagem_nova.at<uchar>(row, col));
    }
  }

  std::string img_realce = img;
  img_realce.insert(img_realce.find_last_of('/'), "/ex2");
  img_realce.insert(img_realce.find_last_of('.'), "_realce_hist_");

  imwrite(img_realce, imagem_nova);
  std::cout << img_realce << std::endl;
  cv::namedWindow("Histogramada", cv::WINDOW_AUTOSIZE);
  imshow("Histogramada", imagem_nova);

  cv::Mat histo_orig(700, 768, CV_8U, cv::Scalar(0, 0, 0));
  cv::Mat histo_nova(700, 768, CV_8U, cv::Scalar(0, 0, 0));

  //  double escala = 1;
  int maior_can = 0;
  int maior_porc = 0;
  for (i = 0; i < 256; ++i) {
    if (canais.at(i) > canais.at(maior_can)) {
      maior_can = i;
    }
    if (porcents_canais.at(i) > porcents_canais.at(maior_porc)) {
      maior_porc = i;
    }
  }

  for (col = 0; col < 256; ++col) {
    for (row = 0; row < (int)(canais.at(col) * 650 / canais.at(maior_can));
         ++row) {
      histo_orig.at<uchar>(histo_orig.rows - row, (col * 3) - 1) = 255;
    }
    for (row = 0; row < (int)((porcents_canais.at(col)) * 650 /
                              (porcents_canais.at(maior_porc)));
         ++row) {
      histo_nova.at<uchar>(histo_nova.rows - row, (col * 3) - 1) = 255;
    }
  }

  std::string img_hist_orig = img;
  img_hist_orig.insert(img_hist_orig.find_last_of('/'), "/ex2");
  img_hist_orig.insert(img_hist_orig.find_last_of('.'), "_hist_orig_");

  imwrite(img_hist_orig, histo_orig);
  std::cout << img_hist_orig << std::endl;
  cv::namedWindow("Histograma Original", cv::WINDOW_AUTOSIZE);
  imshow("Histograma Original", histo_orig);

  std::string img_hist_nova = img;
  img_hist_nova.insert(img_hist_nova.find_last_of('/'), "/ex2");
  img_hist_nova.insert(img_hist_nova.find_last_of('.'), "_hist_nova_");

  imwrite(img_hist_nova, histo_nova);
  std::cout << img_hist_nova << std::endl;
  cv::namedWindow("Histograma Nova", cv::WINDOW_AUTOSIZE);
  imshow("Histograma Nova", histo_nova);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return imagem_nova;
}
