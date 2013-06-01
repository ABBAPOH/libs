#ifndef EDITORVIEWHISTORY_H
#define EDITORVIEWHISTORY_H

#include <Parts/IHistory>

namespace Parts {

class AbstractEditor;
class EditorView;

class EditorViewHistoryPrivate;
class EditorViewHistory : public IHistory
{
    Q_OBJECT
    Q_DISABLE_COPY(EditorViewHistory)

public:
    explicit EditorViewHistory(QObject *parent = 0);
    ~EditorViewHistory();

    void setContainer(EditorView *container);

    void open(const QUrl &url, Parts::AbstractEditor *oldEditor);

    int count() const;

    int currentItemIndex() const;
    void setCurrentItemIndex(int index);

    HistoryItem itemAt(int index) const;

    QByteArray store() const;
    void restore(const QByteArray &);

public slots:
    void clear();

private slots:
    void localHistoryIndexChanged(int index);
    void onUrlChanged(const QUrl &url);

private:
    EditorViewHistoryPrivate *d;
};

} // namespace Parts

#endif // EDITORVIEWHISTORY_H
