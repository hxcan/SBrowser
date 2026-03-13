#ifndef OPERATIONTYPE_H
#define OPERATIONTYPE_H

#include "SiPreCompile.h" //QNetworkRequest

namespace Ui {
class SImageView;
}

enum OperationType
{
    Move, //!<移动文件。
    Trash, //!<移动到回收站。
    NoneOperationType, //!<无。
};

#endif //#ifndef OPERATIONTYPE_H
