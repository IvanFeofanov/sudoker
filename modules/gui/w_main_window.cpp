#include "w_main_window.h"
#include "ui_w_main_window.h"

#include "d_about.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(qApp->applicationName());

    helpDialog_ = std::tr1::shared_ptr<Help>(new Help(this));
}

void MainWindow::setup(SuWindowSettings settings,
                       SuProgramState   state)
{
    //scene
    ui->graphicsView->setScene(new QGraphicsScene(QRectF(0, 0, 640, 480)));

    //action
    ui->menubar->show();
    ui->actionCapture->setVisible(false);

    //set capture mode
    connect(ui->actionCapture, SIGNAL(toggled(bool)),
            this, SIGNAL(selectedCapturingMode(bool)));

    connect(this, SIGNAL(selectedCapturingMode(bool)),
            this, SLOT(setCapturingMode(bool)));

    //set solving mode
    connect(ui->actionSolve, SIGNAL(triggered(bool)),
            this, SIGNAL(selectedSolvingMode(bool)));

    connect(this, SIGNAL(selectedSolvingMode(bool)),
            this, SLOT(setSolvingMode(bool)));

    //continue
    connect(ui->actionContinue, SIGNAL(triggered()),
            this, SIGNAL(continueCapturing()));

    connect(this, SIGNAL(continueCapturing()),
            this, SLOT(setContinueCapturingState()));

    //browsing
    connect(this, SIGNAL(browsing()),
            this, SLOT(setBrowsingState()));

    //chose camera id
    connect(ui->actionVideo_capture, SIGNAL(triggered()),
            this, SLOT(choseCamera()));

    //chose image from file
    connect(ui->actionImage_from_file, SIGNAL(triggered()),
            this, SLOT(choseImage()));

    //selected sources
    connect(this, SIGNAL(imageFromFile(QString)),
            this, SIGNAL(continueCapturing()));

    connect(this, SIGNAL(videoCapture(int)),
            this, SIGNAL(continueCapturing()));

    //set preferences
    connect(ui->actionPreferences, SIGNAL(triggered()),
            this, SLOT(setupPreferences()));

    //about
    connect(ui->actionAbout, SIGNAL(triggered()),
            this, SLOT(showAbout()));
    connect(ui->actionHelp, SIGNAL(triggered()),
            this, SLOT(showHelp()));


    //settings
    windowSettings_ = settings;

    //set state
    setProgramState(state);

    //time
    timeMessage_ = 5000;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::autoShowHelp()
{
    helpDialog_->autoShow();
}

void MainWindow::setupPreferences()
{
    SetPreferences* setPreferences = new SetPreferences(&preferences_, this);

    if(setPreferences->exec() == QDialog::Accepted)
    {
        preferences_ = setPreferences->preferences();

        emit changePreferences(preferences_);
    }

    delete setPreferences;
}

void MainWindow::setSolvingMode(bool status)
{
    if(status == true)
    {
        emit selectedCapturingMode(false);
        ui->actionCapture->setChecked(false);
    }

    isSolvingMode_ = status;
}

void MainWindow::setCapturingMode(bool status)
{
    if(status == true)
    {
        emit selectedSolvingMode(false);
        ui->actionSolve->setChecked(false);
    }

    isCapturingMode_ = status;
}

void MainWindow::drawImage(cv::Mat* image)
{
    QPixmap pixmap;

    //convert cv::Mat to QPixmap
    SuImageConverter::cvmat2qpixmap(*image, pixmap);

    //set

    //get scene
    QGraphicsScene* scene = ui->graphicsView->scene();

    if(scene->width()  != pixmap.width() ||
       scene->height() != pixmap.height() )
    {
        delete scene;
        scene = new QGraphicsScene(QRectF(0, 0, pixmap.width(), pixmap.height()));
        ui->graphicsView->setScene(scene);
    }

    //set pixmap
    QGraphicsItem* graphicsItem = 0;

    scene->clear();
    graphicsItem = scene->addPixmap(pixmap);

    //fit in view
    ui->graphicsView->fitInView(graphicsItem, Qt::KeepAspectRatio);
}

void MainWindow::drawFrame(std::vector<SUPoints>* frame, int status)
{
    assert(frame->size() > 0);

    QGraphicsScene* scene =  ui->graphicsView->scene();

    QPolygonF polygon;

    for(int i = 0; i < 4; i++)
    {
        cv::Point point = frame->at(0).at(i < 4 ? i : 0);
        polygon << QPointF(point.x, point.y);
    }

    QPen pen;
    pen.setWidth(windowSettings_.widthLineFrame);

    switch(status)
    {
    case SuFindFieldOnFrame::CAPTURED:
        pen.setColor(windowSettings_.colorStaticFrame);
        break;

    case SuFindFieldOnFrame::MOVING:
        pen.setColor(windowSettings_.colorDinamicFrame);
        break;
    }

    scene->addPolygon(polygon, pen);

}

void MainWindow::drawNumbers(std::vector<SuFieldCell>* field)
{
//    SuInterface interface;
//    interface.printField(*field);

    assert(field->size() > 0);

    const int CELL_SIZE = 80;
    const int FONT_SCALE = 2;

    const int OFFSET_ON_X = CELL_SIZE / 4;
    const int OFFSET_ON_Y = CELL_SIZE / -4;

    cv::Mat fieldImage = cv::Mat(CELL_SIZE * 9,
                                 CELL_SIZE * 9,
                                 CV_8UC4,
                                 cv::Scalar(0, 0, 0, 0));
    cv::Mat image      = cv::Mat(ui->graphicsView->scene()->height(),
                                 ui->graphicsView->scene()->width(),
                                 CV_8UC4,
                                 cv::Scalar(0, 0, 0, 0));

    //draw fieldImage
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            int index = y * 9 + x;

            //skip empty cell
            if(field->at(index).type == SuFieldCell::UNKNOWN)
                continue;

            //set color
            cv::Scalar color;

            if(field->at(index).isConflict)
                color = cv::Scalar(windowSettings_.colorConflictNumber.blue(),
                                   windowSettings_.colorConflictNumber.green(),
                                   windowSettings_.colorConflictNumber.red(),
                                   windowSettings_.colorConflictNumber.alpha());
            else
                color = cv::Scalar(windowSettings_.colorNumber.blue(),
                                   windowSettings_.colorNumber.green(),
                                   windowSettings_.colorNumber.red(),
                                   windowSettings_.colorNumber.alpha());

            cv::Point point(CELL_SIZE * (x)   + OFFSET_ON_X,
                            CELL_SIZE * (y+1) + OFFSET_ON_Y);

            int value = field->at(index).value;

            cv::putText(fieldImage,
                        QString::number(value).toStdString(),
                        point,
                        cv::FONT_HERSHEY_COMPLEX,
                        FONT_SCALE,
                        color,
                        2*FONT_SCALE);
        }
    }

    //calc transform matrix
    SUPoints dstFrame        = frame_.at(0);
    QRect    srcFrame        = QRect(0, 0, CELL_SIZE * 9, CELL_SIZE * 9);
    cv::Mat  transformMatrix = calcTransformMatrix(srcFrame,
                                                   dstFrame);

    //transform
    cv::warpPerspective(fieldImage, image, transformMatrix, image.size());

    //set to scene
    QPixmap pixmap;
    SuImageConverter::cvmat2qpixmap(image, pixmap);
    ui->graphicsView->scene()->addPixmap(pixmap);
}

