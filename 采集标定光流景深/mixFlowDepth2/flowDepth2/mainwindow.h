#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QThread>
#include"flowThread/opticalflow.h"
#include"QCustomPlot/qcustomplot.h"
#include"depthThread/depthcaculate.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);  
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    ~MainWindow();
public:
    opticalFlowCaculate opticalflowThread;
    depthCaculateThread depthThread;
private slots:
    void realtimeDataSlot(float dx);
private:
    Ui::MainWindow *ui;
    QTimer dataTimer;


};

#endif // MAINWINDOW_H
