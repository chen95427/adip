#ifndef ADAPTIVE_EDGE_DETECTION_H
#define ADAPTIVE_EDGE_DETECTION_H

#include <vector>
#include <cmath>
#include <algorithm>

// 自適應閾值邊緣檢測
void adaptiveEdgeDetection(const unsigned char* gray, unsigned char* edges, int width, int height) {
    const int windowSize = 16;
    const float k = 0.15f; // 靈敏度參數

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float mean = 0, variance = 0;
            int count = 0;

            for (int wy = -windowSize/2; wy <= windowSize/2; ++wy) {
                for (int wx = -windowSize/2; wx <= windowSize/2; ++wx) {
                    int ny = std::min(std::max(y + wy, 0), height - 1);
                    int nx = std::min(std::max(x + wx, 0), width - 1);
                    mean += gray[ny * width + nx];
                    count++;
                }
            }
            mean /= count;

            for (int wy = -windowSize/2; wy <= windowSize/2; ++wy) {
                for (int wx = -windowSize/2; wx <= windowSize/2; ++wx) {
                    int ny = std::min(std::max(y + wy, 0), height - 1);
                    int nx = std::min(std::max(x + wx, 0), width - 1);
                    float diff = gray[ny * width + nx] - mean;
                    variance += diff * diff;
                }
            }
            variance = std::sqrt(variance / count);
            float threshold = mean * (1.0f - k * (variance / 128.0f - 1.0f));
            edges[y * width + x] = (gray[y * width + x] > threshold) ? 255 : 0;
        }
    }
}

#endif
