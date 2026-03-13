#include "SbPreCompile.h" //QWebEngineScriptCollection

#include "UserScriptManager.h" //UserScriptManager

/*!
 * \brief UserScriptManager::UserScriptManager 构造函数
 */
UserScriptManager::UserScriptManager()
{
    initializeWebSettings(); //初始化网页设置。

} //UserScriptManager::UserScriptManager()

/*!
 * \brief UserScriptManager::loadUserScript 载入用户脚本。
 * \param ScriptsremoveBlmzzaqwzdllshjs 要载入的脚本路径
 * \return 载入得到的脚本对象
 */
QWebEngineScript UserScriptManager::loadUserScript(const QString & ScriptsremoveBlmzzaqwzdllshjs) const
{
    //黑命贵：
    QWebEngineScript removeBlmScript; //删除黑命贵的脚本

//    removeBlmScript=loadUserScript(":/Scripts/removeBlm.zzaqwz.dll.sh.js"); //载入黑命贵脚本。


    QFile removeBlmFile(ScriptsremoveBlmzzaqwzdllshjs); //删除黑命 贵的代码文件名

    removeBlmFile.open(QIODevice::ReadOnly);

    auto removeBlmScriptFileContent=removeBlmFile.readAll(); //全部读取

    removeBlmFile.close(); //关闭文件。


    auto removeBlmScriptCodeContent=QString::fromUtf8(removeBlmScriptFileContent); //转换成字符串

    removeBlmScript.setSourceCode(removeBlmScriptCodeContent); //设置源代码内容


    return removeBlmScript;
} //QWebEngineScript UserScriptManager::loadUserScript(const QString & ScriptsremoveBlmzzaqwzdllshjs) const

/*!
 * \brief SBrowserWindow::initializeWebSettings 初始化网页设置。
 */
void UserScriptManager::initializeWebSettings()
{
    //黑命贵：
    QWebEngineScript removeBlmScript; //删除黑命贵的脚本

    removeBlmScript=loadUserScript(":/Scripts/removeBlm.zzaqwz.dll.sh.js"); //载入黑命贵脚本。


    //谷歌广告：
    QWebEngineScript removeBlmScriptAdsByGoogle; //删除黑命贵的脚本

    removeBlmScriptAdsByGoogle=loadUserScript(":/Scripts/removeAdsByGoogle.zzaqvi.dll.sh.js"); //删除黑命 贵的代码文件名

//    :/Scripts/removeAdsByGoogle.zzaqvi.dll.sh.js


    //黑社区平权：
    QWebEngineScript blackEqualityremoveBlmScriptAdsByGoogle; //删除黑命贵的脚本

    blackEqualityremoveBlmScriptAdsByGoogle=loadUserScript(":/Scripts/removeAndroidBlackRacialEquality.js"); //删除黑命 贵的代码文件名


    //百度广告：
//    :/Scripts/removeAdsByBAidu.zzaqvi.dll.sh.js
    QWebEngineScript baiduAdsEqualityremoveBlmScriptAdsByGoogle; //删除黑命贵的脚本

    baiduAdsEqualityremoveBlmScriptAdsByGoogle=loadUserScript(":/Scripts/removeAdsByBAidu.zzaqvi.dll.sh.js"); //删除黑命 贵的代码文件名





    QWebEngineProfile * globalProfile=QWebEngineProfile::defaultProfile(); //默认配置集

    QWebEngineScriptCollection * scriptCollecton=globalProfile->scripts(); //获取脚本集合

    scriptCollecton->insert(removeBlmScript); //插入脚本，删除黑命贵
    scriptCollecton->insert(removeBlmScriptAdsByGoogle); //插入脚本，删除黑命贵
    scriptCollecton->insert(blackEqualityremoveBlmScriptAdsByGoogle); //插入脚本，删除黑命贵
    scriptCollecton->insert(baiduAdsEqualityremoveBlmScriptAdsByGoogle); //插入脚本，删除黑命贵

} //void SBrowserWindow::initializeWebSettings()



/*!
 * \brief UserScriptManager::~UserScriptManager 析构函数
 */
UserScriptManager::~UserScriptManager()
{

} //UserScriptManager::~UserScriptManager()
