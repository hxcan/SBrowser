
#include "SbPreCompile.h" //QVBoxLayout

#include <ValidFileName.h> //ValidFileName


#include "SWebPageWidget.h" //SWebPageWidget
#include "StbWebView.h" //StbWebView

/*!
 * \brief SWebPageWidget::connectSignals 连接信号与信号槽。
 */
void SearchEngineManager::connectSignals()
{
} //void SWebPageWidget::connectSignals()

/*!
   * \brief SWebPageWidget::SWebPageWidget 构造函数。
   * \param parent 亲代部件。
   */
SearchEngineManager::SearchEngineManager(QWidget *parent) : QWidget(parent)
{
    initValidSchemeSet(); //初始化有效的协议集合。
    
    connectSignals(); //连接信号与信号槽。
} //SWebPageWidget::SWebPageWidget

/*!
 * \brief SWebPageWidget::addQrButton 添加二维码按钮。
 * \param urlLyt 要向其中添加按钮的布局器。
 */
void SearchEngineManager::addQrButton(QHBoxLayout * urlLyt)
{
    QPushButton * qrBtn=new QPushButton; //二维码按钮。
    qrBtn->setIcon (QIcon(":/Images/stupidbeauty.80.32.png")); //设置图标。
    qrBtn->setShortcut (QKeySequence("Ctrl+R")); //设置快捷键。
    connect(qrBtn,SIGNAL(clicked()),this,SLOT(generateQrCode())); //按了二维码按钮，则生成二维码。

    urlLyt->addWidget (qrBtn); //加入按钮。
} //void SWebPageWidget::addQrButton(QHBoxLayout * urlLyt)

/*!
 * \brief SWebPageWidget::initValidSchemeSet 初始化有效的协议集合。
 */
void SearchEngineManager::initValidSchemeSet()
{
    vldSchSet << "http" << "ftp" << "https" << "file"; //加入所支持的各个协议。
} //void SWebPageWidget::initValidSchemeSet() //!<初始化有效的协议集合。

/*!
 * \brief SWebPageWidget::searchEngineSearch 按回车键，则开始搜索。
 */
void SearchEngineManager::searchEngineSearch(QString searchKeyWord)
{
    QString searchEngineName=settingsDialog->getSearchEngineName(); //获取搜索引擎的名字。
    
    QUrl searchUrl;
    
    if (searchEngineName=="Google") //谷歌。
    {
        searchUrl=constructGoogleSearchUrl(searchKeyWord); //构造谷歌的搜索网址。
    } //if (searchEngineName=="Google") //谷歌。
    else //鸭鸭去。
    {
        searchUrl=constructDuckduckgoSearchUrl(searchKeyWord); //构造谷歌的搜索网址。
    } //else //鸭鸭去。

    auto encodedUrlContent=searchUrl.toEncoded(); //Encode. 陈欣

    auto urlString=QString::fromUtf8(encodedUrlContent); //Convert to QString
    
    emit shouldOpenNewTab(urlString); //发射信号，应当打开新网页。
} //void SWebPageWidget::searchEngineSearch() //!<按回车键，则开始搜索。

/*!
 * \brief SearchEngineManager::constructGoogleSearchUrl 构造谷歌的搜索网址。
 * \param searchKeyWord 搜索关键字。
 * \return 针对谷歌的搜索网址。
 */
QUrl SearchEngineManager::constructGoogleSearchUrl(QString searchKeyWord)
{
    QString searchEnginePrefix="https://www.google.com.hk/search"; //搜索引擎的网址前缀。

    QString searchUrlString=searchEnginePrefix; //构造搜索路径字符串。
    QUrl searchUrl(searchUrlString); //构造URL。

    QUrlQuery srchQry; //查询对象。
    QByteArray srchKyWrdPcntEcd=QUrl::toPercentEncoding (searchKeyWord); //转换成百分号编码。
    srchQry.addQueryItem ("q",srchKyWrdPcntEcd); //设置查询条件。

    searchUrl.setQuery (srchQry); //设置查询对象。

    return searchUrl;
} //QUrl SearchEngineManager::constructGoogleSearchUrl(QString searchKeyWord)

/*!
 * \brief SearchEngineManager::constructDuckduckgoSearchUrl 构造鸭鸭去的搜索网址。
 * \param searchKeyWord 搜索关键字。
 * \return 针对鸭鸭去的搜索网址。
 */
QUrl SearchEngineManager::constructDuckduckgoSearchUrl(QString searchKeyWord)
{
    QString searchEnginePrefix="https://duckduckgo.com/"; //搜索引擎的网址前缀。

    QString searchUrlString=searchEnginePrefix; //构造搜索路径字符串。
    QUrl searchUrl(searchUrlString); //构造URL。

    QUrlQuery srchQry; //查询对象。
    QByteArray srchKyWrdPcntEcd=QUrl::toPercentEncoding (searchKeyWord); // 转换成百分号编码。
    srchQry.addQueryItem ("q",srchKyWrdPcntEcd); //设置查询条件。

    searchUrl.setQuery (srchQry); //设置查询对象。

    return searchUrl;
    
} //QUrl SearchEngineManager::constructDuckduckgoSearchUrl(QString searchKeyWord)

/*!
 * \brief SWebPageWidget::parseSearchEngineAutoCompleteResult 自动补全请求完成之後，處理其結果。
 */
void SearchEngineManager::parseSearchEngineAutoCompleteResult()
{


    QNetworkReply* Rst=(QNetworkReply *)(sender()); //獲取發射信號的回復對象的指針。


    QString WholeRest=""; //整个结果。

    QTextCodec * gbk_codec=QTextCodec::codecForName("UTF8"); //文字解码器。



    QByteArray ssnStrBtAr=Rst->readAll (); //读取全部内容。

    WholeRest=gbk_codec->toUnicode(  ssnStrBtAr); //转义成UTF8字符串。








    delete Rst; //释放内存。
}

StbSettingDialog *SearchEngineManager::getSettingsDialog() const
{
    return settingsDialog;
}

void SearchEngineManager::setSettingsDialog(StbSettingDialog *value)
{
    settingsDialog = value;
} //void SWebPageWidget::parseSearchEngineAutoCompleteResult()

/*!
 * \brief SWebPageWidget::requestOpenNewTab 按钮被单击，则请求新建空白标签页。
 */
void SearchEngineManager::requestOpenNewTab()
{
    emit shouldOpenNewTab (""); //请求打开空白标签页。
} //void SWebPageWidget::requestOpenNewTab()

