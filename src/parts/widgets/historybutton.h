#ifndef HISTORYBUTTON_H
#define HISTORYBUTTON_H

#include "../parts_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QToolButton>
#else
#include <QtGui/QToolButton>
#endif

namespace Parts {

class History;

class HistoryButtonPrivate;
class PARTS_EXPORT HistoryButton : public QToolButton
{
    Q_OBJECT
    Q_DISABLE_COPY(HistoryButton)

public:
    explicit HistoryButton(QWidget *parent = 0);
    ~HistoryButton();

    enum Direction { Back, Forward };

    Direction direction() const;
    void setDirection(Direction direction);

    History *history() const;
    void setHistory(History *history);

signals:

private slots:
    void onAboutToShow();
    void onClicked();
    void onTriggered();

private:
    HistoryButtonPrivate *d;
};

} // namespace Parts

#endif // HISTORYBUTTON_H
