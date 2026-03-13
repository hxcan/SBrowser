#ifndef LOADEVERYTHINGASIMAGE_H
#define LOADEVERYTHINGASIMAGE_H


#include "SbPreCompile.h" //QMainWindow

class TabManager; //!<QtDocChinese的前向声明。
class AboutDialog; //!<AboutDialog的前向声明。
class SWebPageWidget; //!<SWebPageWidget的前向聲明.

#include "FindDialog.h" //FindDialog

/*!
 * \brief The SBrowserWindow class 浏览器主窗口类。
 */
class LoadEverythingAsImage : public QObject
{
    Q_OBJECT

private :
    void decodeRawFile(QString imageFileName); //!<解码选中的图片。

public :
    QImage parseImage(QString imageFileName); //!<解析成图片。
    QImage parseImageRaw(QString imageFileName); //!< 解析成图片。

signals:
    void restoreClosedTabToolTipChanged(QString trRestoretabcurrentUrl); //!<信号，恢复标签页按钮的提示文字发生变化。
    void closedTabUrlStackIsEmpty(); //!<信号，关闭的标签页栈为空。
    void closedTabUrlStackIsNotEmpty (); //!<信号，已关闭的标签页栈，不再为空。
}; //class LoadEverythingAsImage : public QObject

#endif // PRECOMPILE_H
