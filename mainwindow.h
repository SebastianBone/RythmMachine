#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
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

    void mouse_current_pos();
    void mouse_press();
    void mouse_left();
    void mouse_rel();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked(bool checked);

private:
    void run();
    void drawRectangle(int x, int y, const cv::Scalar &scalar);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
