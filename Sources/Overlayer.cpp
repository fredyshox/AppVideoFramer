//
// Created by Kacper Rączy on 23/07/2020.
//

#include "Overlayer.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include <filesystem>

namespace avo {

RGBColor::RGBColor(
    uint8_t red,
    uint8_t green,
    uint8_t blue
): red(red), green(green), blue(blue) {}

RGBColor::RGBColor(uint32_t value) {
    blue = (uint8_t) (value & 0xff);
    green = (uint8_t) ((value >> 8u) & 0xff);
    red = (uint8_t) ((value >> 16u) & 0xff);
}

std::string RGBColor::hexString() {
    uint32_t value = ((uint32_t) red << 16u) | ((uint32_t) blue << 8u) | ((uint32_t) green);
    std::stringstream ss;
    ss << '#' << std::uppercase << std::setfill('0') << std::setw(6) << std::right << std::hex << value;
    return ss.str();
}

OverlayConfig::OverlayConfig(
    std::string imagePath,
    int originX,
    int originY,
    int templateWidth,
    int templateHeight
): imagePath(imagePath), originX(originX),
   originY(originY), templateWidth(templateWidth),
   templateHeight(templateHeight) {}

bool OverlayConfig::isValid() {
    return std::filesystem::exists(imagePath) && originX >= 0 && originY >= 0;
}

OutputConfig::OutputConfig(
    std::string path,
    double fps,
    int width,
    int height,
    RGBColor backgroundColor
): path(path), fps(fps), width(width), height(height), backgroundColor(backgroundColor) {}

Overlayer::Overlayer(const OverlayConfig &config) : _config(config) {
    cv::Mat bgraImage = cv::imread(config.imagePath, cv::IMREAD_UNCHANGED);
    cv::extractChannel(bgraImage, _mask, 3);
    cv::cvtColor(bgraImage, _backgroundImage, cv::COLOR_BGRA2BGR);
}

OverlayConfig Overlayer::config() {
    return _config;
}

Task Overlayer::overlayTask(const OutputConfig &outputConfig) {
    return Task(_backgroundImage, _mask, _config, outputConfig);
}

}