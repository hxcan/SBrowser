#include "SbPreCompile.h" //QMainWindow

#include "ui_TabManager.h" //TabManager.ui
#include "TabManager.h" //QtDocChinese

/*!
 * \brief TabManager::focusFilterInputWidget 焦点设置到过滤内容输入框。
 */
void TabManager::focusFilterInputWidget()
{
    ui->searchlineEdit->setFocus(); //设置焦点。

    ui->searchlineEdit->selectAll(); //全选。
} //void TabManager::focusFilterInputWidget()

/*!
 * \brief BlueMainWindow::BlueMainWindow Constructor.
 * \param parent The parent widget.
 */
TabManager::TabManager(QWidget *parent) :QDialog(parent),ui(new Ui::TabManager)
{
    ui->setupUi(this);

    setWindowState (Qt::WindowMaximized); //Maximize.

    initMemberObjects(); //Initialize the member objects.

    installEventFilters(); //安装事件过滤器。

    connectSignals(); //Connect signals and slots.
}

/*!
 * \param eventTarget 事件的目标。
 * \param event 事件对象。
 */
bool TabManager::eventFilter(QObject * eventTarget, QEvent * event)
{
    bool result=false; //结果。

    if (eventTarget==ui->searchlineEdit) //是文本内容输入框。
    {
        result=handleTextContentLineEditEvent(event); //处理文本内容输入框的事件。
    } //if (eventTarget==ui->textContentlineEdit_2) //是文本内容输入框。

    return result;
} //void SWebPageWidget::eventFilter (QObject * eventTarget, QEvent * event)

/*!
 * \param event 事件对象。
 */
bool TabManager::handleTextContentLineEditEvent(QEvent * event)
{
    bool result=false; //结果。

    if (event->type ()==QEvent::KeyRelease) //按键松开事件。
    {
        QKeyEvent * currentKeyEvent=(QKeyEvent*)event; //转换成按键事件。

        if ((currentKeyEvent->key () ==Qt::Key_Up ) || ((currentKeyEvent->key () ==Qt::Key_Down ))) //上箭头。
        {
            toggleHistoryUpText(); //在历史记录中，向上切换，并且将该记录放置到文本内容输入框中。

            result=true; //消化此事件。
        } //if (currentKeyEvent->key () ==Qt::Key_Up ) //上箭头。
    } //if (event->type ()==QEvent::KeyPress) //按键按下事件。

    return result;
} //void SWebPageWidget::handleTextContentLineEditEvent(QEvent * event)

void TabManager::toggleHistoryUpText()
{
    ui->articlelistWidget->setFocus(); //设置焦点。
} //void SWebPageWidget::toggleHistoryUpText()

/*!
 * \brief TabManager::installEventFilters 安装事件过滤器。
 */
void TabManager::installEventFilters()
{
    ui->searchlineEdit->installEventFilter (this); //给文本内容输入框安装事件过滤器。
} //void TabManager::installEventFilters()

TabManager::~TabManager()
{
    delete ui;
}

/*!
 * \brief BlueMainWindow::connectSignals Connect signals and slots.
 */
void TabManager::connectSignals()
{
    QClipboard *clipboard = QApplication::clipboard();

    connect(clipboard,&QClipboard::dataChanged,this,&TabManager::checkClipboardChange); //剪贴板发生变化，则检查变化。
    
} //void BlueMainWindow::connectSignals()

/*!
 * \brief SWebPageWidget::checkClipboardChange 剪贴板发生变化，则检查变化。
 */
void TabManager::checkClipboardChange()
{
    SWebPageWidget * crtPgWdgt=(SWebPageWidget *)(pgTbs->currentWidget ()); //获取当前的标签页。

    if (crtPgWdgt) //The widget exists
    {
        crtPgWdgt->checkClipboardChange (); //检查剪贴板变动。

    } //if (crtPgWdgt) //The widget exists

} //void SWebPageWidget::checkClipboardChange()

/*!
 * \brief TabManager::setPageTabs 设置标签页部件。
 * \param pgTbs 要设置的标签页部件。
 */
void TabManager::setPageTabs(QTabWidget * pgTbs)
{
    this->pgTbs=pgTbs;
} //void TabManager::setPageTabs(QTabWidget * pgTbs)

/*!
 * \brief BlueMainWindow::initMemberObjects Initialize the member objects.
 */
