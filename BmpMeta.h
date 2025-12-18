/*
  * Vatslav Didkovskiy
  * st142215@student.spbu.ru
  * LabWork1
  */
#ifndef BMPMETA_H
#define BMPMETA_H

#include <cstdint>
#include <fstream>

#pragma pack(push, 1)
struct FileHeader {
    uint16_t signature;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixel_offset;
};

struct InfoHeader {
    uint32_t header_size;
    int32_t img_width;
    int32_t img_height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t raw_size;
    uint32_t h_resolution;
    uint32_t v_resolution;
    uint32_t palette_colors;
    uint32_t important_colors;
};
#pragma pack(pop)

class BmpMeta {
private:
    FileHeader fhdr;
    InfoHeader ihdr;
public:
    BmpMeta() = default;
    BmpMeta(uint32_t w, uint32_t h);
    bool read_from(std::ifstream& file);
    bool write_to(std::ofstream& file) const;
    void adjust_after_rotation(uint32_t w, uint32_t h);
    uint32_t get_width() const;
    uint32_t get_height() const;
    uint32_t get_bits() const;
    uint32_t get_offset() const;
    uint32_t get_image_size() const;
    void set_image_size(uint32_t sz);
};

#endif
