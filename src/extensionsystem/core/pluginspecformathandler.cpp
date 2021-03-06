#include "pluginspecformathandler_p.h"

#include <QtCore/QDataStream>
#include <QtCore/QIODevice>
#include <QtCore/QXmlStreamReader>

#include <QtCore/QDebug>

using namespace ExtensionSystem;

/*!
    \internal
*/
bool PluginSpecBinaryHandler::canRead(QIODevice *device)
{
    return device->peek(4) == QByteArray("SPEC");
}

bool PluginSpecBinaryHandler::read(QIODevice *device, PluginSpecPrivate *d)
{
    clearError();

    QDataStream s(device);
    s.setByteOrder(QDataStream::BigEndian);
    s >> *d;

    if (s.status() != QDataStream::Ok) {
        setErrorString(QObject::tr("Cannot read binary file format"));
        return false;
    }

    return true;
}

bool PluginSpecBinaryHandler::write(QIODevice *device, PluginSpecPrivate *d)
{
    clearError();

    QDataStream s(device);
    s.setByteOrder(QDataStream::BigEndian);
    s << *d;

    return true;
}

namespace ExtensionSystem {

QDataStream & operator<<(QDataStream &s, const Version &version)
{
    s << (quint32)version.major;
    s << (quint32)version.minor;
    s << (quint32)version.build;
    s << (quint32)version.revision;
    return s;
}

QDataStream & operator>>(QDataStream &s, Version &version)
{
    quint32 tmp;
    s >> tmp; version.major = tmp;
    s >> tmp; version.minor = tmp;
    s >> tmp; version.build = tmp;
    s >> tmp; version.revision = tmp;
    return s;
}

QDataStream & operator>>(QDataStream &s, PluginDependency &dependency)
{
    QString name;
    Version version;
    s >> name;
    s >> version;
    dependency = PluginDependency(name, version);
    return s;
}

QDataStream & operator<<(QDataStream &s, const PluginDependency &dependency)
{
    s << dependency.name();
    s << dependency.version();
    return s;
}

QDataStream & operator>>(QDataStream &s, PluginSpecPrivate &pluginSpec)
{
    PluginSpecPrivate * d = &pluginSpec;
    s.device()->read(4);
    quint32 version;
    s >> version;

    s >> d->name;
    s >> d->version;
    s >> d->compatibilityVersion;
    s >> d->vendor;
    s >> d->category;
    s >> d->copyright;
    s >> d->license;
    s >> d->description;
    s >> d->url;
    s >> d->dependencies;
    return s;
}

QDataStream & operator<<(QDataStream &s, const PluginSpecPrivate &pluginSpec)
{
    const PluginSpecPrivate * d = &pluginSpec;
    s << (uchar)'S' << (uchar)'P' << (uchar)'E' << (uchar)'C';
    quint32 version = 1;
    s << version;
    s << d->name;
    s << d->version;
    s << d->compatibilityVersion;
    s << d->vendor;
    s << d->category;
    s << d->copyright;
    s << d->license;
    s << d->description;
    s << d->url;
    s << d->dependencies;
    return s;
}

} // namespace ExtensionSystem

bool stringToBool(bool *ok, const QString &s);

static void readXmlPluginSpec(PluginSpecPrivate *d, QXmlStreamReader &reader);
static void readXmlPluginSpecDependencies(PluginSpecPrivate *d, QXmlStreamReader &reader);

/*!
    \internal
*/
bool PluginSpecXmlHandler::canRead(QIODevice *device)
{
    // todo: add identifier
    return device->peek(10).contains("<plugin");
}

bool PluginSpecXmlHandler::read(QIODevice *device, PluginSpecPrivate *d)
{
    clearError();

    QXmlStreamReader reader(device);

    readXmlPluginSpec(d, reader);

    if (reader.hasError()) {
        setErrorString(QObject::tr("Error parsing xml stream: %1, at line %2, column %3")
                       .arg(reader.errorString())
                       .arg(reader.lineNumber())
                       .arg(reader.columnNumber()));
        return false;
    }
    return true;
}

