/*
  * Vatslav Didkovskiy
  * st142215@student.spbu.ru
  * LabWork1
  */
#include "BmpMeta.h"
#include <vector>
#include <iostream>

BmpMeta::BmpMeta(uint32_t w, uint32_t h) {
    fhdr.signature = 0x4D42;
    fhdr.reserved1 = 0;
    fhdr.reserved2 = 0;
    fhdr.pixel_offset = 54;
    ihdr.header_size = 40;
    ihdr.img_width = static_cast<int32_t>(w);
    ihdr.img_height = static_cast<int32_t>(h);
    ihdr.planes = 1;
    ihdr.bits_per_pixel = 24;
    ihdr.compression = 0;
    ihdr.h_resolution = 0;
    ihdr.v_resolution = 0;
    ihdr.palette_colors = 0;
    ihdr.important_colors = 0;
    uint32_t row = ((w * 3 + 3) / 4) * 4;
    ihdr.raw_size = row * h;
    fhdr.file_size = fhdr.pixel_offset + ihdr.raw_size;
}

bool BmpMeta::read_from(std::ifstream& file) {
    if (!file.read(reinterpret_cast<char*>(&fhdr), sizeof(fhdr))) return false;
    if (fhdr.signature != 0x4D42) return false;

    if (!file.read(reinterpret_cast<char*>(&ihdr), sizeof(ihdr))) return false;

    if (ihdr.bits_per_pixel != 24 || ihdr.compression != 0 || ihdr.planes != 1) {
        return false;
    }

    file.seekg(fhdr.pixel_offset, std::ios::beg);
    return true;
}

bool BmpMeta::write_to(std::ofstream& file) const {
    if (!file.write(reinterpret_cast<const char*>(&fhdr), sizeof(fhdr))) return false;
    if (!file.write(reinterpret_cast<const char*>(&ihdr), sizeof(ihdr))) return false;
    uint32_t written = sizeof(FileHeader) + sizeof(InfoHeader);
    if (fhdr.pixel_offset > written) {
        std::vector<char> pad(fhdr.pixel_offset - written, 0);
        if (!file.write(pad.data(), pad.size())) return false;
    }
    return true;
}

void BmpMeta::adjust_after_rotation(uint32_t w, uint32_t h) {
    ihdr.img_width = static_cast<int32_t>(w);
    ihdr.img_height = static_cast<int32_t>(h);
    uint32_t row = ((w * 3 + 3) / 4) * 4;
    ihdr.raw_size = row * h;
    fhdr.file_size = fhdr.pixel_offset + ihdr.raw_size;
}

uint32_t BmpMeta::get_width() const {
    return static_cast<uint32_t>(ihdr.img_width);
}

uint32_t BmpMeta::get_height() const {
    return static_cast<uint32_t>(ihdr.img_height);
}

uint32_t BmpMeta::get_bits() const {
    return ihdr.bits_per_pixel;
}

uint32_t BmpMeta::get_offset() const {
    return fhdr.pixel_offset;
}

uint32_t BmpMeta::get_image_size() const {
    return ihdr.raw_size;
}

void BmpMeta::set_image_size(uint32_t sz) {
    ihdr.raw_size = sz;
}
