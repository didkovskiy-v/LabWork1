/*
  * Vatslav Didkovskiy
  * st142215@student.spbu.ru
  * LabWork1
  */
#include "RasterImage.h"
#include <fstream>
#include <algorithm>
#include <cmath>

uint32_t RasterImage::row_bytes() const {
    uint32_t row_size = width() * BYTES_PER_PIXEL;
    uint32_t padding = (ROW_ALIGNMENT - (row_size % ROW_ALIGNMENT)) % ROW_ALIGNMENT;
    return row_size + padding;
}

bool RasterImage::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;
    if (!meta.read_from(file)) return false;
    uint32_t h = height();
    uint32_t rb = row_bytes();
    pixels.assign(rb * h, 0);
    file.seekg(meta.get_offset(), std::ios::beg);
    for (uint32_t y = 0; y < h; ++y) {
        uint32_t bmp_y = h - 1 - y;
        uint32_t idx = bmp_y * rb;
        if (!file.read(reinterpret_cast<char*>(&pixels[idx]), rb)) return false;
    }
    return true;
}

bool RasterImage::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;
    if (!meta.write_to(file)) return false;
    uint32_t h = height();
    uint32_t rb = row_bytes();
    for (uint32_t y = 0; y < h; ++y) {
        uint32_t bmp_y = h - 1 - y;
        uint32_t idx = bmp_y * rb;
        if (!file.write(reinterpret_cast<const char*>(&pixels[idx]), rb)) return false;
    }
    return true;
}

void RasterImage::get_pixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b) const {
    if (x >= width() || y >= height()) {
        r = g = b = 0;
        return;
    }
    uint32_t rb = row_bytes();
    uint32_t idx = y * rb + x * 3;
    b = pixels[idx];
    g = pixels[idx + 1];
    r = pixels[idx + 2];
}

void RasterImage::set_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= width() || y >= height()) {
        return;
    }
    uint32_t rb = row_bytes();
    uint32_t idx = y * rb + x * 3;
    pixels[idx] = b;
    pixels[idx + 1] = g;
    pixels[idx + 2] = r;
}

std::unique_ptr<Image> RasterImage::rotate_cw() const {
    uint32_t w = width(), h = height();
    auto img = std::make_unique<RasterImage>();
    img->meta = meta;
    img->meta.adjust_after_rotation(h, w);
    uint32_t new_rb = img->row_bytes();
    img->pixels.assign(new_rb * w, 0);
    for (uint32_t y = 0; y < h; ++y)
        for (uint32_t x = 0; x < w; ++x) {
            uint8_t r, g, b;
            get_pixel(x, y, r, g, b);
            img->set_pixel(h - 1 - y, x, r, g, b);
        }
    return img;
}

std::unique_ptr<Image> RasterImage::rotate_ccw() const {
    uint32_t w = width(), h = height();
    auto img = std::make_unique<RasterImage>();
    img->meta = meta;
    img->meta.adjust_after_rotation(h, w);
    uint32_t new_rb = img->row_bytes();
    img->pixels.assign(new_rb * w, 0);
    for (uint32_t y = 0; y < h; ++y)
        for (uint32_t x = 0; x < w; ++x) {
            uint8_t r, g, b;
            get_pixel(x, y, r, g, b);
            img->set_pixel(y, w - 1 - x, r, g, b);
        }
    return img;
}
void RasterImage::gaussian_blur() {
    uint32_t w = width(), h = height();
    if (w < 3 || h < 3) return;
    static const float kernel[3][3] = {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };
    std::vector<uint8_t> newData = pixels;
    uint32_t rb = row_bytes();
    for (uint32_t y = 0; y < h; ++y) {
        for (uint32_t x = 0; x < w; ++x) {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = static_cast<int>(x) + dx;
                    int ny = static_cast<int>(y) + dy;
                    if (nx < 0) nx = 0;
                    if (nx >= static_cast<int>(w)) nx = w - 1;
                    if (ny < 0) ny = 0;
                    if (ny >= static_cast<int>(h)) ny = h - 1;
                    uint8_t r, g, b;
                    get_pixel(nx, ny, r, g, b);
                    float weight = kernel[dy + 1][dx + 1];
                    sumR += r * weight;
                    sumG += g * weight;
                    sumB += b * weight;
                }
            }
            newData[y * rb + x * 3]     = static_cast<uint8_t>(std::clamp(sumB, 0.0f, 255.0f));
            newData[y * rb + x * 3 + 1] = static_cast<uint8_t>(std::clamp(sumG, 0.0f, 255.0f));
            newData[y * rb + x * 3 + 2] = static_cast<uint8_t>(std::clamp(sumR, 0.0f, 255.0f));
        }
    }
    pixels = std::move(newData);
}

uint32_t RasterImage::estimate_memory(uint32_t w, uint32_t h) {
    uint32_t row = ((w * BYTES_PER_PIXEL + ROW_ALIGNMENT - 1) / ROW_ALIGNMENT) * ROW_ALIGNMENT;
    return sizeof(FileHeader) + sizeof(InfoHeader) + row * h;
}
