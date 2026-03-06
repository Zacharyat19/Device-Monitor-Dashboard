#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
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
    QComboBox *sensorSelector;

    QChart *chart;
    QLineSeries *chartSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    
    int tickCount;
    int warningThreshold;
    bool isRunning;
    int currentChartSensor; // Tracks which sensor is actively being graphed

    void setupUI();
    void setupChart();
    void connectSignals();
};

#endif