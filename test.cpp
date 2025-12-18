/*
  * Vatslav Didkovskiy
  * st142215@student.spbu.ru
  * LabWork1
  */
#include "RasterImage.h"
#include <gtest/gtest.h>
#include <filesystem>

class ImageTest : public ::testing::Test {
protected:
    void TearDown() override {
        if (std::filesystem::exists(test_output)) {
            std::filesystem::remove(test_output);
        }
    }

    std::string test_input = "i.bmp";
    std::string test_output = "test_output.bmp";
};

TEST_F(ImageTest, CanEstimateMemory) {
    uint32_t mem = RasterImage::estimate_memory(100, 100);
    EXPECT_GT(mem, 0u);
    EXPECT_EQ(mem, 54u + 300u * 100u);
}

TEST_F(ImageTest, LoadNonExistentFileFails) {
    RasterImage img;
    EXPECT_FALSE(img.load("nonexistent.bmp"));
}

TEST_F(ImageTest, RotateClockwiseAndSave) {
    RasterImage img;
    ASSERT_TRUE(img.load(test_input));
    auto rotated = img.rotate_cw();
    ASSERT_NE(rotated, nullptr);
    EXPECT_EQ(rotated->width(), img.height());
    EXPECT_EQ(rotated->height(), img.width());
    ASSERT_TRUE(rotated->save(test_output));
    EXPECT_TRUE(std::filesystem::exists(test_output));
}

TEST_F(ImageTest, RotateCounterClockwiseAndSave) {
    RasterImage img;
    ASSERT_TRUE(img.load(test_input));
    auto rotated = img.rotate_ccw();
    ASSERT_NE(rotated, nullptr);
    EXPECT_EQ(rotated->width(), img.height());
    EXPECT_EQ(rotated->height(), img.width());
    ASSERT_TRUE(rotated->save(test_output));
    EXPECT_TRUE(std::filesystem::exists(test_output));
}

TEST_F(ImageTest, GaussianBlurApplies) {
    RasterImage img;
    ASSERT_TRUE(img.load(test_input));
    uint32_t orig_size = img.data_size();
    img.gaussian_blur();
    EXPECT_EQ(img.data_size(), orig_size);
    ASSERT_TRUE(img.save(test_output));
    EXPECT_TRUE(std::filesystem::exists(test_output));
}

TEST_F(ImageTest, SaveAfterLoadIsIdentical) {
    RasterImage img;
    ASSERT_TRUE(img.load(test_input));
    ASSERT_TRUE(img.save(test_output));
    RasterImage img2;
    ASSERT_TRUE(img2.load(test_output));
    EXPECT_EQ(img.width(), img2.width());
    EXPECT_EQ(img.height(), img2.height());
    EXPECT_EQ(img.data_size(), img2.data_size());
}
