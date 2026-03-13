#ifndef SV_LOG_SUBSYSTEM_GLOBAL_H
#define SV_LOG_SUBSYSTEM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SV_LOG_SUB_SYSTEM)
#  define SV_LOG_SUB_SYSTEM_EXPORT Q_DECL_EXPORT
#else
#  define SV_LOG_SUB_SYSTEM_EXPORT Q_DECL_IMPORT
#endif

#endif // XY_GLOBAL_H
