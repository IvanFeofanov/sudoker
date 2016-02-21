#ifndef W_MAIN_WINDOW_H
#define W_MAIN_WINDOW_H

#include <tr1/memory>

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include <QPolygonF>
#include <QPointF>

#include <cv.h>

#include "su_app_preferences.h"
#include "su_window_settings.h"
#include "su_program_state.h"

#include "su_algorithm.h"
#include "finder/su_find_field_on_frame.h"

#include "su_game.h"

#include "su_image_converter.h"

#include "d_input_cam_id.h"
#include "d_set_preferences.h"
#include "d_about.h"
#include "d_help.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    //select mode
    void selectedSolvingMode  (bool status);
    void selectedCapturingMode(bool status);

    //
    void browsing();
    void continueCapturing();

    //selected sources
    void imageFromFile  (QString pathToImage);
    void videoCapture   (int     cameraId);

    //preferences
    void changePreferences  (SuAppPreferences preferences);

public slots:
    /*call after connected all signals and slots*/
    void setup(SuWindowSettings settings = SuWindowSettings(),
               SuProgramState state = SuProgramState());

    void setImage(cv::Mat*                    image,
                  std::vector<SUPoints>*      frame,
                  int                         searchStatus);

    void setField(std::vector<SuFieldCell>*   field,
                  int                         solveStatus);

    void updatePreferences(SuAppPreferences preferences);

    //show help
    void autoShowHelp();

private slots:
    //game
    void startGame  (std::vector<int>* field);

    //draw
    void drawImage  (cv::Mat*                  image);
    void drawFrame  (std::vector<SUPoints>*    frame, int status);
    void drawNumbers(std::vector<SuFieldCell>* field);

    //show message
    void showMessage(QString message);

    //chose source
    void choseImage ();
    void choseCamera();

    //setup preferences
    void setupPreferences();

    //set mode
    void setSolvingMode  (bool status);
    void setCapturingMode(bool status);

    //browsing states
    void setBrowsingState();
    void setContinueCapturingState();

    //help
    void showHelp();
    void showAbout();

    //save image
//    void saveResultImage();

private:
    Ui::MainWindow *ui;
    std::tr1::shared_ptr<Help> helpDialog_;

    std::vector<SUPoints> frame_;
    cv::Mat               image_;

    cv::Mat calcTransformMatrix(QRect &srcFrame, SUPoints &dstFrame);

    //settings
    SuWindowSettings windowSettings_;
    SuAppPreferences preferences_;

    int timeMessage_;

    void setProgramState(SuProgramState state);

    bool isSolvingMode_;
    bool isCapturingMode_;
};

#endif // W_MAIN_WINDOW_H
