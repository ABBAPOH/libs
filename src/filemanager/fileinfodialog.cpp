#include "fileinfodialog.h"

#include <QtCore/QDateTime>

#if QT_VERSION >= 0x050000
#include <QtCore/QMimeDatabase>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFileIconProvider>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#else
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QFileIconProvider>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <IO/QMimeDatabase>
#endif

#include <IO/QDriveInfo>
#include <IO/DirectoryDetails>

#include <Widgets/OutlineWidget>

#include <qmath.h>

#include "permissionswidget.h"

namespace FileManager {

class FileInfoDialogPrivate
{
public:
    explicit FileInfoDialogPrivate(FileInfoDialog *qq) : q(qq) {}

    void updateUi();
    void setupUi();
    void retranslateUi();

public:
    FileInfoDialog *q;

    QGridLayout *topLayout;
    OutlineWidget *widget;
    QVBoxLayout *layout;
    QLabel *iconLabel;
    QLabel *nameLabel;
    QWidget *widget_1;
    QWidget *widget_2;
    PermissionsWidget *permissionsWidget;
    QFormLayout *layout_1;
    QFormLayout *layout_2;
    QFormLayout *layout_3;
    QLabel *mimeTypeLabel;
    QLabel *mimeType;
    QLabel *sizeLabel;
    QLabel *size;
    QLabel *locationLabel;
    QLabel *location;
    QLabel *createdLabel;
    QLabel *created;
    QLabel *modifiedLabel;
    QLabel *modified;
    QLabel *acceccedLabel;
    QLabel *accecced;

    QLabel *driveLabel;
    QLabel *drive;
    QLabel *totalSizeLabel;
    QLabel *totalSize;
    QLabel *availableSizeLabel;
    QLabel *availableSize;
    QLabel *mountPointLabel;
    QLabel *mountPoint;
    QLabel *fileSystemLabel;
    QLabel *fileSystem;

    QLabel *userPermissionsLabel;
    QComboBox *userPermissionsComboBox;
    QLabel *groupPermissionsLabel;
    QComboBox *groupPermissionsComboBox;
    QLabel *otherPermissionsLabel;
    QComboBox *otherPermissionsComboBox;

    QFileInfo fileInfo;
    QDriveInfo driveInfo;
    DirectoryDetails *directoryDetails;
};

} // namespace FileManager

using namespace FileManager;

