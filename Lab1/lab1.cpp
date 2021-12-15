#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
    int max_pic_in_row = 3;
    int max_pic_in_column = 2;
    int size = 300;
    int scale;
    int max;
    int width_pic;
    int height_pic;

    //first pic
    Mat image = imread("pic.jpg", IMREAD_COLOR);

    if (image.empty()) {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    //second pic
    Mat imgray;
    cvtColor(image, imgray, COLOR_BGR2GRAY);
    cvtColor(imgray, imgray, COLOR_GRAY2BGR);

    //third pic
    Mat hueimage;
    cvtColor(image, hueimage, COLOR_BGR2HSV);

    //fourth pic
    Mat gaussepic;
    GaussianBlur(image, gaussepic, Size(51, 51), 10);

    //fifth pic
    Mat cannyimage;
    Canny(image, cannyimage, 10, 350);
    cvtColor(cannyimage, cannyimage, COLOR_GRAY2BGR);

    //sixth picture
    Mat negativeimage;
    negativeimage = 255 - image;

    //output pictures
    width_pic = image.cols;
    height_pic = image.rows;

    Mat DispImage = Mat::zeros(Size(280 + size * max_pic_in_row, 160 + size * max_pic_in_column), CV_8UC3);
    putText(DispImage, "Original picture!", Point(110, 45), FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2);
    putText(DispImage, "GrayScale picture!", Point(500, 45), FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2);
    putText(DispImage, "HSV picture!", Point(900, 45), FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2);
    putText(DispImage, "Gausse picture!", Point(110, 425), FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2);
    putText(DispImage, "Canny picture!", Point(500, 425), FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2);
    putText(DispImage, "Negative picture!", Point(900, 425), FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2);
    Mat temp;

    max = (width_pic > height_pic) ? width_pic : height_pic;
    scale = (float)((float)max / size);

    Rect first(20, 50, (int)( width_pic / scale), (int)(height_pic / scale));
    resize(image, temp, Size(first.width, first.height));
    temp.copyTo(DispImage(first));

    Rect second(400, 50, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(imgray, temp, Size(second.width, second.height));
    temp.copyTo(DispImage(second));

    Rect third(780, 50, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(hueimage, temp, Size(third.width, third.height));
    temp.copyTo(DispImage(third));

    Rect fourth(20, 430, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(gaussepic, temp, Size(fourth.width, fourth.height));
    temp.copyTo(DispImage(fourth));

    Rect fifth(400, 430, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(cannyimage, temp, Size(fifth.width, fifth.height));
    temp.copyTo(DispImage(fifth));

    Rect sixth(780, 430, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(negativeimage, temp, Size(sixth.width, sixth.height));
    temp.copyTo(DispImage(sixth));

    namedWindow("Binary Cat", 1);
    imshow("Binary Cat", DispImage);
    waitKey(0);

    return 0;
}
