#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include "../parts_global.h"

#include <QtCore/QPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

namespace Parts {

class AbstractEditor;
class ToolModel;

class PARTS_EXPORT ToolWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ToolWidget)

public:
    explicit ToolWidget(ToolModel &model, QWidget *parent = 0);

    ToolModel *model() const;

    AbstractEditor *editor() const;
    virtual void setEditor(AbstractEditor *editor);

signals:
    void editorChanged();

protected slots:
    virtual void onDocumentChanged();

protected:
    ToolModel *m_model;
    QPointer<AbstractEditor> m_editor;
};

} // namespace Parts

#endif // TOOLWIDGET_H
