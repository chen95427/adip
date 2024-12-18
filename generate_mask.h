#ifndef GENERATE_MASK_H
#define GENERATE_MASK_H

#include <vector>
#include <queue>

void generateObjectMask(const std::vector<Region>& regions, unsigned char* mask, int width, int height) {
    std::fill(mask, mask + width * height, 0);
    auto regionsCopy = regions;
    std::sort(regionsCopy.begin(), regionsCopy.end(),
        [](const Region& a, const Region& b) { return a.area > b.area; });

    if (!regionsCopy.empty()) {
        const Region& mainObject = regionsCopy[0];
        std::queue<std::pair<int, int>> queue;
        std::vector<bool> visited(width * height, false);

        int startX = static_cast<int>(mainObject.centerX);
        int startY = static_cast<int>(mainObject.centerY);
        queue.push({startX, startY});
        visited[startY * width + startX] = true;
        mask[startY * width + startX] = 255;

        while (!queue.empty()) {
            auto [x, y] = queue.front();
            queue.pop();
            const int dx[] = {-1, 1, 0, 0};
            const int dy[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; ++i) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= mainObject.minX && nx <= mainObject.maxX &&
                    ny >= mainObject.minY && ny <= mainObject.maxY &&
                    !visited[ny * width + nx]) {
                    visited[ny * width + nx] = true;
                    mask[ny * width + nx] = 255;
                    queue.push({nx, ny});
                }
            }
        }
    }
}

#endif
