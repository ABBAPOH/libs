#ifndef OPENSTRATEGY_H
#define OPENSTRATEGY_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtGui/QKeySequence>

namespace Parts {

class OpenStrategy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(OpenStrategy)
public:
    explicit OpenStrategy(const QByteArray &id, QObject *parent = 0);
    ~OpenStrategy();

    enum StrategyType { TypeNone, TypeOpenCurrentWindow, TypeOpenNewTab, TypeOpenNewWindow };

    QByteArray id() const;

    virtual bool canOpen(const QList<QUrl> &urls) = 0;
    // TODO: add Reason (aka "click", "doubleClick", "shortcut") ?
    virtual void open(const QList<QUrl> &urls) = 0;
    void open(const QUrl &url);
    virtual QString text() const = 0;
    virtual QString toolTip() const { return QString(); }
    virtual QKeySequence keySequence() const { return QKeySequence(); }
    virtual Qt::KeyboardModifiers modifiers() const { return Qt::NoModifier; }
    // TODO: remove ?
    virtual StrategyType type() const { return TypeNone; }

    static QList<OpenStrategy *> strategies();
    static OpenStrategy *defaultStrategy();
    static OpenStrategy *strategy(const QByteArray &id);
    static OpenStrategy *strategy(Qt::KeyboardModifiers modifiers);
    static OpenStrategy *strategy(StrategyType type);

private:
    QByteArray m_id;
};

} // namespace Parts

#endif // OPENSTRATEGY_H
