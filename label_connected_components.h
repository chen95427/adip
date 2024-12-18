#ifndef LABEL_CONNECTED_COMPONENTS_H
#define LABEL_CONNECTED_COMPONENTS_H

#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

struct Region {
    int minX, minY, maxX, maxY;
    int area;
    float centerX, centerY;
    float distanceToCenter;
};

// 標記連通區域並返回區域資訊
std::vector<Region> labelConnectedComponents(const unsigned char* edges, int width, int height) {
    std::vector<int> labels(width * height, 0);
    int currentLabel = 1;
    std::vector<Region> regions;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (edges[y * width + x] == 255 && labels[y * width + x] == 0) {
                Region region = {x, y, x, y, 0, 0, 0, 0};
                std::queue<std::pair<int, int>> queue;
                queue.push({x, y});
                labels[y * width + x] = currentLabel;

                while (!queue.empty()) {
                    auto [cx, cy] = queue.front();
                    queue.pop();
                    region.area++;
                    region.centerX += cx;
                    region.centerY += cy;

                    region.minX = std::min(region.minX, cx);
                    region.maxX = std::max(region.maxX, cx);
                    region.minY = std::min(region.minY, cy);
                    region.maxY = std::max(region.maxY, cy);

                    const int dx[] = {-1, 1, 0, 0};
                    const int dy[] = {0, 0, -1, 1};
                    for (int i = 0; i < 4; ++i) {
                        int nx = cx + dx[i];
                        int ny = cy + dy[i];
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                            edges[ny * width + nx] == 255 && labels[ny * width + nx] == 0) {
                            labels[ny * width + nx] = currentLabel;
                            queue.push({nx, ny});
                        }
                    }
                }

                if (region.area > 0) {
                    region.centerX /= region.area;
                    region.centerY /= region.area;
                    region.distanceToCenter = std::sqrt(
                        std::pow(region.centerX - width / 2.0f, 2) +
                        std::pow(region.centerY - height / 2.0f, 2)
                    );
                    regions.push_back(region);
                }
                currentLabel++;
            }
        }
    }
    return regions;
}

#endif
