/*
  * Vatslav Didkovskiy
  * st142215@student.spbu.ru
  * LabWork1
  */
#ifndef RASTERIMAGE_H
#define RASTERIMAGE_H

#include "Image.h"
#include "BmpMeta.h"
#include <vector>

class RasterImage : public Image {
private:
    BmpMeta meta;
    std::vector<uint8_t> pixels;
    uint32_t row_bytes() const;
public:
    RasterImage() = default;
    bool load(const std::string& filename) override;
    bool save(const std::string& filename) const override;
    std::unique_ptr<Image> rotate_cw() const override;
    std::unique_ptr<Image> rotate_ccw() const override;
    void gaussian_blur() override;
    uint32_t width() const override { return meta.get_width(); }
    uint32_t height() const override { return meta.get_height(); }
    uint32_t data_size() const override { return pixels.size(); }
    void set_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    void get_pixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b) const;
    static uint32_t estimate_memory(uint32_t w, uint32_t h);
};

#endif
