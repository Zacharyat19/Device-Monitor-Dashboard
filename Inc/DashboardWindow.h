#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include "Sensor.h"

class DashboardWindow : public QMainWindow 
{
    Q_OBJECT 

public:
    DashboardWindow(QWidget *parent = nullptr);

private:
    Sensor *tempSensor;
    Sensor *humiditySensor;
    Sensor *pressureSensor;

    QLabel *tempLabel;
    QLabel *humidityLabel;
    QLabel *pressureLabel;
    QLabel *statusLabel;
    QPushButton *toggleButton;
    QSlider *warningSlider;

    QChart *chart;
    QLineSeries *chartSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    
    int tickCount;
    int warningThreshold;
    bool isRunning;

    void setupUI();
    void setupChart();
    void connectSignals();
};

#endif