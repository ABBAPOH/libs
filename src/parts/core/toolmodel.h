#ifndef TOOLMODEL_H
#define TOOLMODEL_H

#include "../parts_global.h"

#include <QtCore/QObject>

namespace Parts {

class AbstractDocument;

class ToolModelPrivate;
class PARTS_EXPORT ToolModel : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ToolModel)
    Q_DISABLE_COPY(ToolModel)

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)

public:
    explicit ToolModel(QObject *parent = 0);
    ~ToolModel();

    AbstractDocument *document() const;
    virtual void setDocument(AbstractDocument *document);

    QString title() const;

signals:
    void documentChanged();
    void titleChanged(const QString &title);

protected:
    void setTitle(const QString &title);

protected:
    ToolModelPrivate *d_ptr;
};

} // namespace Parts

#endif // TOOLMODEL_H
