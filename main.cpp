// main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "sterngerlachsimulator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<SternGerlachSimulator>("SternGerlach", 1, 0, "SternGerlachSimulator");
    
    engine.loadFromModule("SternGerlachSimulator", "Main");

    return app.exec();
}
