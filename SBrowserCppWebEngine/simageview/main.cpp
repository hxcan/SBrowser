#include "SbPreCompile.h" //InitializeMagick

#include "SImageView.h"

static void initializeRootLogger();
static void shutdownRootLogger();
static void setupRootLogger(const QString &introMessage);
static void shutDownRootLogger(const QString &extroMessage);


int main(int argc, char *argv[])
{
    InitializeMagick(*argv); //初始化GraphicsMagick++.

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed (false); //关闭窗口的情况下还不退出。
    a.setOrganizationName ("StupidBeauty"); //设置组织名字。
    a.setOrganizationDomain ("stupidbeauty.com"); //设置组织域名。
    a.setApplicationName ("SImageView"); //设置程序名字。要读取相同的配置文件。后续重构成显式指定配置文件名。

    initializeRootLogger();
    

    QTranslator CnTrns;
    CnTrns.load(":/XyRunner"); //载入翻译文件。
    QTranslator XyTrns;
    XyTrns.load("Xy"); //载入翻译文件。

    a.installTranslator(&CnTrns); //安装翻译文件。
    a.installTranslator (&XyTrns); //安装翻译文件。

    SImageView w;
    w.show();

    int result=a.exec();
    
    shutdownRootLogger();
    
    
    return result;
}

void initializeRootLogger()
{
    setupRootLogger(QObject::tr("Start running, %1").arg(QDateTime::currentDateTime().toString()));
} //void initializeRootLogger()

void shutdownRootLogger()
{
    shutDownRootLogger(QString());
} //void shutdownRootLogger()

void setupRootLogger(const QString &introMessage)
{
    auto logger = Log4Qt::Logger::rootLogger();
    Log4Qt::TTCCLayout *layout = new Log4Qt::TTCCLayout();
    layout->setName(QLatin1String("My Layout"));
    layout->activateOptions();
    
    Log4Qt::ConsoleAppender *consoleAppender = new Log4Qt::ConsoleAppender(layout, Log4Qt::ConsoleAppender::STDOUT_TARGET);
    
    consoleAppender->setName(QLatin1String("My Appender"));
    consoleAppender->activateOptions();
    
    logger->addAppender(consoleAppender);
    
    QDir downloadDir(QStandardPaths::writableLocation (QStandardPaths::HomeLocation)+QObject::tr("/.simageview/log")); //下载目录。

    downloadDir.mkpath ((QStandardPaths::HomeLocation)+QObject::tr("/.simageview/log")); //创建目录。
    
    QString dateTimeString=QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss.zzz"); //日期时间字符串。
    
    
    
    Log4Qt::FileAppender *fileAppender = new Log4Qt::FileAppender(layout, downloadDir.absolutePath()+QObject::tr("/simageview.%1.log").arg(dateTimeString), true);
    fileAppender->setName(QLatin1String("My file appender"));
    fileAppender->activateOptions();
    
    logger->addAppender(fileAppender);
    
    logger->setLevel(Log4Qt::Level::INFO_INT);
    
    Log4Qt::LogManager::setHandleQtMessages(true);
    
    if (!introMessage.isEmpty())
    {
        logger->info(introMessage);
    } //if (!introMessage.isEmpty())
    
    qWarning("Handling Qt messages is enabled");
} //void setupRootLogger(const QString &introMessage)

void shutDownRootLogger(const QString &extroMessage)
{
    auto logger = Log4Qt::Logger::rootLogger();
    
    if (!extroMessage.isEmpty())
    {
        logger->info(extroMessage);
    } //if (!extroMessage.isEmpty())
    
    logger->removeAllAppenders();
    logger->loggerRepository()->shutdown();
} //void shutDownRootLogger(const QString &extroMessage)
