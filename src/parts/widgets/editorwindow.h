#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../parts_global.h"

#include <QtCore/QUrl>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif

#include <Parts/AbstractDocument>

class QToolButton;
class QUrl;

namespace Parts {
class AbstractEditor;

class EditorWindowPrivate;
class PARTS_EXPORT EditorWindow : public QMainWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(EditorWindow)
    Q_DISABLE_COPY(EditorWindow)

    Q_PROPERTY(bool menuVisible READ menuVisible WRITE setMenuVisible NOTIFY menuVisibleChanged)

public:
    enum Action { NoAction = -1,
                  OpenFile,
                  Close,
                  Save,
                  SaveAs,
                  Reload,
                  Stop,

                  ShowMenu,

                  ActionCount
                };

    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

    QAction *action(Action action) const;

    AbstractDocument *document() const;
    AbstractEditor *editor() const;
    virtual void setEditor(AbstractEditor *editor);

    bool menuVisible() const;

    QUrl url() const;

#ifndef Q_OS_MAC
    QToolButton *menuBarButton() const;
#endif

    static EditorWindow *currentWindow();
    static QList<EditorWindow*> windows();
    static EditorWindow *createWindow();

    virtual bool restoreState(const QByteArray &state);
    virtual QByteArray saveState() const;

public slots:
    virtual void open(const QUrl &url);

    virtual void close();

    static void openNewWindow(const QUrl &url);
//    static void openNewWindow(const QList<QUrl> &urls);

    void save();
    void saveAs();

    void reload();
    void stop();

    void setMenuVisible(bool visible);

signals:
    void menuVisibleChanged(bool visible);

protected slots:
    virtual void onUrlChanged(const QUrl &url);

    virtual void onWindowIconChanged(const QIcon &icon);
    virtual void onTitleChanged(const QString &title);

    virtual void onProgressChanged(int progress);
    virtual void onStateChanged(AbstractDocument::State state);

    virtual void onModificationChanged(bool modified);
    virtual void onWritableChanged(bool writable);

protected:
    EditorWindowPrivate *d_ptr;
};

} // namespace Parts

#endif // MAINWINDOW_H
