#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QThread>

void MainWindow::startVpn() {    
    //mouseL();
    //mouseMove(340, 570);

    moveLclickAndReturn( 577,  47); // close Pidgin
    moveLclickAndReturn( 597, 594); // close Pidgin Under
    moveLclickAndReturn(1902, 243); // close Skype

    moveLclickAndReturn( 40,  610);  // call VpnWindow
    //moveLclickAndReturn( 350, 575);  // click Connect

    moveLLclickAndReturn( 136,1010);  // open txt
    moveLLLclickAndReturn(637, 120);  // select gitlab link

    moveRclickAndReturn( 537, 120);  // R click menu
    moveLclickAndReturn( 600, 238);  // Copy
    moveLclickAndReturn(1820,  55);  // hide

    moveLclickAndReturn(  36, 680);  // Open Firefox
    moveLclickAndReturn(1720,  48);  // click +
    moveRclickAndReturn( 622,  95);  // R click menu
    moveLclickAndReturn( 692, 239);  // paste and go

    //
}

void MainWindow::startMR() {
    moveLLclickAndReturn( 136,1010);  // open txt
    moveLLLclickAndReturn(637, 120);  // select gitlab link

    moveRclickAndReturn( 537, 120);  // R click menu
    moveLclickAndReturn( 600, 238);  // Copy
    //moveLclickAndReturn(1820,  55);  // hide

    moveLclickAndReturn(  36, 680);  // Open Firefox
    moveLclickAndReturn(1720,  48);  // click +
    moveRclickAndReturn( 622,  95);  // R click menu
    moveLclickAndReturn( 692, 239);  // paste and go

    moveLclickAndReturn(612, 406);   // Create Merge Request

    // GOS-3243: net-mgmt/wifi-galaxy 1.0.33 Error window shows up when connection was Successfull
    // Closes GOS-3243
    moveLLLclickAndReturn(517, 473); // select title mouse on 'G'
    moveLclickAndReturn( 521, 641); // select Description mouse on 'C'
    // GOS-3243: net-mgmt/wifi-galaxy 1.0.33
    // changes:
    //
    //* GOS-3243: Error window shows up when connection was Successfull
    moveLclickAndReturn(517, 904);  // click Assignees
    moveLclickAndReturn(554, 667);  // select PAO
    moveLclickAndReturn(511, 979);  // click Reviewers
    moveLclickAndReturn(557, 746 ); // select PAO

    moveLclickAndReturn(1914, 890); // scroll down
    moveLclickAndReturn(504 , 663); // check Squash
    moveLclickAndReturn(556 , 678); // click Create MR

    // 30 secs wait
    QThread::sleep(30);

    moveLclickAndReturn( 166 , 90 ); // click reaload
    moveLclickAndReturn( 740 ,868 ); // Set automerge
}

void MainWindow::on_pushButton_clicked()
{ timerLoop(3000); getCurPos(xStart, yStart); setMouseToLeftTop(xStart, yStart); }
void MainWindow::on_pushButton_2_clicked()
{ QTimer::singleShot(5000, [=]() { int xx = 0, yy = 0; getCurPos(xx, yy);
    ui->lineEditL->setText(QString::number(xx));
    ui->lineEditR->setText(QString::number(yy));});
}
void MainWindow::on_pushButton_3_clicked()
{ timerMR(5000); getCurPos(xStart, yStart); setMouseToLeftTop(xStart, yStart); }
void MainWindow::on_pushButton_4_clicked()
{ timerVpn(5000); getCurPos(xStart, yStart); setMouseToLeftTop(xStart, yStart); }

void MainWindow::setMouseToLeftTop(int xStart, int yStart)
{ mouseMove(-xStart, -yStart); }



void MainWindow::on_pushButton_5_clicked() {
    getCurPos(xStart, yStart);
    setMouseToLeftTop(xStart, yStart);
    int msecs = ui->lineEditL_2->text().toInt();
    timeraAutoclick->stop();
    timeraAutoclick->start(msecs);
}

void MainWindow::on_pushButton_6_clicked() {
    timeraAutoclick->stop();
}

