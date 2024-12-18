#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
// 高斯模糊函式
void gaussianBlur(const unsigned char* input, unsigned char* output, int width, int height, int channels, int radius) {
    float sigma = radius / 2.0f;
    int kernelSize = 2 * radius + 1;
    std::vector<float> kernel(kernelSize, 0);
    float sum = 0;

    // 生成高斯核
    for (int i = 0; i < kernelSize; ++i) {
        float x = i - radius;
        kernel[i] = std::exp(-(x * x) / (2 * sigma * sigma));
        sum += kernel[i];
    }
    for (float& k : kernel) k /= sum;

    // 水平方向模糊
    std::vector<unsigned char> temp(width * height * channels, 0);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                float blurred = 0;
                for (int i = -radius; i <= radius; ++i) {
                    int sx = std::clamp(x + i, 0, width - 1);
                    blurred += input[(y * width + sx) * channels + c] * kernel[i + radius];
                }
                temp[(y * width + x) * channels + c] = static_cast<unsigned char>(blurred);
            }
        }
    }

    // 垂直方向模糊
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                float blurred = 0;
                for (int i = -radius; i <= radius; ++i) {
                    int sy = std::clamp(y + i, 0, height - 1);
                    blurred += temp[(sy * width + x) * channels + c] * kernel[i + radius];
                }
                output[(y * width + x) * channels + c] = static_cast<unsigned char>(blurred);
            }
        }
    }
}

int main() {
    int width, height, channels;

    // 讀取圖片
    unsigned char* img = stbi_load("J:/adip2/25.jpg", &width, &height, &channels, 0);
    if (!img) {
        std::cerr << "Failed to load image\n";
        return -1;
    }

    // 灰度圖
    std::vector<unsigned char> gray(width * height);
    for (int i = 0; i < width * height; ++i) {
        gray[i] = 0.299f * img[i * channels] + 0.587f * img[i * channels + 1] + 0.114f * img[i * channels + 2];
    }
    stbi_write_jpg("gray.jpg", width, height, 1, gray.data(), 95);

    // 邊緣檢測 (簡單閾值法)
    std::vector<unsigned char> edges(width * height, 0);
    for (int i = 0; i < width * height; ++i) {
        edges[i] = (gray[i] < 128) ? 255 : 0;
    }
    stbi_write_jpg("edges.jpg", width, height, 1, edges.data(), 95);

    // 邊緣膨脹
    std::vector<unsigned char> dilatedEdges(width * height, 0);
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            if (edges[y * width + x] == 255) {
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        dilatedEdges[(y + ky) * width + (x + kx)] = 255;
                    }
                }
            }
        }
    }
    stbi_write_jpg("edges_dilated.jpg", width, height, 1, dilatedEdges.data(), 95);

    // 生成遮罩
    std::vector<unsigned char> mask = dilatedEdges;
    stbi_write_jpg("mask.jpg", width, height, 1, mask.data(), 95);

    // 高斯模糊背景
    std::vector<unsigned char> blurred(width * height * channels, 0);
    gaussianBlur(img, blurred.data(), width, height, channels, 10);
    stbi_write_jpg("blurred_background.jpg", width, height, channels, blurred.data(), 95);

    // 合成最終圖像
    std::vector<unsigned char> output(width * height * channels, 0);
    for (int i = 0; i < width * height; ++i) {
        if (mask[i] == 255) {
            // 主物體：保留原始圖片
            for (int c = 0; c < channels; ++c) {
                output[i * channels + c] = img[i * channels + c];
            }
        } else {
            // 背景：使用模糊後的圖片
            for (int c = 0; c < channels; ++c) {
                output[i * channels + c] = blurred[i * channels + c];
            }
        }
    }

    // 輸出最終結果
    stbi_write_jpg("output_blurred.jpg", width, height, channels, output.data(), 95);

    // 清理記憶體
    stbi_image_free(img);

    std::cout << "All outputs saved: gray.jpg, edges.jpg, edges_dilated.jpg, mask.jpg, blurred_background.jpg, and output_blurred.jpg\n";
    return 0;
}