static QString sizeToString(qint64 size)
{
    static const char *const strings [] = { "b", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

    if (size <= 0)
        return FileInfoDialog::tr("0 b");

    double power = log((double)size)/log(1024.0);
    int intPower = (int)power;
    intPower = intPower >= 8 ? 8 - 1 : intPower;

    double normSize = size / pow(1024.0, intPower);
    return FileInfoDialog::tr("%1 %2").arg(normSize, 0, 'f', intPower > 0 ? 2 : 0).arg(strings[intPower]);
}

static QString pathToWrappedString(const QString &path)
{
    QString result = path;
    result.replace(QString("/"), QString("/") + QChar(0x200B));
    result = result.trimmed();
    return result;
}

void FileInfoDialogPrivate::updateUi()
{
    QIcon icon = QFileIconProvider().icon(fileInfo);
    q->setWindowIcon(icon);
    q->setWindowTitle(FileInfoDialog::tr("\"%1\" info").arg(fileInfo.fileName()));

    iconLabel->setPixmap(icon.pixmap(32));

    QMimeDatabase db;
    mimeType->setText(pathToWrappedString(db.mimeTypeForFile(fileInfo).name()));

    nameLabel->setText(fileInfo.fileName());

    if (fileInfo.isDir())
        size->setText(FileInfoDialog::tr("Calculating..."));
    else
        size->setText(sizeToString(fileInfo.size()));

    location->setText(pathToWrappedString(fileInfo.path()));
    created->setText(fileInfo.created().toString(Qt::SystemLocaleShortDate));
    modified->setText(fileInfo.lastModified().toString(Qt::SystemLocaleShortDate));
    accecced->setText(fileInfo.lastRead().toString(Qt::SystemLocaleShortDate));

    drive->setText(driveInfo.name());
    mountPoint->setText(pathToWrappedString(driveInfo.rootPath()));
    fileSystem->setText(driveInfo.fileSystemName());
    totalSize->setText(sizeToString(driveInfo.bytesTotal()));
    availableSize->setText(sizeToString(driveInfo.bytesAvailable()));

    permissionsWidget->setFileInfo(fileInfo);
}

void FileInfoDialogPrivate::setupUi()
{
    q->setWindowTitle(FileInfoDialog::tr("File info"));

    layout = new QVBoxLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    topLayout = new QGridLayout();
    topLayout->setContentsMargins(5, 5, 5, 5);
    topLayout->setSpacing(5);

    iconLabel = new QLabel(q);
    iconLabel->setObjectName(QLatin1String("iconLabel"));

    nameLabel = new QLabel(q);
    nameLabel->setObjectName(QLatin1String("nameLabel"));
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    topLayout->addWidget(iconLabel, 0, 0, 1, 1);
    topLayout->addWidget(nameLabel, 0, 1, 1, 1);

    layout->addLayout(topLayout);

    QFrame *line = new QFrame(q);
    line->setObjectName(QLatin1String("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);

    widget = new OutlineWidget(q);
    layout->addWidget(widget);

    widget_1 = new QWidget(widget);
    widget_1->setObjectName(QLatin1String("widget_1"));
    widget->addWidget(widget_1, FileInfoDialog::tr("General Info"));

    layout_1 = new QFormLayout(widget_1);
    layout_1->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    layout_1->setSizeConstraint(QLayout::SetMinimumSize);
    layout_1->setVerticalSpacing(0);

    mimeTypeLabel = new QLabel(widget_1);
    mimeTypeLabel->setObjectName(QLatin1String("mimeTypeLabel"));
    mimeTypeLabel->setAlignment(Qt::AlignTop);

    mimeType = new QLabel(widget_1);
    mimeType->setObjectName(QLatin1String("mimeType"));
    mimeType->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mimeType->setWordWrap(true);
    layout_1->addRow(mimeTypeLabel, mimeType);

    sizeLabel = new QLabel(widget_1);
    sizeLabel->setObjectName(QLatin1String("sizeLabel"));
    sizeLabel->setAlignment(Qt::AlignTop);

    size = new QLabel(widget_1);
    size->setObjectName(QLatin1String("size"));
    size->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_1->addRow(sizeLabel, size);

    locationLabel = new QLabel(widget_1);
    locationLabel->setObjectName(QLatin1String("locationLabel"));
    locationLabel->setAlignment(Qt::AlignTop);

    location = new QLabel(widget_1);
    location->setObjectName(QLatin1String("location"));
    location->setWordWrap(true);
    location->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_1->addRow(locationLabel, location);

    createdLabel = new QLabel(widget_1);
    createdLabel->setObjectName(QLatin1String("createdLabel"));
    createdLabel->setAlignment(Qt::AlignTop);

    created = new QLabel(widget_1);
    created->setObjectName(QLatin1String("created"));
    created->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_1->addRow(createdLabel, created);

    modifiedLabel = new QLabel(widget_1);
    modifiedLabel->setObjectName(QLatin1String("modifiedLabel"));
    modifiedLabel->setAlignment(Qt::AlignTop);

    modified = new QLabel(widget_1);
    modified->setObjectName(QLatin1String("modified"));
    modified->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_1->addRow(modifiedLabel, modified);

    acceccedLabel = new QLabel(widget_1);
    acceccedLabel->setObjectName(QLatin1String("acceccedLabel"));
    acceccedLabel->setAlignment(Qt::AlignTop);

    accecced = new QLabel(widget_1);
    accecced->setObjectName(QLatin1String("accecced"));
    accecced->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_1->addRow(acceccedLabel, accecced);

    widget_2 = new QWidget(widget);
    widget_2->setObjectName(QLatin1String("widget_2"));
    widget->addWidget(widget_2, FileInfoDialog::tr("Drive Info"));

    layout_2 = new QFormLayout(widget_2);
    layout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    layout_2->setSizeConstraint(QLayout::SetMinimumSize);
    layout_2->setVerticalSpacing(0);

    driveLabel = new QLabel(widget_2);
    driveLabel->setObjectName(QLatin1String("driveLabel"));
    driveLabel->setAlignment(Qt::AlignTop);

    drive = new QLabel(widget_2);
    drive->setObjectName(QLatin1String("drive"));
    drive->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_2->addRow(driveLabel, drive);

    mountPointLabel = new QLabel(widget_2);
    mountPointLabel->setObjectName(QLatin1String("mountPointLabel"));
    mountPointLabel->setAlignment(Qt::AlignTop);

    mountPoint = new QLabel(widget_2);
    mountPoint->setObjectName(QLatin1String("mountPoint"));
    mountPoint->setWordWrap(true);
    mountPoint->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_2->addRow(mountPointLabel, mountPoint);

    fileSystemLabel = new QLabel(widget_2);
    fileSystemLabel->setObjectName(QLatin1String("fileSystemLabel"));
    fileSystemLabel->setAlignment(Qt::AlignTop);

    fileSystem = new QLabel(widget_2);
    fileSystem->setObjectName(QLatin1String("fileSystem"));
    fileSystem->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_2->addRow(fileSystemLabel, fileSystem);

    totalSizeLabel = new QLabel(widget_2);
    totalSizeLabel->setObjectName(QLatin1String("totalSizeLabel"));
    totalSizeLabel->setAlignment(Qt::AlignTop);

    totalSize = new QLabel(widget_2);
    totalSize->setObjectName(QLatin1String("availableSize"));
    totalSize->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_2->addRow(totalSizeLabel, totalSize);

    availableSizeLabel = new QLabel(widget_2);
    availableSizeLabel->setObjectName(QLatin1String("availableSizeLabel"));
    availableSizeLabel->setAlignment(Qt::AlignTop);

    availableSize = new QLabel(widget_2);
    availableSize->setObjectName(QLatin1String("availableSize"));
    availableSize->setTextInteractionFlags(Qt::TextBrowserInteraction);
    layout_2->addRow(availableSizeLabel, availableSize);

    permissionsWidget = new PermissionsWidget;
    widget->addWidget(permissionsWidget, FileInfoDialog::tr("Permissions"));

    retranslateUi();
}

void FileInfoDialogPrivate::retranslateUi()
{
    mimeTypeLabel->setText(FileInfoDialog::tr("Mime type:"));
    sizeLabel->setText(FileInfoDialog::tr("Size:"));
    locationLabel->setText(FileInfoDialog::tr("Where:"));
    createdLabel->setText(FileInfoDialog::tr("Created:"));
    modifiedLabel->setText(FileInfoDialog::tr("Modified:"));
    acceccedLabel->setText(FileInfoDialog::tr("Accessed:"));

    driveLabel->setText(FileInfoDialog::tr("Drive:"));
    totalSizeLabel->setText(FileInfoDialog::tr("Total:"));
    availableSizeLabel->setText(FileInfoDialog::tr("Available:"));
    mountPointLabel->setText(FileInfoDialog::tr("Mounted:"));
    fileSystemLabel->setText(FileInfoDialog::tr("Format:"));

    widget->setText(0, FileInfoDialog::tr("General Info"));
    widget->setText(1, FileInfoDialog::tr("Drive Info"));
    widget->setText(2, FileInfoDialog::tr("Permissions"));
}

/*!
    \class FileManager::FileInfoDialog

    \brief FileInfoDialog is a widget for displaying file info.

    \image html fileinfodialog.png
*/

/*!
    Creates FileInfoDialog with the given \a parent.
*/
FileInfoDialog::FileInfoDialog(QWidget *parent) :
    QWidget(parent),
    d(new FileInfoDialogPrivate(this))
{
    d->setupUi();
    d->directoryDetails = 0;

    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::Window);

    setMinimumSize(300, 400);
}

/*!
    Destroys FileInfoDialog.
*/
FileInfoDialog::~FileInfoDialog()
{
    if (d->directoryDetails) {
        if (d->directoryDetails->isRunning())
            d->directoryDetails->stopProcessing();

        d->directoryDetails->wait();
    }

    delete d;
}

/*!
    \property FileInfoDialog::fileInfo
    This property hold QFileInfo currently displayed in the FileInfoDialog.
*/
QFileInfo FileInfoDialog::fileInfo() const
{
    return d->fileInfo;
}

void FileInfoDialog::setFileInfo(const QFileInfo &info)
{
    if (d->fileInfo == info)
        return;

    d->fileInfo = info;
    d->driveInfo = QDriveInfo(d->fileInfo.absoluteFilePath());

    if (d->directoryDetails) {
        delete d->directoryDetails;
        d->directoryDetails = 0;
    }

    if (info.isDir()) {
        d->directoryDetails = new DirectoryDetails(info.absoluteFilePath(), this);
        connect(d->directoryDetails, SIGNAL(finished()), this, SLOT(updateSize()));
        d->directoryDetails->start();
    }

    d->updateUi();

    emit fileInfoChanged(d->fileInfo);
}

/*!
    \fn void FileInfoDialog::fileInfoChanged(const QFileInfo &info)

    This signal is emitted when FileInfoDialog::fileInfo property is changed.
*/

void FileInfoDialog::onActivatedUser(int i)
{
    d->fileInfo.refresh();
    QFile::Permissions flags = (i == 1) ? (QFile::WriteOwner | QFile::ReadOwner) : (QFile::ReadOwner);
    QFile::setPermissions(d->fileInfo.filePath(), (d->fileInfo.permissions() & 0x1FF) | flags );
}

void FileInfoDialog::onActivatedGroup(int i)
{
    d->fileInfo.refresh();
    QFile::Permissions flags = (i == 1) ? (QFile::WriteGroup | QFile::ReadGroup) : (QFile::ReadGroup);
    QFile::setPermissions(d->fileInfo.filePath(), (d->fileInfo.permissions() & 0xFC7) | flags );
}

void FileInfoDialog::onActivatedOther(int i)
{
    d->fileInfo.refresh();
    QFile::Permissions flags = (i == 1) ? (QFile::WriteOther | QFile::ReadOther) : (QFile::ReadOther);
    QFile::setPermissions(d->fileInfo.filePath(), (d->fileInfo.permissions() & 0xFF8) | flags );
}

void FileInfoDialog::updateSize()
{
    int objCount = d->directoryDetails->totalFiles() + d->directoryDetails->totalFolders();
    qint64 totalSize = d->directoryDetails->totalSize();
    QString sizeLabel = tr("%2 for %3 objects").
            arg(sizeToString(totalSize)).
            arg(objCount);
    d->size->setText(sizeLabel);
}
