#ifndef SHORTCUTEDIT_H
#define SHORTCUTEDIT_H

#include "widgets_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class ShortcutEditPrivate;
class WIDGETS_EXPORT ShortcutEdit : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ShortcutEdit)
    Q_DECLARE_PRIVATE(ShortcutEdit)

    Q_PROPERTY(QKeySequence keySequence READ keySequence WRITE setKeySequence USER true)
public:
    explicit ShortcutEdit(QWidget *parent = 0);
    ~ShortcutEdit();

    QKeySequence keySequence() const;
    void setKeySequence(const QKeySequence &key);

public slots:
    void clearKeySequence();

signals:
    void keySequenceFinished();
    void keySequenceChanged(const QKeySequence &key);

protected:
    bool event(QEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);

protected:
    ShortcutEditPrivate *d_ptr;
};

#endif // SHORTCUTEDIT_H
