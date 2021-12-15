#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
    int max_pic_in_row = 3;
    int max_pic_in_column = 2;
    int size = 440;
    int scale;
    int max;
    int width_pic;
    int height_pic;
    const int matrix_size = 7;
    float standard_deviation = 10;
    float blur_matrix[matrix_size][matrix_size];
    
    //original picture
    Mat image = imread("pic.jpg", IMREAD_COLOR);

    if (image.empty()) {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    width_pic = image.cols;
    height_pic = image.rows;

    //blur
    Mat my_blur_pic = Mat::zeros(Size(width_pic + (matrix_size - 1), (height_pic + (matrix_size - 1))), CV_8UC3);
    Mat blur_roi;
    Rect roi(((matrix_size - 1) / 2) - 1, ((matrix_size - 1) / 2) - 1, width_pic, height_pic);
    resize(image, blur_roi, Size(roi.width, roi.height));
    blur_roi.copyTo(my_blur_pic(roi));

    int start_matrix_x = 0;
    int start_matrix_y = 0;
    float div = 0;
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            blur_matrix[i][j] = exp(-(pow(start_matrix_x, 2) + (pow(start_matrix_y, 2))) / (2 * pow(standard_deviation, 2))) / (2 * M_PI * pow(standard_deviation, 2));
            div += blur_matrix[i][j];
            start_matrix_x++;
        }
        start_matrix_x = 0;
        start_matrix_y++;
    }

    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            blur_matrix[i][j] /= div;
        }
    }

    for (int i = 0; i < height_pic; i++) {
        for (int j = 0; j < width_pic; j++) {
            if (i < (matrix_size - 1) / 2 && j < (matrix_size - 1) / 2) {
                my_blur_pic.at<Vec3b>(i, j)[0] = my_blur_pic.at<Vec3b>((matrix_size - 1) / 2, (matrix_size - 1) / 2)[0];
                my_blur_pic.at<Vec3b>(i, j)[1] = my_blur_pic.at<Vec3b>((matrix_size - 1) / 2, (matrix_size - 1) / 2)[1];
                my_blur_pic.at<Vec3b>(i, j)[2] = my_blur_pic.at<Vec3b>((matrix_size - 1) / 2, (matrix_size - 1) / 2)[2];
            }
            if (i < (matrix_size - 1) / 2 && j >= (matrix_size - 1) / 2 && j <= width_pic + (matrix_size - 1) / 2) {
                my_blur_pic.at<Vec3b>(i, j)[0] = my_blur_pic.at<Vec3b>((matrix_size - 1) / 2, j)[0];
                my_blur_pic.at<Vec3b>(i, j)[1] = my_blur_pic.at<Vec3b>((matrix_size - 1) / 2, j)[1];
                my_blur_pic.at<Vec3b>(i, j)[2] = my_blur_pic.at<Vec3b>((matrix_size - 1) / 2, j)[2];
            }
            if (i > height_pic + (matrix_size - 1) / 2 && j >= (matrix_size - 1) / 2 && j <= width_pic + (matrix_size - 1) / 2) {
                my_blur_pic.at<Vec3b>(i, j)[0] = my_blur_pic.at<Vec3b>(height_pic + (matrix_size - 1) / 2, j)[0];
                my_blur_pic.at<Vec3b>(i, j)[1] = my_blur_pic.at<Vec3b>(height_pic + (matrix_size - 1) / 2, j)[1];
                my_blur_pic.at<Vec3b>(i, j)[2] = my_blur_pic.at<Vec3b>(height_pic + (matrix_size - 1) / 2, j)[2];
            }
            if (j < (matrix_size - 1) / 2 && i <= height_pic + (matrix_size - 1) / 2 && i >= (matrix_size - 1) / 2) {
                my_blur_pic.at<Vec3b>(i, j)[0] = my_blur_pic.at<Vec3b>(i, (matrix_size - 1) / 2)[0];
                my_blur_pic.at<Vec3b>(i, j)[1] = my_blur_pic.at<Vec3b>(i, (matrix_size - 1) / 2)[1];
                my_blur_pic.at<Vec3b>(i, j)[2] = my_blur_pic.at<Vec3b>(i, (matrix_size - 1) / 2)[2];
            }
            if (j > width_pic + (matrix_size - 1) / 2 && i >= (matrix_size - 1) / 2 && i <= height_pic + (matrix_size - 1) / 2) {
                my_blur_pic.at<Vec3b>(i, j)[0] = my_blur_pic.at<Vec3b>(i, width_pic + (matrix_size - 1) / 2)[0];
                my_blur_pic.at<Vec3b>(i, j)[1] = my_blur_pic.at<Vec3b>(i, width_pic + (matrix_size - 1) / 2)[1];
                my_blur_pic.at<Vec3b>(i, j)[2] = my_blur_pic.at<Vec3b>(i, width_pic + (matrix_size - 1) / 2)[2];
            }
            if (i > height_pic + (matrix_size - 1) / 2 && j > width_pic + (matrix_size - 1) / 2) {
                my_blur_pic.at<Vec3b>(i, j)[0] = my_blur_pic.at<Vec3b>(height_pic + (matrix_size - 1) / 2 - 1, width_pic + (matrix_size - 1) / 2 - 1)[0];
                my_blur_pic.at<Vec3b>(i, j)[1] = my_blur_pic.at<Vec3b>(height_pic + (matrix_size - 1) / 2 - 1, width_pic + (matrix_size - 1) / 2 - 1)[1];
                my_blur_pic.at<Vec3b>(i, j)[2] = my_blur_pic.at<Vec3b>(height_pic + (matrix_size - 1) / 2 - 1, width_pic + (matrix_size - 1) / 2 - 1)[2];
            }

            float matrix_coeff_r = 0;
            float matrix_coeff_g = 0;
            float matrix_coeff_b = 0;
            for (int l = 0; l < matrix_size; l++) {
                for (int k = 0; k < matrix_size; k++) {
                    matrix_coeff_b += (int)my_blur_pic.at<Vec3b>(i + k, j + l)[0] * blur_matrix[k][l];
                    matrix_coeff_g += (int)my_blur_pic.at<Vec3b>(i + k, j + l)[1] * blur_matrix[k][l];
                    matrix_coeff_r += (int)my_blur_pic.at<Vec3b>(i + k, j + l)[2] * blur_matrix[k][l];
                }
            }
            for (int l = 0; l < matrix_size; l++) {
                for (int k = 0; k < matrix_size; k++) {
                    my_blur_pic.at<Vec3b>(i + k, j + l)[0] = matrix_coeff_b;
                    my_blur_pic.at<Vec3b>(i + k, j + l)[1] = matrix_coeff_g;
                    my_blur_pic.at<Vec3b>(i + k, j + l)[2] = matrix_coeff_r;
                }
            }
        }
    }

    //gradient
    Mat my_gradient_pic = image.clone();
    int a_r = 0, a_g = 0, a_b = 0;
    int b_r = 0, b_g = 0, b_b = 0;
    for (int i = 0; i < height_pic - 1; i++) {
        for (int j = 0; j < width_pic - 1; j++) {
            a_b = (int)my_gradient_pic.at<Vec3b>(i, j + 1)[0] - (int)my_blur_pic.at<Vec3b>(i, j)[0];
            a_g = (int)my_gradient_pic.at<Vec3b>(i, j + 1)[1] - (int)my_blur_pic.at<Vec3b>(i, j)[1];
            a_r = (int)my_gradient_pic.at<Vec3b>(i, j + 1)[2] - (int)my_blur_pic.at<Vec3b>(i, j)[2];

            b_b = (int)my_gradient_pic.at<Vec3b>(i + 1, j)[0] - (int)my_blur_pic.at<Vec3b>(i, j)[0];
            b_g = (int)my_gradient_pic.at<Vec3b>(i + 1, j)[1] - (int)my_blur_pic.at<Vec3b>(i, j)[1];
            b_r = (int)my_gradient_pic.at<Vec3b>(i + 1, j)[2] - (int)my_blur_pic.at<Vec3b>(i, j)[2];

            my_gradient_pic.at<Vec3b>(i, j)[0] = a_b + b_b;
            my_gradient_pic.at<Vec3b>(i, j)[1] = a_g + b_g;
            my_gradient_pic.at<Vec3b>(i, j)[2] = a_r + b_r;
        }
    }


    //output pictures
    Mat DispImage = Mat::zeros(Size(280 + size * max_pic_in_row, 100 + size * max_pic_in_column), CV_8UC3);
    Mat temp;

    max = (width_pic > height_pic) ? width_pic : height_pic;
    scale = (float)((float)max / size);

    Rect first(450, 50, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(image, temp, Size(first.width, first.height));
    temp.copyTo(DispImage(first));

    Rect second(50, 450, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(my_blur_pic, temp, Size(second.width, second.height));
    temp.copyTo(DispImage(second));

    Rect third(780, 450, (int)(width_pic / scale), (int)(height_pic / scale));
    resize(my_gradient_pic, temp, Size(third.width, third.height));
    temp.copyTo(DispImage(third));

    namedWindow("Tribore Menendez", 1);
    imshow("Tribore Menendez", DispImage);
    waitKey(0);

    return 0;
}