cv::Mat MainWindow::calcTransformMatrix(QRect &srcFrame, SUPoints &dstFrame)
{
    SUPoints    points = dstFrame;
    cv::Point2f srcNode[4];
    cv::Point2f dstNode[4];
    cv::Mat     warp_mat(3, 3, CV_32FC1);

    su::PoinstComOnY pointsComOnY;
    su::PointsComOnX pointsComOnX;

    //dst nodes
    srcNode[0].x = srcFrame.x();  //left top point
    srcNode[0].y = srcFrame.y();

    srcNode[1].x = srcFrame.x() + srcFrame.width() - 1; //right top point
    srcNode[1].y = srcFrame.y();

    srcNode[2].x = srcFrame.x();   //left bottom point
    srcNode[2].y = srcFrame.y() + srcFrame.height() - 1;

    srcNode[3].x = srcFrame.x() + srcFrame.width() - 1; // right bottom point
    srcNode[3].y = srcFrame.y() + srcFrame.height() - 1;

    //copy dst nodes
    //search top and bottom points
    std::sort(points.begin(), points.end(), pointsComOnY);
    //search right and left top points
    std::sort(points.begin(), points.begin()+2, pointsComOnX);
    //serch right and left bottom points
    std::sort(points.begin()+2, points.end(), pointsComOnX);

    dstNode[0] = points.at(1);  //left top point
    dstNode[1] = points.at(0);  //right top point
    dstNode[2] = points.at(3);  //left bottom point
    dstNode[3] = points.at(2);  //right bottom point

    //transform matrix
    warp_mat = cv::getPerspectiveTransform(srcNode, dstNode);

    return warp_mat;
}

