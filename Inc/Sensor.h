#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QTimer>

class Sensor : public QObject 
{
    Q_OBJECT

public:
    Sensor(QObject *parent = nullptr);
    void toggle();

signals:
    void dataReady(double value);

private:
    QTimer *timer;
    void generate();
};

#endif