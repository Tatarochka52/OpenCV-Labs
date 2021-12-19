#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;

int main() {
    int max_pic_in_row = 2;
    int max_pic_in_column = 2;
    int size = 500;
    int scale;
    int max;
    int width_pic;
    int height_pic;

    //start pic
    vector <Mat> input_pics;
    Mat first_pic = imread("1.jpg", IMREAD_COLOR);
    input_pics.push_back(first_pic);
    Mat second_pic = imread("2.jpg", IMREAD_COLOR);
    input_pics.push_back(second_pic);

    width_pic = first_pic.cols + second_pic.cols;
    height_pic = first_pic.rows;
    max = (width_pic > height_pic) ? width_pic : height_pic;
    scale = (float)((float)max / size);

    //result pic
    Mat panorama;

    //class Stitcher object - panorama mode
    Ptr<Stitcher> result = Stitcher::create(Stitcher::PANORAMA);

    //stitching pictures
    Stitcher::Status status = result->stitch(input_pics, panorama);

    if (status != Stitcher::OK) {
        cout << "Can't stitch images" << endl;
        return -1;
    }
    
    //output pictures
    Mat DispImage = Mat::zeros(Size(100 + size * max_pic_in_row, 100 + size * max_pic_in_column), CV_8UC3);
    Mat temp;

    Rect first(10, 10, size, height_pic);
    resize(first_pic, temp, Size(first.width, first.height));
    temp.copyTo(DispImage(first));

    Rect second(550, 10, size, height_pic);
    resize(second_pic, temp, Size(second.width, second.height));
    temp.copyTo(DispImage(second));

    /*Rect third(50, 500, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(panorama, temp, Size(third.width, third.height));
    temp.copyTo(DispImage(third));*/

    namedWindow("Input", 1);
    imshow("Input", DispImage);
    imshow("Result", panorama);
    waitKey(0);

    return 0;
}
