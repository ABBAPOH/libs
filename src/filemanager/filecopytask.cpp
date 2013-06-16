#include "filecopytask.h"
#include "filecopytask_p.h"

#include <QtCore/QFileInfo>
#include <QtCore/QTimerEvent>

using namespace FileManager;

FileCopyTaskPrivate::FileCopyTaskPrivate(FileCopyTask *qq) :
    q_ptr(qq),
    copier(0),
    objectsCount(0),
    totalObjects(0),
    speed(0),
    speedLastSize(0),
    currentProgress(0),
    finishedSize(0),
    totalSize(0)
{
}

void FileCopyTaskPrivate::onStateChanged(QFileCopier::State state)
{
    QFileCopier *copier = qobject_cast<QFileCopier *>(sender());
    Q_ASSERT(copier);

    if (state == QFileCopier::Gathering) {
        reset();
    } else if (state == QFileCopier::Copying) {
        totalObjects = copier->count();
        totalSize = copier->totalSize();
    }
}

void FileCopyTaskPrivate::onStarted(int /*id*/)
{
    currentProgress = 0;
    emit q_func()->updated();
}

void FileCopyTaskPrivate::onFinished(int /*id*/)
{
    objectsCount++;
    emit q_func()->updated();
}

void FileCopyTaskPrivate::onProgress(qint64 progress, qint64 /*progressTotal*/)
{
    finishedSize += progress - currentProgress;
    currentProgress = progress;

    emit q_func()->currentProgress(progress);
    emit q_func()->progress(finishedSize);
}

void FileCopyTaskPrivate::onDone()
{
    Q_Q(FileCopyTask);

    finishedSize = totalSize;
    objectsCount = totalObjects;

    emit q->currentProgress(0);
    emit q->progress(finishedSize);
    emit q->updated();
    q->deleteLater();
}

void FileCopyTaskPrivate::reset()
{
    objectsCount = 0;
    totalObjects = 0;

    speed = 0;
    speedLastSize = 0;

    currentProgress = 0;
    finishedSize = 0;
    totalSize = 0;
}

FileCopyTask::FileCopyTask(QObject *parent) :
    QObject(parent),
    d_ptr(new FileCopyTaskPrivate(this))
{
    startTimer(1000);
}

FileCopyTask::~FileCopyTask()
{
    delete d_ptr;
}

QFileCopier *FileCopyTask::copier() const
{
    return d_func()->copier;
}

void FileCopyTask::setCopier(QFileCopier *copier)
{
    Q_D(FileCopyTask);

    if (d->copier) {
        disconnect(d->copier, 0, d, 0);
    }
    if (d->copier != copier) {
        d->copier = copier;
        connect(copier, SIGNAL(stateChanged(QFileCopier::State)),
                d, SLOT(onStateChanged(QFileCopier::State)));
        connect(copier, SIGNAL(started(int)), d, SLOT(onStarted(int)));
        connect(copier, SIGNAL(finished(int,bool)), d, SLOT(onFinished(int)));
        connect(copier, SIGNAL(progress(qint64,qint64)),
                d, SLOT(onProgress(qint64,qint64)));
        connect(copier, SIGNAL(done(bool)), d, SLOT(onDone()));
    }
}

QString FileCopyTask::currentFilePath()
{
    Q_D(FileCopyTask);

    int id = d->copier->currentId();
    return d->copier->sourceFilePath(id);
}

int FileCopyTask::objectsCount() const
{
    return d_func()->objectsCount;
}

int FileCopyTask::totalObjects() const
{
    return d_func()->totalObjects;
}

int FileCopyTask::speed() const
{
    return d_func()->speed;
}

int FileCopyTask::remainingTime() const
{
    if (speed() == 0)
        return 0;

    return 1000*(qint64)(totalSize() - finishedSize())/speed();
}

qint64 FileCopyTask::finishedSize() const
{
    return d_func()->finishedSize;
}

qint64 FileCopyTask::totalSize() const
{
    return d_func()->totalSize;
}

void FileCopyTask::timerEvent(QTimerEvent * /*event*/)
{
    Q_D(FileCopyTask);

    d->speed = (qint64)(d->finishedSize - d->speedLastSize); // delta per second
    d->speedLastSize = d->finishedSize;
}
