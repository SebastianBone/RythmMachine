#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <SFML/Audio.hpp>
#include "audioengine.h"
#include <QCursor>
#include "modqpushbutton.h"
#include "modqlabel.h"

/////////////////////////////////////////////////////////////
namespace Ui {
class Dialog;
}


/////////////////////////////////////////////////////////////
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


private:
    Ui::Dialog *ui;

    cv::VideoCapture capWebCam;

    cv::Mat matSrc, matOriginal, matHSV, matProcessed;

    QImage imgOriginal, imgProcessed, imgGrayscale;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;

    QTimer *tmrTimer,*audioTimer;

    int hH, hS, hV, lH, lS, lV;

    AudioEngine beatAudio, snareAudio, hitAudio, drumAudio;

    QPushButton * beatButtons[8];
    QPushButton * snareButtons[8];
    QPushButton * hitButtons[8];
    QPushButton * drumButtons[8];

    int index;

    QCursor *cursor;

    modQLabel lblEnter;

    bool cursorTracking;

public slots:
    void processFrameAndUpdateGUI();
    cv::Mat rescale(cv::Mat &mat);
    void processAudio();
    void pairUiButtonstoArray();
    void timerStart();
    void initialize();
    void hsvTreshholdsInit();
    void check4Swipe();


private slots:
    void on_btnPauseResume_clicked();
    void on_blueLow_valueChanged(int value);
    void on_greenLow_valueChanged(int value);
    void on_redLow_valueChanged(int value);
    void on_blueHigh_valueChanged(int value);
    void on_greenHigh_valueChanged(int value);
    void on_redHigh_valueChanged(int value);
    void on_btnCursorTr_clicked();
    void on_btnPRCapt_clicked();
};

#endif // DIALOG_H
