#include "SbPreCompile.h" //QNetworkCookie

#include "cookiejar.h"

#include "autosaver.h"

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMetaEnum>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

#include <QCompleter>
#include <QtGui/QDesktopServices>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QHeaderView>
#include <QtGui/QKeyEvent>
#include <QSortFilterProxyModel>

//#include <QtWebKit/QWebSettings>

#include <QtCore/QDebug>

static const unsigned int JAR_VERSION = 23;

QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &stream, const QList<QNetworkCookie> &list)
{
    stream << JAR_VERSION;
    stream << quint32(list.size());
    for (int i = 0; i < list.size(); ++i)
        stream << list.at(i).toRawForm();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QList<QNetworkCookie> &list)
{
    list.clear();

    quint32 version;
    stream >> version;

    if (version != JAR_VERSION)
        return stream;

    quint32 count;
    stream >> count;
    for(quint32 i = 0; i < count; ++i)
    {
        QByteArray value;
        stream >> value;
        QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(value);
        if (newCookies.count() == 0 && value.length() != 0) {
            qWarning() << "CookieJar: Unable to parse saved cookie:" << value;
        }
        for (int j = 0; j < newCookies.count(); ++j)
            list.append(newCookies.at(j));
        if (stream.atEnd())
            break;
    }
    return stream;
}
QT_END_NAMESPACE

CookieJar::CookieJar(QObject *parent)
    : QNetworkCookieJar(parent)
    , m_loaded(false)
    , m_saveTimer(new AutoSaver(this))
    , m_acceptCookies(AcceptOnlyFromSitesNavigatedTo)
{
}

CookieJar::~CookieJar()
{
    if (m_keepCookies == KeepUntilExit)
        clear();
    m_saveTimer->saveIfNeccessary();
}

void CookieJar::clear()
{
    setAllCookies(QList<QNetworkCookie>());
    m_saveTimer->changeOccurred();
    emit cookiesChanged();
} // void CookieJar::clear()

/*!
 * \brief CookieJar::load 载入已有小甜饼。
 */
void CookieJar::load()
{
    if (m_loaded)
        return;
    // load cookies and exceptions
    // qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");


    QSettings cookieSettings(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +"/data/" +  QApplication::organizationName ()+"/" + QApplication::applicationName () + QLatin1String("/cookies.ini"), QSettings::IniFormat); //打开小甜饼储存文件。


    setAllCookies(qvariant_cast<QList<QNetworkCookie> >(cookieSettings.value(QLatin1String("cookies"))));
    cookieSettings.beginGroup(QLatin1String("Exceptions"));
    m_exceptions_block = cookieSettings.value(QLatin1String("block")).toStringList();
    m_exceptions_allow = cookieSettings.value(QLatin1String("allow")).toStringList();
    m_exceptions_allowForSession = cookieSettings.value(QLatin1String("allowForSession")).toStringList();

    std::sort(m_exceptions_block.begin(), m_exceptions_block.end());

    std::sort(m_exceptions_allow.begin(), m_exceptions_allow.end());

    std::sort(m_exceptions_allowForSession.begin(), m_exceptions_allowForSession.end());

    loadSettings();
} //void CookieJar::load()

void CookieJar::loadSettings()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("cookies"));
    QByteArray value = settings.value(QLatin1String("acceptCookies"),
                        QLatin1String("AcceptOnlyFromSitesNavigatedTo")).toByteArray();
    QMetaEnum acceptPolicyEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("AcceptPolicy"));
    m_acceptCookies = acceptPolicyEnum.keyToValue(value) == -1 ?
                        AcceptOnlyFromSitesNavigatedTo :
                        static_cast<AcceptPolicy>(acceptPolicyEnum.keyToValue(value));

    value = settings.value(QLatin1String("keepCookiesUntil"), QLatin1String("KeepUntilExpire")).toByteArray();
    QMetaEnum keepPolicyEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("KeepPolicy"));
    m_keepCookies = keepPolicyEnum.keyToValue(value) == -1 ?
                        KeepUntilExpire :
                        static_cast<KeepPolicy>(keepPolicyEnum.keyToValue(value));

    if (m_keepCookies == KeepUntilExit)
        setAllCookies(QList<QNetworkCookie>());

    m_loaded = true;
    emit cookiesChanged();
}

/*!
 * \brief CookieJar::save 保存小甜饼。
 */