void TabManager::initMemberObjects()
{
    ui->articlelistWidget->viewport ()->setAttribute (Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(ui->articlelistWidget->viewport (),QScroller::LeftMouseButtonGesture);
} //void BlueMainWindow::initMemberObjects()


/*!
 * \brief BlueMainWindow::showArticle 显示对应的文章。
 * \param currentItem 该文章的列表条目。
 */
void TabManager::showArticle(QListWidgetItem* currentItem)
{
    SWebPageWidget* qrcFileName=ListWidgetItemQrcFileNameMap[currentItem]; //获取qrc文件名。


    emit shouldSwitchTab(qrcFileName); //发射信号，应当切换标签页。

    hide();
} //void BlueMainWindow::showArticle(QListWidgetItem* currentItem)


/*!
 * \brief QtDocChinese::searchArticle 搜索文章。
 */
void TabManager::searchArticle(QString searchWord)
{
    QString searchPhrase=searchWord; //获取搜索关键字。

    int itemCounter=0; //条目计数器。
    int itemAmount=ui->articlelistWidget->count (); //获取条目个数。

    for(itemCounter=0;itemCounter<itemAmount;itemCounter++) //一个个地搜索。
    {
        QListWidgetItem * currentItem=ui->articlelistWidget->item (itemCounter); //获取当前条目。

        QString currentText=currentItem->text (); //获取文字内容。

        if (currentText.contains (searchPhrase,Qt::CaseInsensitive)) //包含文字。
        {
            currentItem->setHidden(false); //显示。
        } //if (currentText.contains (searchPhrase)) //包含文字。
        else //不包含文字。
        {
            currentItem->setHidden(true); //隐藏。
        } //else //不包含文字。
    } //for(itemCounter=0;itemCounter<itemAmount;itemCounter++) //一个个地搜索。
} //void QtDocChinese::searchArticle()

/*!
 * \brief TabManager::deleteTab 删除标签页。
 * \param crtTb 要删除的标签页对象。
 */
void TabManager::deleteTab(SWebPageWidget * crtTb)
{
    QListWidgetItem * currentItem=TabListWidgetItemMap[crtTb]; //获取当前列表条目对象。

    TabListWidgetItemMap.remove (crtTb); //删除记录。

    ListWidgetItemQrcFileNameMap.remove (currentItem); //删除映射关系。

    delete currentItem; //删除当前条目。
} //void TabManager::deleteTab(SWebPageWidget * crtTb)

/*!
 * \brief TabManager::copyList 复制列表内容
 */
void TabManager::copyList()
{
    QStringList textList;

    for(auto currentListItem: ListWidgetItemQrcFileNameMap.keys()) //一个个条目地处理
    {
        auto currentText=currentListItem->text(); //获取文字内容

        textList << currentText; //加入列表中
    } //for(auto currentListItem: ListWidgetItemQrcFileNameMap.keys()) //一个个条目地处理

    auto listTextContent=textList.join("\n"); //以换行合并起来

    QClipboard *clipboard = QApplication::clipboard();

        clipboard->setText(listTextContent);



} //void TabManager::copyList()

/*!
 * \brief QtDocChinese::addTab 加入一个标签页。
 * \param crtTbWdgt 要加入的标签页的部件。
 */
void TabManager::addTab(SWebPageWidget * crtTbWdgt)
{
    Q_UNUSED(crtTbWdgt); //不使用这个参数。

    QString currentTitle=crtTbWdgt->getTitle (); //获取标题。
    QString currentUrl=crtTbWdgt->getUrl (); //获取网址。

    QIcon currentIcon; //当前图标。


    QListWidgetItem * currentItem=new QListWidgetItem(currentIcon,currentTitle+"//"+currentUrl); //创建条目。
//    currentItem->setSizeHint (QSize(currentItem->sizeHint ().width (),32)); //设置尺寸提示。

    ListWidgetItemQrcFileNameMap[currentItem]=crtTbWdgt; //记录映射关系。
    TabListWidgetItemMap[crtTbWdgt]=currentItem; //记录映射关系。

    ui->articlelistWidget->addItem(currentItem); //添加当前条目。

    connect (crtTbWdgt,&SWebPageWidget::titleChanged, this, &TabManager::updateTabTitle); //标题改变，则更新标题。
    connect (crtTbWdgt,&SWebPageWidget::urlChanged, this, &TabManager::updateTabTitle); //网址改变，则更新标题和网址。
} //void QtDocChinese::addTab(SWebPageWidget * crtTbWdgt)

/*!
 * \brief QtDocChinese::updateTabTitle 更新标题。
 * \param newTitle 新标题。
 */
void TabManager::updateTabTitle(QString newTitle)
{
    SWebPageWidget * currentTab=(SWebPageWidget*)sender(); //获取来源标签页。

    QListWidgetItem * currentItem=TabListWidgetItemMap[currentTab]; //获取到对应的列表条目。

    QString currentUrl=currentTab->getUrl (); //获取网址。
    QString currentTitle=currentTab->getTitle (); //获取标题。

    currentItem->setText (currentTitle+"//"+currentUrl); //更新文字。


    QString currentText=newTitle; //获取文字内容。

    auto searchPhrase=ui->searchlineEdit->text (); //获取搜索关键字。

    if (currentText.contains (searchPhrase,Qt::CaseInsensitive)) //包含文字。
    {
        currentItem->setHidden(false); //显示。
    } //if (currentText.contains (searchPhrase)) //包含文字。
    else //不包含文字。
    {
        currentItem->setHidden(true); //隐藏。
    } //else //不包含文字。
} //void QtDocChinese::updateTabTitle(QString newTitle)

/*!
 * \brief QtDocChinese::resetSearch 重置搜索结果。
 * \param crtTbWdgt 要加入的标签页的部件。
 */
void TabManager::resetSearch()
{
    ui->searchlineEdit->clear (); //清空搜索条件。
    ui->searchlineEdit->setFocus (); //搜索条件获取焦点。
} //void QtDocChinese::resetSearch()