bool PluginSpecXmlHandler::write(QIODevice *device, PluginSpecPrivate *d)
{
    clearError();

    QXmlStreamWriter writer(device);
    writer.setAutoFormatting(true);
    writer.writeStartElement(QLatin1String("plugin"));
    writer.writeAttribute(QLatin1String("name"), d->name);
    writer.writeAttribute(QLatin1String("version"), d->version.toString());
    writer.writeAttribute(QLatin1String("compatVersion"), d->compatibilityVersion.toString());

    writer.writeTextElement(QLatin1String("vendor"), d->vendor);
    writer.writeTextElement(QLatin1String("copyright"), d->copyright);
    writer.writeTextElement(QLatin1String("license"), d->license);
    writer.writeTextElement(QLatin1String("category"), d->category);
    writer.writeTextElement(QLatin1String("description"), d->description);
    writer.writeTextElement(QLatin1String("url"), d->url);

    if (!d->dependencies.isEmpty()) {
        writer.writeStartElement(QLatin1String("dependencyList"));

        foreach (const PluginDependency &dependency, d->dependencies) {
            writer.writeStartElement(QLatin1String("dependency"));
            writer.writeAttribute(QLatin1String("name"), dependency.name());
            writer.writeAttribute(QLatin1String("version"), dependency.version().toString());
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }

    writer.writeEndElement();

    return true;
}

static void readXmlPluginSpecDependencies(PluginSpecPrivate *d, QXmlStreamReader &reader)
{
    QString name;
    bool readingElement = false;
    while (!reader.atEnd()) {
        reader.readNext();
        name = reader.name().toString();
        switch (reader.tokenType()) {

        case QXmlStreamReader::StartElement:

            if (readingElement)
                reader.raiseError(QObject::tr("Unexpected token '%1'").arg(name));
            readingElement = true;

            if (name == QLatin1String("dependency")) {
                QString depName = reader.attributes().value(QLatin1String("name")).toString();
                QString depVersion = reader.attributes().value(QLatin1String("version")).toString();
                d->dependencies.append(PluginDependency(depName, depVersion));
            } else {
                reader.raiseError(QObject::tr("Unknown element '%1'").arg(name));
            }

            break;

        case QXmlStreamReader::EndElement:

            readingElement = false;

            if (name == QLatin1String("dependency"))
                break;

            if (name == QLatin1String("dependencyList"))
                return;

            reader.raiseError(QObject::tr("Expected </dependencyList>"));
            break;

        case QXmlStreamReader::Comment:
            break;

        case QXmlStreamReader::Characters:

            if (!reader.text().toString().trimmed().isEmpty())
                reader.raiseError(QObject::tr("Unexpected character sequence"));
            break;

        default:
            reader.raiseError(QObject::tr("Unexpected token"));
            break;
        }
    }
}

static void readXmlPluginSpecAttributes(PluginSpecPrivate *d, QXmlStreamReader &reader)
{
    QString name;
    while (!reader.atEnd()) {
        reader.readNext();
        name = reader.name().toString();
        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:

            if (name == QLatin1String("vendor")) {
                d->vendor = reader.readElementText();
            } else if (name == QLatin1String("copyright")) {
                d->copyright = reader.readElementText();
            } else if (name == QLatin1String("license")) {
                d->license = reader.readElementText();
            } else if (name == QLatin1String("category")) {
                d->category = reader.readElementText();
            } else if (name == QLatin1String("description")) {
                d->description = reader.readElementText();
            } else if (name == QLatin1String("url")) {
                d->url = reader.readElementText();
            } else if (name == QLatin1String("dependencyList")) {
                readXmlPluginSpecDependencies(d, reader);
            } else {
                reader.raiseError(QObject::tr("Unknown element %1").arg(name));
            }

            break;
        case QXmlStreamReader::EndElement:

            if (name == QLatin1String("plugin"))
                return;

            reader.raiseError(QObject::tr("Expected </plugin>"));
            break;

        case QXmlStreamReader::Comment:
            break;

        case QXmlStreamReader::Characters:

            if (!reader.text().toString().trimmed().isEmpty())
                reader.raiseError(QObject::tr("Unexpected character sequence"));
            break;

        default:
            reader.raiseError(QObject::tr("Unexpected token"));
            break;
        }
    }
}

static void readXmlPluginSpec(PluginSpecPrivate *d, QXmlStreamReader &reader)
{
    if (reader.atEnd()) {
        reader.raiseError("Empty xml file");
        return;
    }

    QString name;
    while (!reader.atEnd()) {
        reader.readNext();
        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:

            name = reader.name().toString();
            if (name == QLatin1String("plugin")) {

                d->name = reader.attributes().value(QLatin1String("name")).toString();
                d->version = Version(reader.attributes().value(QLatin1String("version")).toString());
                d->compatibilityVersion = Version(reader.attributes().value(QLatin1String("compatVersion")).toString());
                readXmlPluginSpecAttributes(d, reader);

            } else {
                reader.raiseError(QObject::tr("Unknown element %1").arg(name));
            }

            break;

        case QXmlStreamReader::StartDocument:
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::Characters:
            break;

        case QXmlStreamReader::EndElement:

            reader.raiseError(QObject::tr("Expected end of document"));
            break;

        case QXmlStreamReader::EndDocument:
            return;

        default:
            qWarning() << "Unexpected token" << reader.tokenString();
            reader.raiseError(QObject::tr("Unexpected token"));
            break;
        }
    }
}
