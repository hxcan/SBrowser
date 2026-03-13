
#ifndef COOKIEEXCEPTIONSMODEL_H
#define COOKIEEXCEPTIONSMODEL_H

#include "SbPreCompile.h" //QT_FORWARD_DECLARE_CLASS

#include <QtNetwork/QNetworkCookieJar>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QStringList>

#include <QDialog>
#include <QTableView>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QKeyEvent;
QT_END_NAMESPACE

class AutoSaver;

//#include "ui_cookiesexceptions.h"
#include "cookiejar.h" //CookieJar

QT_FORWARD_DECLARE_CLASS(CookieJar)

class CookieExceptionsModel : public QAbstractTableModel
{
    Q_OBJECT
    friend class CookiesExceptionsDialog;

public:
    CookieExceptionsModel(CookieJar *cookieJar, QObject *parent = 0);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
    CookieJar *m_cookieJar;
    QVariant getDisplayOrEditRoleData(const QModelIndex &index) const; //!<返回显示或编辑角色数据。

    // Domains we allow, Domains we block, Domains we allow for this session
    QStringList m_allowedCookies;
    QStringList m_blockedCookies;
    QStringList m_sessionCookies;
};

#endif // COOKIEJAR_H

