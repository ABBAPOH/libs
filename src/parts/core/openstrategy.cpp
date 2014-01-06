#include "openstrategy.h"
#include "openstrategy_p.h"

using namespace Parts;

Q_GLOBAL_STATIC(OpenStrategyPrivate, static_instance)
OpenStrategyPrivate * OpenStrategyPrivate::instance()
{
    return static_instance();
}

/*!
    \internal
*/
void OpenStrategyPrivate::addStrategy(OpenStrategy *strategy)
{
    QList<OpenStrategy *> &strategies = instance()->strategies;
    if (strategies.contains(strategy)) {
        return;
    }
    strategies.append(strategy);
}

/*!
    \internal
*/
void OpenStrategyPrivate::removeStrategy(OpenStrategy *strategy)
{
    instance()->strategies.removeOne(strategy);
}

/*!
    \class Parts::OpenStrategy

    OpenStrategy is a base interface for strategies that implement opening
    files in applications main window.

    Strategies should be registered in an application via addStrategy() method;
    to unregister strategy, use removeStrategy() call. When deleted, strategies
    are removed automatically.

    For example, you can register "open new tab" strategy if your main window
    supports tabs; or you can register "open new window" strategy if your
    application supports multiple windows.
*/

/*!
    Creates OpenStrategy with the given \a id and \a parent.
*/
OpenStrategy::OpenStrategy(const QByteArray &id, QObject *parent) :
    QObject(parent),
    m_id(id)
{
    OpenStrategyPrivate::addStrategy(this);
}

/*!
    Destroys strategy.
*/
OpenStrategy::~OpenStrategy()
{
    OpenStrategyPrivate::removeStrategy(this);
}

/*!
    \property Parts::OpenStrategy::id

    Holds strategy's id. Id is used as QAction's object name to connect them
    to menus via ActionManager.
*/

QByteArray OpenStrategy::id() const
{
    return m_id;
}

/*!
    \fn bool Parts::OpenStrategy::canOpen(const QList<QUrl> &urls)

    Reimplement to return whether strategy can open given list of \a urls.
*/

/*!
    \fn void Parts::OpenStrategy::open(const QList<QUrl> &urls)

    Reimplement to perform opening of the given list of \a urls.
*/

/*!
    Opens single url.
*/
void OpenStrategy::open(const QUrl &url)
{
    open(QList<QUrl>() << url);
}

/*!
    \fn QString Parts::OpenStrategy::text() const

    Reimplement to return text that would be displayed as a QAction's text.
*/

/*!
    \fn QString Parts::OpenStrategy::toolTip() const

    Reimplement to return text that would be displayed as a QAction's tool tip.
*/

/*!
    \fn QString Parts::OpenStrategy::keySequence() const

    Reimplement to return key sequence that would be used as a QAction's
    shortcut.
*/

/*!
    \fn Qt::KeyboardModifiers Parts::OpenStrategy::modifiers() const

    Reimplement to return keyboard modifiers. These modifiers are usually used
    when double clicking on an item in an itemview to determine what strategy
    should be used.
*/

/*!
    Returns list of available strategies in an application.
*/
QList<OpenStrategy *> OpenStrategy::strategies()
{
    return OpenStrategyPrivate::instance()->strategies;
}

/*!
    Returns first strategy in list os availeble strategies or 0 if there
    is no strategies.
*/
OpenStrategy *OpenStrategy::defaultStrategy()
{
    QList<OpenStrategy *> strategies = OpenStrategy::strategies();
    if (strategies.isEmpty())
        return 0;
    return strategies.first();
}

/*!
    Returns strategy with identifier equal to \a id.

    If there are several strategies with the same id, first created
    strategy is returned.
*/
OpenStrategy *OpenStrategy::strategy(const QByteArray &id)
{
    foreach (OpenStrategy *strategy, strategies()) {
        if (strategy->id() == id)
            return strategy;
    }
    return 0;
}

/*!
    Returns strategy with modifiers equal to \a modifiers.

    If there are several strategies with the same modifiers, first created
    strategy is returned.
*/
OpenStrategy *OpenStrategy::strategy(Qt::KeyboardModifiers modifiers)
{
    foreach (OpenStrategy *strategy, strategies()) {
        if (strategy->modifiers() == modifiers)
            return strategy;
    }
    return 0;
}
