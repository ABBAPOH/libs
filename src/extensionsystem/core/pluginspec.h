#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

#include "../extensionsystem_global.h"

#include <QtCore/QObject>

class QIODevice;

namespace ExtensionSystem {

class EXTENSIONSYSTEM_EXPORT Version
{
public:
    unsigned major;
    unsigned minor;
    unsigned build;
    unsigned revision;

    Version();
    explicit Version(const QString &version);

    bool isNull() const;

    QString toString() const;
    static Version fromString(const QString &version);

    bool operator==(const Version &) const;
};

class EXTENSIONSYSTEM_EXPORT PluginDependency
{
    QString m_name;
    Version m_version;

public:
    PluginDependency() {}
    PluginDependency(const QString &name, const QString &version);
    PluginDependency(const QString &name, const Version &version);

    inline QString name() const { return m_name; }
    inline Version version() const { return m_version; }

    bool operator==(const PluginDependency &other) const;
};

class IPlugin;

class PluginSpecPrivate;
class EXTENSIONSYSTEM_EXPORT PluginSpec : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PluginSpec)
    Q_DISABLE_COPY(PluginSpec)

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString vendor READ name)
    Q_PROPERTY(QString category READ name)
    Q_PROPERTY(QString copyright READ name)
    Q_PROPERTY(QString license READ name)
    Q_PROPERTY(QString description READ name)
    Q_PROPERTY(QString url READ name)

    Q_PROPERTY(bool loaded READ loaded WRITE setLoaded NOTIFY loadedChanged)
    Q_PROPERTY(bool loadOnStartup READ loadOnStartup WRITE setLoadOnStartup NOTIFY loadOnStartupChanged)
    Q_PROPERTY(bool canBeUnloaded READ canBeUnloaded)

public:
    enum Format { XmlFormat, BinaryFormat };

    explicit PluginSpec(QObject *parent = 0);
    ~PluginSpec();

    QString name() const;
    Version version() const;
    Version compatibilityVersion() const;
    QString vendor() const;
    QString category() const;
    QString copyright() const;
    QString license() const;
    QString description() const;
    QString url() const;

    QList<PluginDependency> dependencies() const;
    // plugins that are requied for this plugin
    QList<PluginSpec*> dependencySpecs() const;
    // plugins that require this plugin
    QList<PluginSpec*> dependentSpecs() const;

    QString libraryPath() const; // path to library

    bool loaded() const;

    bool loadOnStartup() const;
    void setLoadOnStartup(bool yes);

    bool canBeUnloaded() const;

    bool hasError() const;
    QString errorString() const;

    IPlugin *plugin() const;

    bool provides(const PluginDependency &dependency) const;

//    bool read(QIODevice *device);
//    bool write(QIODevice *device, Format format);

    bool read(const QString & path);
    bool write(const QString & path, Format format = XmlFormat);

public slots:
    void load();
    void unload();
    void setLoaded(bool yes);

signals:
    void loadedChanged(bool loaded);
    void loadOnStartupChanged(bool loads);
    void error(const QString &message);

protected:
    PluginSpecPrivate *d_ptr;

    friend class PluginManagerPrivate;
};

typedef QList<PluginSpec*> PluginSpecList;

} // namespace ExtensionSystem

#endif // PLUGINSPEC_H
