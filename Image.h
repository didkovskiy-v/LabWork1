/*
  * Vatslav Didkovskiy
  * st142215@student.spbu.ru
  * LabWork1
  */
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <memory>
#include <cstdint>

class Image {
public:
    virtual ~Image() = default;
    virtual bool load(const std::string& filename) = 0;
    virtual bool save(const std::string& filename) const = 0;
    virtual std::unique_ptr<Image> rotate_cw() const = 0;
    virtual std::unique_ptr<Image> rotate_ccw() const = 0;
    virtual void gaussian_blur() = 0;
    virtual uint32_t width() const = 0;
    virtual uint32_t height() const = 0;
    virtual uint32_t data_size() const = 0;
};

#endif
