#include "SbPreCompile.h" //QShortcut

#include "StartSlideGoBackTop.h" //StartSlideGoBackTop

/*!
 * \brief StbWebView::StbWebView 构造函数。
 * \param parent 亲代对象的指针。
 */
StartSlideGoBackTop::StartSlideGoBackTop(StbWebView * parent)
{
    masterStbWebView=parent; //记录服务对象。

    connectSignals(); //连接信号槽。
} //def initialize(parent=nil)

/*!
 * \brief StbWebView::connectSignals 连接信号槽。
 */
void StartSlideGoBackTop::connectSignals()
{

    return;
} //void StbWebView::connectSignals() //!<连接信号槽。

/*!
 * \brief StbWebView::operator () 函子操作符。
 * \param clientHeightResult 获取到的显示区高度及网页高度。
 */
void StartSlideGoBackTop::operator() (const QVariant & clientHeightResult)
{
    QString clientHeightString=clientHeightResult.toString (); //结果当成字符串来处理。

    float clientTop=clientHeightString.toFloat (); //获取显示区高度。

    masterStbWebView->startSlideGoBackTop (clientTop); //启动滑动动画过程。
} //void StbWebView::operator() (const QVariant & clientHeightResult)
