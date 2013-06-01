#ifndef ERRORSDIALOG_H
#define ERRORSDIALOG_H

#include "../extensionsystem_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

class QLabel;
class QListView;
class QStringListModel;
class QDialogButtonBox;

namespace ExtensionSystem {

class EXTENSIONSYSTEM_EXPORT ErrorsDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(ErrorsDialog)

public:
    explicit ErrorsDialog(QWidget *parent = 0);

    QString message() const;
    void setMessage(const QString &message);

    void setErrors(const QStringList &errors);

private:
    QLabel *m_label;
    QListView *m_view;
    QStringListModel *m_model;
    QDialogButtonBox *m_buttonBox;
};

} // namespace ExtensionSystem

#endif // ERRORSDIALOG_H
