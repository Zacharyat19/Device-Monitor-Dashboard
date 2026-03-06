#include "Sensor.h"
#include <QRandomGenerator>

Sensor::Sensor(QObject *parent) : QObject(parent) 
{
    timer = new QTimer(this);
    timer->setInterval(1000);
    
    connect(timer, &QTimer::timeout, this, &Sensor::generate);
}

void Sensor::toggle() 
{
    if (timer->isActive()) 
    {
        timer->stop();
    } 
    else 
    {
        timer->start();
    }
}

void Sensor::generate() 
{
    double randomValue = QRandomGenerator::global()->bounded(20, 81);
    emit dataReady(randomValue);
}