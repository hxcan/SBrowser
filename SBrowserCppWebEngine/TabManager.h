#ifndef TAB_MANAGER_H
#define TAB_MANAGER_H

#include "SbPreCompile.h" //QListWidgetItem

#include "SWebPageWidget.h" //SWebPageWidget
#include "ui_TabManager.h" //TabManager

namespace Ui
{
    class TabManager; //TabManager
}

class SWebPageWidget; //SWebPageWidget

class TabManager : public QDialog
{
    Q_OBJECT

protected:
    bool eventFilter (QObject * eventTarget, QEvent * event); //!<事件过滤器。

public:
    void setPageTabs(QTabWidget * pgTbs); //!<设置标签页部件。
    void focusFilterInputWidget(); //!<焦点设置到过滤内容输入框。
    void deleteTab(SWebPageWidget * crtTb); //!<删除标签页。
    void addTab(SWebPageWidget * crtTbWdgt); //!<加入一个标签页。
    explicit TabManager(QWidget *parent = nullptr);
    ~TabManager();

private:
    QTabWidget * pgTbs; //!<用于容纳各个网页的标签页部件。
    void toggleHistoryUpText(); //!<在历史记录中，向上切换，并且将该记录放置到文本内容输入框中。
    bool handleTextContentLineEditEvent(QEvent * event); //!<处理文本内容输入框的事件。
    void installEventFilters(); //!<安装事件过滤器。
    void updateTabTitle(QString newTitle); //!<更新标题。
    QMap<QListWidgetItem*, SWebPageWidget *> ListWidgetItemQrcFileNameMap; //!<列表部件条目与标签页部件之间的映射。
    QMap<SWebPageWidget *, QListWidgetItem*> TabListWidgetItemMap; //!<标签页部件与列表部件条目之间的映射。
    void initMemberObjects(); //!<Initialize the member objects.
    void connectSignals(); //!<Connect signals and slots.
    Ui::TabManager *ui; //!<用户界面。
    void checkClipboardChange(); //!<剪贴板发生变化，则检查变化。

signals:
    void shouldSwitchTab(SWebPageWidget * qrcFileName); //!<信号，应当切换标签页。

private slots:
    void resetSearch(); //!<重置搜索结果。
    void showArticle(QListWidgetItem* currentItem); //!<显示对应的文章。
    void searchArticle(QString searchWord); //!<搜索文章。
    void copyList(); //!<复制列表内容
}; //class BlueMainWindow : public QMainWindow

#endif