void MainWindow::autoClickLoop() {
    static bool t = true;
    if(t) { qDebug() << "1200 x 600";
        mouseMove(1200, 600);
    } else { qDebug() << "-1200 x -600";
        mouseMove(-1200, -600); mouseL();
    }
    t = !t;
}

void MainWindow::moveLclickAndReturn(int x, int y) {
    mouseMove( x,  y); mouseL(); mouseMove( -x,  -y); QThread::sleep(2); // close Pidgin
}
void MainWindow::moveLLclickAndReturn(int x, int y) {
    mouseMove( x,  y); mouseDoubleL(); mouseMove( -x,  -y); QThread::sleep(2); // close Pidgin
}
void MainWindow::moveLLLclickAndReturn(int x, int y) {
    mouseMove( x,  y); mouseTrippleL(); mouseMove( -x,  -y); QThread::sleep(2); // close Pidgin
}
void MainWindow::moveRclickAndReturn(int x, int y) {
    mouseMove( x,  y); mouseR(); mouseMove( -x,  -y); QThread::sleep(2); // close Pidgin
}

void MainWindow::startMouseMoveLoop()
{
    static bool t = true;
    if(t) {
        t = !t;
        qDebug() << "600 x 600";
        //mouseMove(50, 150);
        //mouseMove(600, 600);
        mouseMove(1200, 600);
    } else {
        //mouseMove(-600, -600);
        t = !t;
        qDebug() << "-600 x -600";
        //QCursor :: setPos (1000, 800);
        //clickMouse();

        mouseL();
        //mouseDoubleL();
        mouseR();
    }
}


void MainWindow::mouseL()
{
    system ("xdotool click 1");
}
void MainWindow::mouseDoubleL()
{
    system ("xdotool click 1");
    system ("xdotool click 1");
}
void MainWindow::mouseTrippleL()
{
    system ("xdotool click 1");
    system ("xdotool click 1");
    system ("xdotool click 1");
}
void MainWindow::mouseR()
{
    system ("xdotool click 3");
}

void MainWindow::click2()
{
    int delta_x = 0, delta_y = 0;

    Display *display = XOpenDisplay(0);

    Window root = DefaultRootWindow(display);

    XWarpPointer(display, None, root, 0, 0, 0, 0, delta_x, delta_y);
    Display *display2;

    if ((display2 = XOpenDisplay(NULL)) == NULL) {
    qDebug() << ("Cannot open local X-display.\n");
        return;
    }

    mouseClick(0);

    XCloseDisplay(display);
}

/////////////////////////////////////////////////////
void MainWindow::mouseClick(int button)
{
    qDebug() << ("in mouse click\n");
    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
    //fprintf(stderr, "Errore nell'apertura del Display !!!\n");
    qDebug() << ("Errore nell'apertura del Display !!! \n");
    exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    qDebug() << ("PUSHING BUTTON1 !!! \n");
    //event.xbutton.button = Button1;
    event.xbutton.button = Button2;
    event.xbutton.same_screen = True;

    XQueryPointer(display
        , RootWindow(display, DefaultScreen(display))
        , &event.xbutton.root
        , &event.xbutton.window
        , &event.xbutton.x_root
        , &event.xbutton.y_root
        , &event.xbutton.x
        , &event.xbutton.y
        , &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0)
    qDebug() <<(" 2 Errore nell'apertura del Display !!! \n");
    //fprintf(stderr, "Errore nell'invio dell'evento !!!\n");

    XFlush(display);

    usleep(1000);
    qDebug() << ("done sleeping !!! \n");
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    qDebug() << ("button pushed? !!! \n");

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0)
        qDebug() << (" 3 Errore nell'apertura del Display !!! \n");

    XFlush(display);

    XCloseDisplay(display);
}
/////////////////////////////////////////////////////

void MainWindow::clickMouse()
{
    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
        fprintf(stderr, "Errore nell'apertura del Display !!!\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    //event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Errore nell'invio dell'evento !!!\n");

    XFlush(display);

    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Errore nell'invio dell'evento !!!\n");

    XFlush(display);

    XCloseDisplay(display);
}

void MainWindow::timerVpn(int msecs)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::startVpn);
    timer->setSingleShot(true);
    timer->start(msecs); // И запустим таймер*/
}

