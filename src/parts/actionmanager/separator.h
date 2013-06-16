#ifndef SEPARATOR_H
#define SEPARATOR_H

#include "abstractcommand.h"

namespace Parts {

class SeparatorPrivate;
class PARTS_EXPORT Separator : public AbstractCommand
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Separator)
    Q_DISABLE_COPY(Separator)

    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

public:
    explicit Separator(QObject *parent = 0);

    bool isVisible() const;
    void setVisible(bool visible);

protected:
    QAction *createAction(QObject *parent) const;
};

} // namespace Parts

#endif // SEPARATOR_H
