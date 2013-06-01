#ifndef COMMANDSSETTINGS_H
#define COMMANDSSETTINGS_H

#include "../parts_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

namespace Ui {
class CommandsSettingsWidget;
}

class ShortcutEdit;
class QItemSelection;
class QSortFilterProxyModel;

namespace Parts {

class CommandsModel;

class PARTS_EXPORT CommandsSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommandsSettingsWidget(QWidget *parent = 0);
    ~CommandsSettingsWidget();

private slots:
    void onTextChanged(const QString &text);
    void onSelectionChanged(const QItemSelection &selection);
    void reset();
    void resetAll();

    void exportShortcuts();
    void importShortcuts();

private:
    Ui::CommandsSettingsWidget *ui;
    CommandsModel *m_model;
    QSortFilterProxyModel *m_proxy;
};

} // namespace Parts

#endif // COMMANDSSETTINGS_H
