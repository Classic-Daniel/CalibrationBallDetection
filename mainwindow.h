#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtQuick>
#include <opencv2/opencv.hpp>
#include <QMainWindow>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QKeyEvent>
#include <RangeSlider.h>
#include <QtQuick>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_rgbThreshCheckBox_stateChanged(int arg1);

    void on_erosionSlider_sliderMoved(int position);

    void on_dilationSlider_sliderMoved(int position);

    void on_maskedImageRadioButton_toggled(bool checked);

    void on_redASlider_sliderMoved(int position);

    void on_redBSlider_sliderMoved(int position);


    void on_greenASlider_sliderMoved(int position);

    void on_greenBSlider_sliderMoved(int position);

    void on_blueASlider_sliderMoved(int position);

    void on_blueBSlider_sliderMoved(int position);

    void on_redCSlider_sliderMoved(int position);

    void on_greenCSlider_sliderMoved(int position);

    void on_blueCSlider_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_blueASlider_2_sliderMoved(int position);

    void on_blueBSlider_2_sliderMoved(int position);

    void on_blueCSlider_2_sliderMoved(int position);

    void on_redASlider_2_sliderMoved(int position);

    void on_redBSlider_2_sliderMoved(int position);

    void on_redCSlider_2_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
