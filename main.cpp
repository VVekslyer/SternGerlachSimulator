/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  main.cpp
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SternGerlachSimulator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<SternGerlachSimulator>("SternGerlach", 1, 0, "SternGerlachSimulator");
    
    engine.loadFromModule("SternGerlachSimulator", "Main");

    return app.exec();
}
