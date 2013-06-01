#ifndef PARTS_GLOBAL_H
#define PARTS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PARTS_LIBRARY)
#  define PARTS_EXPORT Q_DECL_EXPORT
#else
#  define PARTS_EXPORT Q_DECL_IMPORT
#endif

#endif // PARTS_GLOBAL_H
