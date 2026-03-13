#ifndef SEARCHENGINEMANAGER_H
#define SEARCHENGINEMANAGER_H

#include "SbPreCompile.h" // QLineEdit

#include "StbSettingDialog.h" //SettingsDialog

class SearchEngineManager :public QWidget
{
    Q_OBJECT

private slots:
    void parseSearchEngineAutoCompleteResult(); //!<自动补全请求完成之後，處理其結果。

private :
    QUrl constructGoogleSearchUrl(QString searchKeyWord); //!<构造谷歌的搜索网址。
    QUrl constructDuckduckgoSearchUrl(QString searchKeyWord); //!<构造鸭鸭去的搜索网址。
    StbSettingDialog * settingsDialog; //!<设置对话框。
    QMenu * reloadMenu; //!<重新载入的菜单。
    void requestOpenNewTab(); //!<按钮被单击，则请求新建空白标签页。
    void connectSignals(); //!<连接信号与信号槽。
    void addQrButton(QHBoxLayout * urlLyt); //!<添加二维码按钮。
    void initValidSchemeSet(); //!<初始化有效的协议集合。
    QSet<QString> vldSchSet; //!<本浏览器所支持的协议集合。

signals:
    void shouldQuerySearchEngine(QString searchKeyWord); //!<信号，应当向搜索引擎查询。
    void windowCloseRequested(); //!<要求关闭窗口。
    void zoomFactorChanged(qreal newZoomFactor); //!<信号，缩放比例发生变化。
    void loadFinished(); //!<信号，载入完毕。
    void toggleProxyRule(bool isCkd,QString hstNm); //!<信号，要求切换针对当前域名的智能代理规则。
    void shouldOpenNewTab(QString url2Open); //!<应当打开新标签页。
    void titleChanged(QString);
    void iconChanged(QIcon); //!<图标发生变化。

public:
    void searchEngineSearch(QString searchKeyWord); //!<按回车键，则开始搜索。
    SearchEngineManager(QWidget * parent=0); //!<构造函数。
    StbSettingDialog *getSettingsDialog() const;
    void setSettingsDialog(StbSettingDialog *value);
}; //class SWebPageWidget :public QWidget

#endif // PRECOMPILE_H
