/*
  * Vatslav Didkovskiy
  * st142215@student.spbu.ru
  * LabWork1
  */
#include "RasterImage.h"
#include <iostream>
#include <string>

int main() {
    std::string filename;
    std::cout << "enter BMP filename: ";
    std::cin >> filename;

    RasterImage img;
    if (!img.load(filename)) {
        std::cerr << "Error: cannot load image\n";
        return 1;
    }

    std::cout << "loaded image: " << img.width() << "x" << img.height() << "\n";
    std::cout << "memory needed: " << RasterImage::estimate_memory(img.width(), img.height()) << " bytes\n";

    auto cw_img = img.rotate_cw();
    if (cw_img && cw_img->save("rotated_cw.bmp")) {
        std::cout << "rotated clockwise saved\n";
    } else {
        std::cerr << "failed to save rotated clockwise\n";
    }

    auto ccw_img = img.rotate_ccw();
    if (ccw_img && ccw_img->save("rotated_ccw.bmp")) {
        std::cout << "rotated counter-clockwise saved\n";
    } else {
        std::cerr << "failed to save rotated counter-clockwise\n";
    }

    img.gaussian_blur();
    if (img.save("gaussian_blurred.bmp")) {
        std::cout << "gaussian blur applied and saved\n";
    } else {
        std::cerr << "failed to save blurred image\n";
    }

    return 0;
}