void MainWindow::setImage(cv::Mat*               image,
                          std::vector<SUPoints>* frame,
                          int                    searchStatus)
{
    assert(!image->empty());

    //save
    image_ = *image;
    delete image;

    frame_ = *frame;
    delete frame;

    //1. draw image
    drawImage(&image_);

    //2. draw frame
    if(searchStatus == SuFindFieldOnFrame::CAPTURED ||
       searchStatus == SuFindFieldOnFrame::MOVING)
    {
        drawFrame(&frame_, searchStatus);
    }


}

void MainWindow::setField(std::vector<SuFieldCell>* field,
                          int                       solveStatus)
{
    //draw numbers
    drawNumbers(field);
    delete field;

    //set browsing state
    emit setBrowsingState();

    //run sudoku game
    if(isCapturingMode_)
    {
        std::cout << "opening the game...\n";
        //TODO
    }
}

void MainWindow::setBrowsingState()
{
    ui->actionContinue->setEnabled(true);
}

void MainWindow::setContinueCapturingState()
{
    ui->actionContinue->setEnabled(false);
}

void MainWindow::showMessage(QString message)
{
    ui->statusbar->showMessage(message, timeMessage_);
}

void MainWindow::startGame(std::vector<int>* field)
{

}

void MainWindow::updatePreferences(SuAppPreferences preferences)
{
    preferences_ = preferences;
}

void MainWindow::choseCamera()
{
    InputCamId* inputCamId = new InputCamId(windowSettings_.cameraId, this);

    if(inputCamId->exec() == QDialog::Accepted)
    {
        int camId = inputCamId->camId();

        windowSettings_.cameraId = camId;

        emit videoCapture(camId);
    }

    delete inputCamId;
}

void MainWindow::choseImage()
{
    QString fileName;

    //get file name
    fileName = QFileDialog::getOpenFileName(this,
                                            "Open image",
                                            windowSettings_.folderForImages,
                                            "*");

    if(!fileName.isEmpty())
    {
        //save path to folder
        windowSettings_.folderForImages = QFileInfo(fileName).absoluteDir().path();

        //send path to file
        emit imageFromFile(fileName);
    }
}

void MainWindow::setProgramState(SuProgramState state)
{
    //mode
    emit selectedSolvingMode(state.isSolvingMode);
    if(state.isSolvingMode)
    {
        setSolvingMode(true);
        ui->actionSolve->setChecked(true);
    }

    emit selectedCapturingMode(state.isCapturingMode);
    if(state.isCapturingMode)
    {
        setCapturingMode(true);
        ui->actionCapture->setChecked(true);
    }

    //browsign
    if(state.isBrowsingState)
    {
        setBrowsingState();
        ui->actionContinue->setDisabled(true);
        return;
    }else{
        setContinueCapturingState();
    }

    //source
    switch(state.source)
    {
    case SuProgramState::SOURCE_VIDEO_MODE:
        emit videoCapture(windowSettings_.cameraId);
        break;
    case SuProgramState::SOURCE_IMAGE_FROM_FILE:
        choseImage();
        break;
    default:
        break;
    }

}

void MainWindow::showAbout()
{
    About* about = new About(this);

    about->exec();

    delete about;
}

void MainWindow::showHelp()
{
    helpDialog_->show();
}