void CookieJar::save()
{
    if (!m_loaded)
        return;
    purgeOldCookies();



    QString directory = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +"/data/" +  QApplication::organizationName ()+"/" + QApplication::applicationName (); //小甜饼文件所在的目录。

    if (directory.isEmpty())
        directory = QDir::homePath() + QLatin1String("/.") + QCoreApplication::applicationName();
    if (!QFile::exists(directory)) {
        QDir dir;
        dir.mkpath(directory);
    }
    QSettings cookieSettings(directory + QLatin1String("/cookies.ini"), QSettings::IniFormat);
    QList<QNetworkCookie> cookies = allCookies();
    for (int i = cookies.count() - 1; i >= 0; --i) {
        if (cookies.at(i).isSessionCookie())
            cookies.removeAt(i);
    }
    cookieSettings.setValue(QLatin1String("cookies"), QVariant::fromValue<QList<QNetworkCookie> >(cookies));
    cookieSettings.beginGroup(QLatin1String("Exceptions"));
    cookieSettings.setValue(QLatin1String("block"), m_exceptions_block);
    cookieSettings.setValue(QLatin1String("allow"), m_exceptions_allow);
    cookieSettings.setValue(QLatin1String("allowForSession"), m_exceptions_allowForSession);

    // save cookie settings
    QSettings settings;
    settings.beginGroup(QLatin1String("cookies"));
    QMetaEnum acceptPolicyEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("AcceptPolicy"));
    settings.setValue(QLatin1String("acceptCookies"), QLatin1String(acceptPolicyEnum.valueToKey(m_acceptCookies)));

    QMetaEnum keepPolicyEnum = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("KeepPolicy"));
    settings.setValue(QLatin1String("keepCookiesUntil"), QLatin1String(keepPolicyEnum.valueToKey(m_keepCookies)));
}

void CookieJar::purgeOldCookies()
{
    QList<QNetworkCookie> cookies = allCookies();
    if (cookies.isEmpty())
        return;
    int oldCount = cookies.count();
    QDateTime now = QDateTime::currentDateTime();
    for (int i = cookies.count() - 1; i >= 0; --i) {
        if (!cookies.at(i).isSessionCookie() && cookies.at(i).expirationDate() < now)
            cookies.removeAt(i);
    }
    if (oldCount == cookies.count())
        return;
    setAllCookies(cookies);
    emit cookiesChanged();
}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl &url) const
{
    CookieJar *that = const_cast<CookieJar*>(this);
    if (!m_loaded)
        that->load();



    return QNetworkCookieJar::cookiesForUrl(url);
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
  if (!m_loaded)
  {
    load();
  }

  QString host = url.host();

  // bool eBlock = qBinaryFind(m_exceptions_block.begin(), m_exceptions_block.end(), host) != m_exceptions_block.end();
  bool eBlock = std::binary_search(m_exceptions_block.begin(), m_exceptions_block.end(), host) ;

  // bool eAllow = qBinaryFind(m_exceptions_allow.begin(), m_exceptions_allow.end(), host) != m_exceptions_allow.end();
  bool eAllow = std::binary_search(m_exceptions_allow.begin(), m_exceptions_allow.end(), host) ;

  // bool eAllowSession = qBinaryFind(m_exceptions_allowForSession.begin(), m_exceptions_allowForSession.end(), host) != m_exceptions_allowForSession.end();
  bool eAllowSession = std::binary_search(m_exceptions_allowForSession.begin(), m_exceptions_allowForSession.end(), host) ;

  bool addedCookies = false;
  // pass exceptions
  bool acceptInitially = (m_acceptCookies != AcceptNever);
  if ((acceptInitially && !eBlock) || (!acceptInitially && (eAllow || eAllowSession)))
  {
    // pass url domain == cookie domain
    QDateTime soon = QDateTime::currentDateTime();
    soon = soon.addDays(90);
    foreach(QNetworkCookie cookie, cookieList)
    {
      QList<QNetworkCookie> lst;
      if (m_keepCookies == KeepUntilTimeLimit && !cookie.isSessionCookie() && cookie.expirationDate() > soon)
      {
        cookie.setExpirationDate(soon);
      }
      lst += cookie;
      if (QNetworkCookieJar::setCookiesFromUrl(lst, url))
      {
        addedCookies = true;
      }
      else
      {
        // finally force it in if wanted
        if (m_acceptCookies == AcceptAlways)
        {
          QList<QNetworkCookie> cookies = allCookies();
          cookies += cookie;
          setAllCookies(cookies);
          addedCookies = true;
        }
      }
    }
  }

  if (addedCookies)
  {
    m_saveTimer->changeOccurred();
    emit cookiesChanged();
  }
  return addedCookies;
}

CookieJar::AcceptPolicy CookieJar::acceptPolicy() const
{
    if (!m_loaded)
        (const_cast<CookieJar*>(this))->load();
    return m_acceptCookies;
}

void CookieJar::setAcceptPolicy(AcceptPolicy policy)
{
    if (!m_loaded)
        load();
    if (policy == m_acceptCookies)
        return;
    m_acceptCookies = policy;
    m_saveTimer->changeOccurred();
}

CookieJar::KeepPolicy CookieJar::keepPolicy() const
{
    if (!m_loaded)
        (const_cast<CookieJar*>(this))->load();
    return m_keepCookies;
}

