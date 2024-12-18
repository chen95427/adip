#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <iostream>

class ImageProcessingGUI : public QMainWindow {
    Q_OBJECT
public:
    ImageProcessingGUI(QWidget *parent = nullptr) : QMainWindow(parent) {
        auto *centralWidget = new QWidget(this);
        auto *layout = new QVBoxLayout(centralWidget);

        auto *label = new QLabel("圖片處理工具", this);
        auto *loadButton = new QPushButton("載入圖片", this);
        auto *processButton = new QPushButton("處理圖片", this);
        auto *saveButton = new QPushButton("儲存輸出", this);

        layout->addWidget(label);
        layout->addWidget(loadButton);
        layout->addWidget(processButton);
        layout->addWidget(saveButton);

        connect(loadButton, &QPushButton::clicked, this, &ImageProcessingGUI::loadImage);
        connect(processButton, &QPushButton::clicked, this, &ImageProcessingGUI::processImage);
        connect(saveButton, &QPushButton::clicked, this, &ImageProcessingGUI::saveOutput);

        setCentralWidget(centralWidget);
    }

private slots:
    void loadImage() {
        QString fileName = QFileDialog::getOpenFileName(this, "開啟圖片", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            std::cout << "載入圖片: " << fileName.toStdString() << std::endl;
        }
    }

    void processImage() {
        std::cout << "處理圖片中..." << std::endl;
        // 呼叫你的處理函式，例如 GaussianBlur 或邊緣檢測
    }

    void saveOutput() {
        QString fileName = QFileDialog::getSaveFileName(this, "儲存圖片", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            std::cout << "儲存輸出到: " << fileName.toStdString() << std::endl;
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageProcessingGUI window;
    window.setWindowTitle("圖片處理工具");
    window.show();
    return app.exec();
}
