#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "../parts_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif
#include <Parts/AbstractEditor>

namespace Parts {

class IHistory;

class EditorViewPrivate;
class PARTS_EXPORT EditorView : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(EditorView)

public:
    explicit EditorView(QWidget *parent = 0);
    ~EditorView();

    void setSourceEditor(AbstractEditor *editor);

    AbstractEditor *currentEditor() const;

    QUrl url() const;

    IHistory *history() const;

    bool restoreState(const QByteArray &state);
    QByteArray saveState() const;

    AbstractDocument *document() const;
    AbstractEditor *editor() const;

public slots:
    void open(const QUrl &url = QUrl());
    void openEditor(const QUrl &url, const QByteArray &editor);
    void openEditor(const QByteArray &editorId);

signals:
    void editorChanged();

private slots:
    void onUrlChanged(const QUrl &url);

private:
    EditorViewPrivate *d;

    friend class EditorViewPrivate;
};

} // namespace Parts

#endif // EDITORVIEW_H
