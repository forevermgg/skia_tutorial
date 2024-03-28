#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat srcImage = imread("resources/test1.bmp");
    if (!srcImage.data) {
        std::cout << "Image not loaded";
        return -1;
    }
    resize(srcImage, srcImage, Size(1024, 1024));
    namedWindow("TestOpenCV", 0);
    imshow("TestOpenCV", srcImage);
    waitKey(0);
    destroyAllWindows();
    return 0;
}