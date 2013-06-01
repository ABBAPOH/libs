#ifndef FILEIMAGEPROVIDER_H
#define FILEIMAGEPROVIDER_H

#include "io_global.h"

#include <QtCore/QMap>
#include <QtCore/QTime>
#include <QtCore/QReadWriteLock>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileIconProvider>
#else
#include <QtGui/QFileIconProvider>
#endif

class IO_EXPORT FileImageProvider : public QObject, public QFileIconProvider
{
public:
    FileImageProvider();

    QIcon icon(const QFileInfo &info) const;
    virtual QImage image(const QFileInfo & info, const QSize size = QSize(128, 128)) const;

protected:

private:
    mutable QReadWriteLock m_lock;
    mutable QMap<QString, QIcon> m_iconMap;
    mutable QTime m_lastCleanup;
};

#endif // FILEIMAGEPROVIDER_H
