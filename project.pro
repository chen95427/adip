# ====================
# Qt 模組設置
# ====================
QT += core gui widgets

# ====================
# 專案設置
# ====================
TEMPLATE = app
TARGET = ImageProcessingGUI
CONFIG += c++17 release

# ====================
# 編譯器和鏈接器設置
# ====================
QMAKE_CXX = C:/msys64/mingw64/bin/g++.exe
QMAKE_LINK = C:/msys64/mingw64/bin/g++.exe
QMAKE_AR = C:/msys64/mingw64/bin/ar.exe

# ====================
# 源碼和標頭文件
# ====================
SOURCES += main.cpp

# 包含路徑（根據目錄結構設定）
INCLUDEPATH += C:/msys64/mingw64/include \
               C:/msys64/mingw64/include/QtCore \
               C:/msys64/mingw64/include/QtGui \
               C:/msys64/mingw64/include/QtWidgets

# ====================
# 連結庫路徑與庫文件
# ====================
LIBS += -LC:/msys64/mingw64/lib \
        -lQt6Core \
        -lQt6Gui \
        -lQt6Widgets \
        -lshell32

# ====================
# 輸出執行檔目錄
# ====================
DESTDIR = ./bin

# ====================
# 編譯器標誌與鏈接器設置
# ====================
QMAKE_CXXFLAGS += -Wall -Wextra -O2
QMAKE_LFLAGS += -Wl,-subsystem,windows
