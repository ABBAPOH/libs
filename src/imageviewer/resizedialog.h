#ifndef QIMAGEVIEWRESIZEDIALOG_H
#define QIMAGEVIEWRESIZEDIALOG_H

#include "imageviewer_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

namespace Ui {
class ResizeDialog;
}

namespace ImageViewer {

class IMAGEVIEWER_EXPORT ResizeDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QSize imageSize READ imageSize WRITE setImageSize NOTIFY imageSizeChanged)

public:
    explicit ResizeDialog(QWidget *parent = 0);
    ~ResizeDialog();

    QSize imageSize() const;
    void setImageSize(const QSize &size);

signals:
    void imageSizeChanged(const QSize &);

private slots:
    void fitIn(int);
    void widthChanged(int);
    void heightChanged(int);
    void saveProportionsClicked(bool);

private:
    qreal factor() const;

private:
    Ui::ResizeDialog *ui;

    QSize m_size;
    QSize m_currentSize;
    bool m_ignoreSignals;
};

} // namespace ImageViewer

#endif // QIMAGEVIEWRESIZEDIALOG_H
