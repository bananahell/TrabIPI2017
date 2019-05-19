#include <string>
#include "opencv2/opencv.hpp"

class WatershedSegmenter {
 private:
  cv::Mat markers;

 public:
  void setMarkers(cv::Mat& markerImage) {
    markerImage.convertTo(markers, CV_32S);
  }

  cv::Mat process(cv::Mat& image) {
    cv::watershed(image, markers);
    markers.convertTo(markers, CV_8U);
    return markers;
  }
};

int main() {
  cv::Mat image = cv::imread("./img/img_cells.jpg");
  cv::Mat blank(image.size(), CV_8U, cv::Scalar(0xFF));
  cv::Mat dest;
  imshow("originalimage", image);

  // Create markers image
  cv::Mat markers(image.size(), CV_8U, cv::Scalar(-1));
  // Rect(topleftcornerX, topleftcornerY, width, height);
  // top rectangle
  markers(Circle(0, 0, image.cols, 5)) = Scalar::all(1);
  // bottom rectangle
  markers(circle(0, image.rows - 5, image.cols, 5)) = Scalar::all(1);
  // left rectangle
  markers(circle(0, 0, 5, image.rows)) = Scalar::all(1);
  // right rectangle
  markers(circle(image.cols - 5, 0, 5, image.rows)) = Scalar::all(1);
  // centre rectangle
  int centreW = image.cols / 4;
  int centreH = image.rows / 4;
  markers(circle((image.cols / 2) - (centreW / 2),
                 (image.rows / 2) - (centreH / 2), centreW, centreH)) =
      Scalar::all(2);
  markers.convertTo(markers, CV_BGR2GRAY);
  imshow("markers", markers);

  // Create watershed segmentation object
  WatershedSegmenter segmenter;
  segmenter.setMarkers(markers);
  cv::Mat wshedMask = segmenter.process(image);
  cv::Mat mask;
  convertScaleAbs(wshedMask, mask, 1, 0);
  double thresh = threshold(mask, mask, 1, 255, THRESH_BINARY);
  bitwise_and(image, image, dest, mask);
  dest.convertTo(dest, CV_8U);

  imshow("final_result", dest);
  cv::waitKey(0);

  return 0;
}