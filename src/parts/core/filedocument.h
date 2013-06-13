#ifndef FILEDOCUMENT_H
#define FILEDOCUMENT_H

#include "../parts_global.h"

#include <Parts/AbstractDocument>

namespace Parts {

class FileDocumentPrivate;
class PARTS_EXPORT FileDocument : public AbstractDocument
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FileDocument)
    Q_DISABLE_COPY(FileDocument)

public:
    explicit FileDocument(QObject *parent = 0);
    ~FileDocument();

public slots:
    void clear();
    void save(const QUrl &url);

protected slots:
    void onReadyRead();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReplyFinished();

protected:
    FileDocument(FileDocumentPrivate &dd, QObject *parent);

    bool openUrl(const QUrl &url);

    virtual bool read(QIODevice *device, const QString &fileName) = 0;
    virtual bool write(QIODevice *device, const QString &fileName);
};

} // namespace Parts

#endif // FILEDOCUMENT_H
