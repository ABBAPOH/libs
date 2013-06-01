#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include "../parts_global.h"

#include <QtCore/QUrl>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

namespace Parts {

class AbstractDocument;
class IFind;

class PARTS_EXPORT AbstractEditor : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractEditor)

public:
    explicit AbstractEditor(AbstractDocument &document, QWidget *parent = 0);
    ~AbstractEditor();

    AbstractDocument *document() const;
    virtual void setDocument(AbstractDocument *document);

    virtual IFind *find() const;

    virtual void restoreDefaults() {}
    virtual bool restoreState(const QByteArray &state);
    virtual QByteArray saveState() const;

signals:
    void documentChanged();

private:
    AbstractDocument *m_document;
};

} // namespace Parts

#endif // ABSTRACTEDITOR_H
