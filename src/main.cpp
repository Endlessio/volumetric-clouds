//#include "mainwindow.h"

//#include <QApplication>
//#include <QScreen>
//#include <iostream>
//#include <QSettings>


//int main(int argc, char *argv[]) {
//    QApplication a(argc, argv);

//    QCoreApplication::setApplicationName("Volumetric Clouds Rendering!");
//    QCoreApplication::setOrganizationName("CS 1230");
//    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

//    QSurfaceFormat fmt;
//    fmt.setVersion(4, 1);
//    fmt.setProfile(QSurfaceFormat::CoreProfile);
//    QSurfaceFormat::setDefaultFormat(fmt);

//    MainWindow w;
//    w.initialize();
//    w.resize(1200, 600);
//    int desktopArea = QGuiApplication::primaryScreen()->size().width() *
//                      QGuiApplication::primaryScreen()->size().height();
//    int widgetArea = w.width() * w.height();
//    if (((float)widgetArea / (float)desktopArea) < 0.75f) {
//        w.show();
//    } else {
//        w.showMaximized();
//    }

//    int return_val = a.exec();
//    w.finish();


//    return return_val;
//}
#include "mainwindow.h"

#include <QApplication>

//#include "noise/perlin.h"

int main(int argc, char *argv[])
{
//    int noiseRes = 1024;
//    int gridRes = 16;
//    int numOctaves = 4;
//    auto perlinGen = Perlin(noiseRes, gridRes, numOctaves);
//    auto noiseMap = perlinGen.generatePerlinNoise2D();
//    std::vector<uchar> noiseMapU8(noiseRes*noiseRes);
//    for (int i = 0; i < noiseMapU8.size(); i++)
//        noiseMapU8[i] = noiseMap[i] * 255.f;
//    QImage img(noiseMapU8.data(), noiseRes, noiseRes, QImage::Format_Grayscale8);
//    img.save("height.png");

//    auto normalMap = perlinGen.computePerlinNormal2D(noiseMap, 0.1);
//    std::vector<uchar> normalMapU8;
//    normalMapU8.reserve(noiseRes*noiseRes*3);
//    for (auto &v: normalMap) {
////        std::cout << glm::to_string(v) << "\n";
//        normalMapU8.push_back((v.x + 1.f) / 2.f * 255.f);
//        normalMapU8.push_back((v.y + 1.f) / 2.f * 255.f);
//        normalMapU8.push_back((v.z + 1.f) / 2.f * 255.f);
//    }
//    QImage normalImg(normalMapU8.data(), noiseRes, noiseRes, QImage::Format_RGB888);
//    normalImg.save("normal.png");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
