#ifndef MAPREDUCEFUNCTIONS_H
#define MAPREDUCEFUNCTIONS_H

#include "TdPreCompile.h" //QList

QString recognizeFile(const QString file2Recog); //!<识别一个文件是不是有用的多媒体文件。如果是，则返回文件名。如果不是，则返回空字符串。
QList<QString> getSubDirFileList(const QString dir2Scan); //!<获取某个目录下的文件列表。
QList<QString> getSubDirs(const QString dir2Scan); //!<获取某个目录下的子目录。
void addToSubDirList(QStringList &result,const QList<QString> & partListedSubDirs); //!<将某个目录下列出的子目录归并到总的结果列表中。
void addToFileList(QStringList &result,const QList<QString> &partListedFiles); //!<将某个目录下列出的文件列表归并到总的结果列表中。
void addToMediaList(QStringList &result,const QString &fileRecognized); //!<将某个文件的识别结果添加到结果列表中。

#endif // MAPREDUCEFUNCTIONS_H

