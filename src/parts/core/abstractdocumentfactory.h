#ifndef ABSTRACTDOCUMENTFACTORY_H
#define ABSTRACTDOCUMENTFACTORY_H

#include "../parts_global.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtGui/QIcon>

namespace Parts {

class AbstractDocument;

class PARTS_EXPORT AbstractDocumentFactory : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractDocumentFactory)
public:
    explicit AbstractDocumentFactory(const QByteArray &id, QObject *parent = 0);
    ~AbstractDocumentFactory();

    AbstractDocument *document(QObject *parent);

    QByteArray id() const;
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual QStringList mimeTypes() const { return QStringList(); }
    virtual QStringList urlSchemes() const { return QStringList(); }
    virtual int weight() const { return 50; }

protected slots:
    void onDocumentDestroyed(QObject *object);

protected:
    virtual AbstractDocument *createDocument(QObject *parent) = 0;

    QList<AbstractDocument *> m_documents;
    const QByteArray m_id;

    friend class AbstractEditor;
};

} // namespace Parts

#endif // ABSTRACTDOCUMENTFACTORY_H