void MainWindow::timerMR(int msecs)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::startMR);
    timer->setSingleShot(true);
    timer->start(msecs); // И запустим таймер*/
}

void MainWindow::timerLoop(int msecs)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::startMouseMoveLoop);
    timer->start(msecs); // И запустим таймер*/
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Display* dpy = XOpenDisplay(0);
    int scr = XDefaultScreen(dpy);
    Window root_window = XRootWindow(dpy, scr);
    int height = DisplayHeight(dpy, scr);
    int width  = DisplayWidth(dpy, scr);
    qDebug() << "Screen size : " << width << "x" << height ;

    timeraAutoclick = new QTimer(this);
    connect(timeraAutoclick, &QTimer::timeout, this, &MainWindow::autoClickLoop);
    ui->lineEditL_2->setText("120000");

    /*Cursor csr ;
    Window rootw = XDefaultRootWindow(dpy);
    XEvent evt;
    int err = XGrabPointer(dpy,
               rootw,
               false,
               ButtonPressMask,
               GrabModeAsync,
               GrabModeAsync,
               None,
               csr,
               CurrentTime);
    switch(err) {
        int error = err;
        error = error + 1;
    }
    XNextEvent(dpy, &evt);

    qDebug()
    << QString("Absolute coordinates: %1 | %2")
       .arg(evt.xbutton.x_root)
       .arg(evt.xbutton.y_root);
    */

    /*
    c = display.Display().screen().root.query_pointer()._data
    x = c["root_x"]
    y = c["root_y"]

    //POINT p;
    if (GetCursorPos(&p)) { //cursor position now in p.x and p.y
    } // This returns the cursor position relative to screen coordinates. Call ScreenToClient to map to window coordinates.
    if (ScreenToClient(hwnd, &p)) { //p.x and p.y are now relative to hwnd's client area
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

static int _XlibErrorHandler(Display *display, XErrorEvent *event) {
    fprintf(stderr, "An error occured detecting the mouse position\n");
    return True;
}

void MainWindow::getCurPos(int& xx, int& yy)
{
    int number_of_screens = 1;
    int i;
    Bool result;
    Window *root_windows;
    Window window_returned;
    int root_x, root_y;
    int win_x, win_y;
    unsigned int mask_return;

    Display *display = XOpenDisplay(NULL);
    assert(display);
    XSetErrorHandler(_XlibErrorHandler);
    number_of_screens = XScreenCount(display);
    //fprintf(stderr, "There are %d screens available in this X session\n", number_of_screens);
    root_windows = (Window *)(malloc(sizeof(Window) * number_of_screens));
    for (i = 0; i < number_of_screens; i++) {
        root_windows[i] = XRootWindow(display, i);
    }
    for (i = 0; i < number_of_screens; i++) {
        result = XQueryPointer(display, root_windows[i], &window_returned,
                &window_returned, &root_x, &root_y, &win_x, &win_y,
                &mask_return);
        if (result == True) {
            break;
        }
    }
    if (result != True) {
            qDebug() << "(" << stderr << ") No mouse found";
        //return -1;
    }
    qDebug() << QString("Mouse is at (%1 | %2)").arg(root_x).arg(root_y);

    free(root_windows);
    XCloseDisplay(display);

    xx = root_x;
    yy = root_y;
}

void MainWindow::mouseMove(int x, int y)
{
    Display *displayMain = XOpenDisplay(NULL);
    if(displayMain == NULL) {
        qDebug() << "(" << stderr <<  ") Errore nell'apertura del Display !!!\n";
        //exit(EXIT_FAILURE);
    }
    XWarpPointer(displayMain, None, None, 0, 0, 0, 0, x, y);
    XCloseDisplay(displayMain);

    /* Display* dpy = XOpenDisplay(0);
    int scr = XDefaultScreen(dpy);
    Window root_window = XRootWindow(dpy, scr);
    int height = DisplayHeight(dpy, scr);
    int width  = DisplayWidth(dpy, scr);
    qDebug() << "Screen size : " << width << "x" << height ;
    float m = (float)height/(float)width;
    /*
    int j; for(int i=0; i<width; i++){
        j = m*i;
        XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, i, j);
        XFlush(dpy);
    }*/
}



