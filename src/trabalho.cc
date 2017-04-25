#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

  Mat image;
  char img_entrada[] = "./img/car.png";
  image = imread(img_entrada, CV_LOAD_IMAGE_GRAYSCALE);

  if (!image.data) {
    cout << "No image data" << endl;
    return -1;
  }
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", image);

  waitKey(0);

  return 0;
}
