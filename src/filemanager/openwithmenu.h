#ifndef OPENWITHMENU_H
#define OPENWITHMENU_H

#include "filemanager_global.h"

#include <QtCore/QUrl>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#else
#include <QtGui/QMenu>
#endif

#include <IO/QDefaultProgram>

class FILEMANAGER_EXPORT OpenWithMenu : public QMenu
{
    Q_OBJECT
    Q_PROPERTY(QList<QUrl> urls READ urls WRITE setUrls NOTIFY urlsChanged)

public:
    explicit OpenWithMenu(QWidget *parent = 0);

    QList<QUrl> urls() const;
    void setUrls(const QList<QUrl> &urls);

    void setPaths(const QStringList &paths);

signals:
    void urlsChanged(const QList<QUrl> &urls);

private slots:
    void onTriggered(QAction *action);
    void selectProgram();

private:
    QList<QUrl> m_urls;
    QDefaultProgramList m_programs;
};

#endif // OPENWITHMENU_H
