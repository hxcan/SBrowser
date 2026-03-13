/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

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



CookieExceptionsModel::CookieExceptionsModel(CookieJar *cookiejar, QObject *parent)
    : QAbstractTableModel(parent)
    , m_cookieJar(cookiejar)
{
    m_allowedCookies = m_cookieJar->allowedCookies();
    m_blockedCookies = m_cookieJar->blockedCookies();
    m_sessionCookies = m_cookieJar->allowForSessionCookies();
}

QVariant CookieExceptionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::SizeHintRole) {
        QFont font;
        font.setPointSize(10);
        QFontMetrics fm(font);
        int height = fm.height() + fm.height()/3;
        int width = fm.horizontalAdvance (headerData(section, orientation, Qt::DisplayRole).toString());
        return QSize(width, height);
    }

    if (orientation == Qt::Horizontal
        && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Website");
            case 1:
                return tr("Status");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

/*!
 * \brief CookieModel::getDisplayOrEditRoleData 返回显示或编辑角色数据。
 * \return 显示或编辑角色数据。
 */
QVariant CookieExceptionsModel::getDisplayOrEditRoleData(const QModelIndex &index) const
{
    int row = index.row();
    if (row < m_allowedCookies.count()) {
        switch (index.column()) {
            case 0:
                return m_allowedCookies.at(row);
            case 1:
                return tr("Allow");
        }
    }
    row = row - m_allowedCookies.count();
    if (row < m_blockedCookies.count()) {
        switch (index.column()) {
            case 0:
                return m_blockedCookies.at(row);
            case 1:
                return tr("Block");
        }
    }
    row = row - m_blockedCookies.count();
    if (row < m_sessionCookies.count()) {
        switch (index.column()) {
            case 0:
                return m_sessionCookies.at(row);
            case 1:
                return tr("Allow For Session");
        }
    }
    
    return QVariant();
} // QVariant CookieModel::getDisplayOrEditRoleData() const


QVariant CookieExceptionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: //编辑角色。
    {
        return getDisplayOrEditRoleData(index); //返回显示或编辑角色的数据。
        

        
    } //case Qt::EditRole: //编辑角色。
        
    case Qt::FontRole:{
        QFont font;
        font.setPointSize(10);
        return font;
        }
    }
    return QVariant();
}

int CookieExceptionsModel::columnCount(const QModelIndex &parent) const
{
    return (parent.isValid()) ? 0 : 2;
}

int CookieExceptionsModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() || !m_cookieJar) ? 0 : m_allowedCookies.count() + m_blockedCookies.count() + m_sessionCookies.count();
}

bool CookieExceptionsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid() || !m_cookieJar)
        return false;

    int lastRow = row + count - 1;
    beginRemoveRows(parent, row, lastRow);
    for (int i = lastRow; i >= row; --i) {
        if (i < m_allowedCookies.count()) {
            m_allowedCookies.removeAt(row);
            continue;
        }
        i = i - m_allowedCookies.count();
        if (i < m_blockedCookies.count()) {
            m_blockedCookies.removeAt(row);
            continue;
        }
        i = i - m_blockedCookies.count();
        if (i < m_sessionCookies.count()) {
            m_sessionCookies.removeAt(row);
            continue;
        }
    }
    m_cookieJar->setAllowedCookies(m_allowedCookies);
    m_cookieJar->setBlockedCookies(m_blockedCookies);
    m_cookieJar->setAllowForSessionCookies(m_sessionCookies);
    endRemoveRows();
    return true;
} //bool CookieExceptionsModel::removeRows(int row, int count, const QModelIndex &parent)

