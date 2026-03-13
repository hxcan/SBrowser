#ifndef ERROR_CODE_SUBSYSTEM_GLOBAL_H
#define ERROR_CODE_SUBSYSTEM_GLOBAL_H

#include <QtCore/qglobal.h>


#ifdef _MSC_VER 
//#pragma warning (push)
#pragma warning (disable: 4100 4189 4251)
//#pragma warning (pop)
#endif


#if defined(ERROR_CODE_SUB_SYSTEM)

#if defined(ERROR_CODE_SUB_SYSTEM_EXPORT)
#else //#if defined(RPC_SUB_SYSTEM_EXPORT)
#  define ERROR_CODE_SUB_SYSTEM_EXPORT Q_DECL_EXPORT
#endif //#if defined(RPC_SUB_SYSTEM_EXPORT)

#else //#if defined(RPC_SUB_SYSTEM_EXPORT)
#  define ERROR_CODE_SUB_SYSTEM_EXPORT Q_DECL_IMPORT
#endif //#if defined(RPC_SUB_SYSTEM_EXPORT)

#endif // XY_GLOBAL_H
