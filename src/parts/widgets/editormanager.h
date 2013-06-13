#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include "../parts_global.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QUrl>

namespace Parts {

class AbstractDocument;
class AbstractEditor;
class AbstractEditorFactory;

class EditorManagerPrivate;
class PARTS_EXPORT EditorManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(EditorManager)
    Q_DISABLE_COPY(EditorManager)

public:
    typedef QList<AbstractEditorFactory *> FactoryList;

    explicit EditorManager(QObject *parent = 0);
    ~EditorManager();

    static EditorManager *instance();

    AbstractEditorFactory *factory(const QByteArray &id) const;
    AbstractEditor *createEditor(const QByteArray &id, QWidget *parent = 0) const;
//    AbstractEditor *createEditor(const QByteArray &id, AbstractDocument *document) const;

    void addFactory(AbstractEditorFactory *factory);
    void removeFactory(AbstractEditorFactory *factory);

private slots:
    void onFactoryDestroyed(QObject *);

protected:
    EditorManagerPrivate *d_ptr;
};

} // namespace Parts

#endif // EDITORMANAGER_H