void CookieJar::setKeepPolicy(KeepPolicy policy)
{
    if (!m_loaded)
        load();
    if (policy == m_keepCookies)
        return;
    m_keepCookies = policy;
    m_saveTimer->changeOccurred();
}

QStringList CookieJar::blockedCookies() const
{
    if (!m_loaded)
        (const_cast<CookieJar*>(this))->load();
    return m_exceptions_block;
}

QStringList CookieJar::allowedCookies() const
{
    if (!m_loaded)
        (const_cast<CookieJar*>(this))->load();
    return m_exceptions_allow;
}

QStringList CookieJar::allowForSessionCookies() const
{
    if (!m_loaded)
        (const_cast<CookieJar*>(this))->load();
    return m_exceptions_allowForSession;
}

void CookieJar::setBlockedCookies(const QStringList &list)
{
    if (!m_loaded)
        load();
    m_exceptions_block = list;
    std::sort(m_exceptions_block.begin(), m_exceptions_block.end());
    m_saveTimer->changeOccurred();
}

void CookieJar::setAllowedCookies(const QStringList &list)
{
    if (!m_loaded)
        load();
    m_exceptions_allow = list;
    std::sort(m_exceptions_allow.begin(), m_exceptions_allow.end());
    m_saveTimer->changeOccurred();
}

void CookieJar::setAllowForSessionCookies(const QStringList &list)
{
    if (!m_loaded)
        load();
    m_exceptions_allowForSession = list;
    std::sort(m_exceptions_allowForSession.begin(), m_exceptions_allowForSession.end());
    m_saveTimer->changeOccurred();
}

CookieModel::CookieModel(CookieJar *cookieJar, QObject *parent)
    : QAbstractTableModel(parent)
    , m_cookieJar(cookieJar)
{
    connect(m_cookieJar, SIGNAL(cookiesChanged()), this, SLOT(cookiesChanged()));
    m_cookieJar->load();
}

QVariant CookieModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::SizeHintRole) {
        QFont font;
        font.setPointSize(10);
        QFontMetrics fm(font);
        int height = fm.height() + fm.height()/3;
        int width = fm.horizontalAdvance (headerData(section, orientation, Qt::DisplayRole).toString());
        return QSize(width, height);
    }

    if (orientation == Qt::Horizontal) {
        if (role != Qt::DisplayRole)
            return QVariant();

        switch (section) {
            case 0:
                return tr("Website");
            case 1:
                return tr("Name");
            case 2:
                return tr("Path");
            case 3:
                return tr("Secure");
            case 4:
                return tr("Expires");
            case 5:
                return tr("Contents");
            default:
                return QVariant();
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

/*!
 * \brief CookieModel::getDisplayOrEditRoleData 返回显示或编辑角色数据。
 * \return 显示或编辑角色数据。
 */
QVariant CookieModel::getDisplayOrEditRoleData(const QModelIndex &index,QList<QNetworkCookie> lst) const
{
    QNetworkCookie cookie = lst.at(index.row());
    switch (index.column())  //根据列来返回对应的数据。
    {
        case 0:
            return cookie.domain();
        case 1:
            return cookie.name();
        case 2:
            return cookie.path();
        case 3:
            return cookie.isSecure();
        case 4:
            return cookie.expirationDate();
        case 5:
            return cookie.value();
    } //switch (index.column())  //根据列来返回对应的数据。
    
    return QVariant();
} //QVariant CookieModel::getDisplayOrEditRoleData() const

QVariant CookieModel::data(const QModelIndex &index, int role) const
{
    QList<QNetworkCookie> lst;
    if (m_cookieJar)
        lst = m_cookieJar->allCookies();
    if (index.row() < 0 || index.row() >= lst.size())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:  //编辑角色。
    {
        return getDisplayOrEditRoleData(index, lst); //返回显示或编辑角色数据。
        
    } //case Qt::EditRole:  //编辑角色。
        
    case Qt::FontRole:{
        QFont font;
        font.setPointSize(10);
        return font;
        }
    }

    return QVariant();
}

int CookieModel::columnCount(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : 6;
}

int CookieModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() || !m_cookieJar) ? 0 : m_cookieJar->allCookies().count();
}

bool CookieModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid() || !m_cookieJar)
        return false;
    int lastRow = row + count - 1;
    beginRemoveRows(parent, row, lastRow);
    QList<QNetworkCookie> lst = m_cookieJar->allCookies();
    for (int i = lastRow; i >= row; --i) {
        lst.removeAt(i);
    }
    m_cookieJar->setAllCookies(lst);
    endRemoveRows();
    return true;
}

/*!
 * \brief CookieModel::cookiesChanged 小甜饼已经发生变化。
 */
void CookieModel::cookiesChanged()
{
    beginResetModel();
    endResetModel();
} //void CookieModel::cookiesChanged()
