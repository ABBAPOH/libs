#ifndef FILECOPYTASK_H
#define FILECOPYTASK_H

#include "filemanager_global.h"

#include <QtCore/QObject>
#include <QtCore/QMap>

#include <IO/QFileCopier>

class QTimeEvent;

namespace FileManager {

class FileCopyTaskPrivate;
class FILEMANAGER_EXPORT FileCopyTask : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FileCopyTask)
    Q_DISABLE_COPY(FileCopyTask)

public:
    explicit FileCopyTask(QObject *parent = 0);
    ~FileCopyTask();

    QFileCopier *copier() const;
    void setCopier(QFileCopier *copier);

    QString currentFilePath();

    int objectsCount() const;
    int totalObjects() const;

    int speed() const;

    int remainingTime() const;

    qint64 finishedSize() const;
    qint64 totalSize() const;

signals:
    void updated();
    void currentProgress(qint64 progress);
    void progress(qint64 progress);

protected:
    void timerEvent(QTimerEvent *);

    FileCopyTaskPrivate *d_ptr;
};

} // namespace FileManager

#endif // FILECOPYTASK_H
