#include "SiPreCompile.h" //QTimer


#include "SImageView.h"
#include "ui_SImageView.h"

/*!
 * \brief XyRunner::XyRunner 构造函数。
 * \param parent 亲代对象。
 */
SImageView::SImageView(QWidget *parent) :QMainWindow(parent),ui(new Ui::SImageView)
{
    setAttribute(Qt::WA_TranslucentBackground); //Debug.

    ui->setupUi(this);

    initScaleTimer.setSingleShot(true); //单发。

//    setWindowState (Qt::WindowMaximized); //最大化。

    initializeMembers(); //初始化成员变量。

    createWidgets(); //创建各个部件。

    connectSignals(); //连接信号槽。

    initScaleTimer.start(); //启动初始缩放定时器。

} //XyRunner::XyRunner(QWidget *parent) :QMainWindow(parent),ui(new Ui::XyRunner)

/*!
 * \brief SImageView::initializeMembers 初始化成员变量。
 */
void SImageView::initializeMembers()
{
    //抓住拖动：
    ui->imagescrollArea->viewport ()->setAttribute (Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(ui->imagescrollArea->viewport (),QScroller::LeftMouseButtonGesture);
    
    QWidget * scrollWidget=ui->imagescrollArea->viewport();
    QScrollerProperties properties=QScroller::scroller(scrollWidget)->scrollerProperties();
    QScroller::scroller(scrollWidget)->setScrollerProperties(properties);

    
    repNode.connectToNode(QUrl(QStringLiteral("local:0000125951"))); //连接到服务器端。
    
} //void SImageView::initializeMembers()

/*!
 * \brief XyRunner::createWidgets 创建各个部件。
 */
void SImageView::createWidgets()
{
    //回收站动作：
    trashAction=new QAction(tr("Trash(T)"),this); //回收站动作。
    trashAction->setShortcut (QKeySequence("T")); //按T就触发。
    connect(trashAction,SIGNAL(triggered(bool)),this,SLOT(trashCurrentFile())); //回收站动作被触发，则将当前文件移到到回收站。
    ui->toolBar->addAction(trashAction); //加入回收站动作。


    QAction * QuitAct=new QAction(this); //退出按钮。
    QuitAct->setText (tr("Quit(Q)")); //设置文字。
    QuitAct->setShortcut (QKeySequence("Q")); //按Q就退出。
    ui->toolBar->addAction(QuitAct); //加入退出按钮。
    connect (QuitAct,SIGNAL(triggered()),this,SLOT(Quit())); //按退出按钮，则退出。

    //撤销按钮：
    QAction * undoAct=new QAction(this); //退出按钮。
    undoAct->setText (tr("Undo(N)")); //设置文字。
    undoAct->setShortcut (QKeySequence("N")); //按N就撤销。
    ui->toolBar->addAction(undoAct); //加入撤销按钮。

    //原始尺寸按钮：

    //自适应尺寸按钮：

    //全屏按钮：
} //void XyRunner::createWidgets()

/*!
 * \brief SImageView::autoFitImageSize 按自适应尺寸按钮，则显示自适应尺寸。
 */
void SImageView::autoFitImageSize()
{
//    logger()->info()  << __FILE__ << ", " << __LINE__ << ", " << __func__  <<  ", rotate degree: " << rotateDegree ; //Debug.

    QPixmap newSizePixmap=QPixmap::fromImage (staticImage); //转换成位图。

    ui->imagelabel->resize (ui->imagescrollArea->size ()); //恢复大小。

    if (newSizePixmap.width()>ui->imagelabel->width()) //位图尺寸过大。
    {
        newSizePixmap=newSizePixmap.scaledToWidth(ui->imagelabel->width(),Qt::SmoothTransformation); //缩放。
    } //if (newSizePixmap.width()>ui->imagelabel->size()) //位图尺寸过大。

    if (newSizePixmap.height()>ui->imagelabel->height()) //位图尺寸过大。
    {
        newSizePixmap=newSizePixmap.scaledToHeight(ui->imagelabel->height(),Qt::SmoothTransformation); //缩放。
    } //if (newSizePixmap.width()>ui->imagelabel->size()) //位图尺寸过大。


    showPixmapToImageLabel(newSizePixmap); //显示位图。
} //void SImageView::autoFitImageSize()

/*!
 * \brief SImageView::setPixmap 设置图片。
 * \param backgroundPixmap 要设置的图片。
 */
void SImageView::setPixmap (const QPixmap & backgroundPixmap)
{
    staticImage=backgroundPixmap.toImage();

    autoFitImageSize(); //自动适应尺寸。
} //void SImageView::setPixmap (const QPixmap & backgroundPixmap)

/*!
 * \brief SImageView::showPixmapToImageLabel 显示位图。
 * \param newSizePixmap 要显示的位图。
 */
void SImageView::showPixmapToImageLabel(QPixmap newSizePixmap)
{
    ui->imagelabel->setPixmap(newSizePixmap); //显示位图。
} //void SImageView::showPixmapToImageLabel(QPixmap newSizePixmap)

SImageView::~SImageView()
{
    delete ui;
}

void SImageView::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 * \brief SetSocketDescriptorTest::ConnectSignals 连接信号槽。
 */
void SImageView::connectSignals()
{
    connect(&initScaleTimer,SIGNAL(timeout()),this,SLOT(scalePixmapToNewSize())); //初始缩放定时器超时，则进行初始缩放。
} //void SetSocketDescriptorTest::ConnectSignals() //!<连接信号槽。

/*!
 * \brief XyRunner::Quit 按退出按钮，则退出。
 */
void SImageView::Quit()
{
//    QApplication::quit (); //退出。
    hide(); //隐藏。
} //void XyRunner::Quit() //!<按退出按钮，则退出。

/*!
 * \brief XyRunner::scalePixmapToNewSize 将位图缩放到新的尺寸。
 */
void SImageView::scalePixmapToNewSize()
{
    setWindowTitle(imageFileName+tr("<->SBrowser")); //显示窗口标题。

    autoFitImageSize(); //自适应尺寸。
} //void XyRunner::scalePixmapToNewSize()

/*!
 * \brief SImageView::resizeEvent 尺寸改变。
 * \param event 尺寸改变事件。
 */
void SImageView::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event)

    scalePixmapToNewSize(); //将位图缩放到新的尺寸。
} //void XyRunner::resizeEvent(QResizeEvent * event)
