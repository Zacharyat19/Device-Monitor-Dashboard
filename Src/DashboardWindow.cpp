#include "DashboardWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

DashboardWindow::DashboardWindow(QWidget *parent) 
: QMainWindow(parent), tickCount(0), warningThreshold(80), isRunning(false), currentChartSensor(0)
{
    // 1. Initialize the backend sensors
    tempSensor = new Sensor(this);
    humiditySensor = new Sensor(this);
    pressureSensor = new Sensor(this);

    // 2. Build the visual interface
    setupUI();
    setupChart();
    
    // 3. Wire the frontend and backend together
    connectSignals();
}

void DashboardWindow::setupUI() 
{
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // Left Column: Sensors and Controls
    QVBoxLayout *leftLayout = new QVBoxLayout();
    
    statusLabel = new QLabel("STATUS: STOPPED", this);
    statusLabel->setStyleSheet("font-weight: bold; color: red;");
    
    tempLabel = new QLabel("Temperature: --", this);
    humidityLabel = new QLabel("Humidity: --", this);
    pressureLabel = new QLabel("Pressure: --", this);
    
    toggleButton = new QPushButton("Start Simulation", this);
    
    QLabel *sliderLabel = new QLabel("Temp Warning Threshold:", this);
    warningSlider = new QSlider(Qt::Horizontal, this);
    warningSlider->setRange(50, 100);
    warningSlider->setValue(warningThreshold);

    QLabel *dropdownLabel = new QLabel("Select Chart Data:", this);
    sensorSelector = new QComboBox(this);
    sensorSelector->addItem("Temperature (°C)");
    sensorSelector->addItem("Humidity (%)");
    sensorSelector->addItem("Pressure (hPa)");

    leftLayout->addWidget(statusLabel);
    leftLayout->addWidget(tempLabel);
    leftLayout->addWidget(humidityLabel);
    leftLayout->addWidget(pressureLabel);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(toggleButton);
    leftLayout->addWidget(sliderLabel);
    leftLayout->addWidget(warningSlider);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(dropdownLabel);
    leftLayout->addWidget(sensorSelector);
    leftLayout->addStretch(); // Pushes everything to the top

    // Right Column: The Chart
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QChartView *chartView = new QChartView(this);
    chart = new QChart();
    chartView->setChart(chart);
    rightLayout->addWidget(chartView);

    // Combine columns
    mainLayout->addLayout(leftLayout, 1); // 1 part width
    mainLayout->addLayout(rightLayout, 2); // 2 parts width
    
    setCentralWidget(centralWidget);
    resize(800, 400);
}

void DashboardWindow::setupChart() 
{
    chartSeries = new QLineSeries();
    chartSeries->setName("Temperature");
    chart->addSeries(chartSeries);
    chart->setTitle("Live Sensor Data");
    chart->setAnimationOptions(QChart::NoAnimation);

    axisX = new QValueAxis();
    axisX->setRange(0, 30);
    axisX->setLabelFormat("%d");
    
    axisY = new QValueAxis();
    axisY->setRange(0, 100);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chartSeries->attachAxis(axisX);
    chartSeries->attachAxis(axisY);
}

void DashboardWindow::connectSignals() 
{
    // Connect Start/Stop Button
    connect(toggleButton, &QPushButton::clicked, this, [=]() 
    {
        tempSensor->toggle();
        humiditySensor->toggle();
        pressureSensor->toggle();
        
        isRunning = !isRunning;
        if (isRunning) 
        {
            toggleButton->setText("Stop Simulation");
            statusLabel->setText("STATUS: RUNNING");
            statusLabel->setStyleSheet("font-weight: bold; color: green;");
        } 
        else 
        {
            toggleButton->setText("Start Simulation");
            statusLabel->setText("STATUS: STOPPED");
            statusLabel->setStyleSheet("font-weight: bold; color: red;");
        }
    });

    // Connect Slider to update threshold variable
    connect(warningSlider, &QSlider::valueChanged, this, [=](int value) 
    {
        warningThreshold = value;
    });

    // Connect the Dropdown to track the selected sensor and update the chart styling
    connect(sensorSelector, &QComboBox::currentIndexChanged, this, [=](int index) 
    {
        currentChartSensor = index;
        chartSeries->clear(); // Clear the old line
        tickCount = 0;        // Reset the X-axis
        axisX->setRange(0, 30);
        
        // Update the visual Y-axis and Title based on the selection
        if (index == 0) 
        {
            axisY->setRange(0, 100);
            chartSeries->setName("Temperature");
        } 
        else if (index == 1) 
        {
            axisY->setRange(0, 100); // Humidity is 0-100%
            chartSeries->setName("Humidity");
        } 
        else if (index == 2) 
        {
            axisY->setRange(900, 1100); // Standard atmospheric pressure range
            chartSeries->setName("Pressure");
        }
    });

    // Connect Temperature Sensor to Label and conditionally to Chart
    connect(tempSensor, &Sensor::dataReady, this, [=](double value) 
    {
        tempLabel->setText(QString("Temperature: %1 C").arg(value, 0, 'f', 1));
        
        // Visual Warning Logic
        if (value >= warningThreshold) 
        {
            tempLabel->setStyleSheet("color: red; font-weight: bold;");
        } 
        else 
        {
            tempLabel->setStyleSheet(""); // Reset to default
        }

        // ONLY draw if Temperature (Index 0) is selected
        if (currentChartSensor == 0) 
        {
            tickCount++;
            chartSeries->append(tickCount, value);
            if (chartSeries->count() > 30) 
            {
                chartSeries->removePoints(0, 1);
                axisX->setRange(tickCount - 30, tickCount);
            }
        }
    });

    // Connect Humidity Sensor to Label and conditionally to Chart
    connect(humiditySensor, &Sensor::dataReady, this, [=](double value) 
    {
        humidityLabel->setText(QString("Humidity: %1 %").arg(value, 0, 'f', 1));
        
        // ONLY draw if Humidity (Index 1) is selected
        if (currentChartSensor == 1) 
        {
            tickCount++;
            chartSeries->append(tickCount, value);
            if (chartSeries->count() > 30) 
            {
                chartSeries->removePoints(0, 1);
                axisX->setRange(tickCount - 30, tickCount);
            }
        }
    });

    // Connect Pressure Sensor to Label and conditionally to Chart
    connect(pressureSensor, &Sensor::dataReady, this, [=](double value) 
    {
        // Adjust display to look like realistic pressure
        double scaledPressure = value * 10.0 + 500.0; 
        pressureLabel->setText(QString("Pressure: %1 hPa").arg(scaledPressure, 0, 'f', 0));
        
        // ONLY draw if Pressure (Index 2) is selected
        if (currentChartSensor == 2) 
        {
            tickCount++;
            chartSeries->append(tickCount, scaledPressure);
            if (chartSeries->count() > 30) 
            {
                chartSeries->removePoints(0, 1);
                axisX->setRange(tickCount - 30, tickCount);
            }
        }
    });
}