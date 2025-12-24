# LabWork 1
## Author  
Didkovskiy Vatslav 25Б81-MM
## Contacts  
st142215@student.spbu.ru
## Description  
This project implements a BMP image processor that loads 24-bit uncompressed BMP files and provides the following operations:
- Loading and saving BMP images
- Rotating the image 90 degrees clockwise
- Rotating the image 90 degrees counter-clockwise
- Applying a Gaussian blur filter
The implementation uses a pure abstract base class and a concrete raster image class, with full unit tests.

## Build
make
make run
<enter image name>

## Test
make
make test

## Clean
make clean

## Usage
The program expects a 24-bit uncompressed BMP file as input.
If your image is in a different format (e.g., PNG, JPG, or compressed BMP), convert it using ImageMagick:

convert <input file name> -define bmp:format=bmp3 -compress none <output file name>.bmp

Than do make clean and run program using new file.

