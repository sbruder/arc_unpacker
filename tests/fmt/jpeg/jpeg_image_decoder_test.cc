#include "fmt/jpeg/jpeg_image_decoder.h"
#include "io/file_io.h"
#include "test_support/catch.hh"
#include "test_support/file_support.h"
#include "test_support/image_support.h"

using namespace au;
using namespace au::fmt::jpeg;

TEST_CASE("JPEG 24-bit images", "[util]")
{
    auto input_file = tests::file_from_path(
        "tests/fmt/jpeg/files/reimu_opaque.jpg");

    JpegImageDecoder jpeg_image_decoder;
    auto pixels = jpeg_image_decoder.decode(*input_file);
    REQUIRE(pixels.width() == 1024);
    REQUIRE(pixels.height() == 1024);

    auto color = pixels.at(200, 100);
    REQUIRE(static_cast<int>(color.r) == 0x60);
    REQUIRE(static_cast<int>(color.g) == 0x97);
    REQUIRE(static_cast<int>(color.b) == 0xE7);
    REQUIRE(static_cast<int>(color.a) == 0xFF);
}

TEST_CASE("JPEG 8-bit images", "[util]")
{
    JpegImageDecoder decoder;
    auto input_file = tests::file_from_path(
        "tests/fmt/jpeg/files/NoName.jpeg");
    auto expected_image = tests::image_from_path(
        "tests/fmt/jpeg/files/NoName-out.png");
    auto actual_image = decoder.decode(*input_file);
    tests::compare_images(*expected_image, actual_image);
}