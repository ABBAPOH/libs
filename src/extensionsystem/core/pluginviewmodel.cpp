#include "pluginviewmodel.h"
#include "pluginviewmodel_p.h"

using namespace ExtensionSystem;

PluginViewModelPrivate::PluginViewModelPrivate()
{
    manager = PluginManager::instance();
    rootNode = new Node;
}

PluginViewModelPrivate::~PluginViewModelPrivate()
{
    delete rootNode;
}

Node *PluginViewModelPrivate::node(const QString &category)
{
    if (nodesForCategories.contains(category))
        return nodesForCategories.value(category);
    Node *result = new Node(rootNode);
    result->categoryName = category;
    result->isCategory = true;
    nodesForCategories.insert(category, result);
    return result;
}

Node * PluginViewModelPrivate::node(PluginSpec *spec)
{
    if (nodesForSpecs.contains(spec))
        return nodesForSpecs.value(spec);
    Node *parent = node(spec->category());
    Node *result = new Node(parent);
    result->spec = spec;
    nodesForSpecs.insert(spec, result);
    return result;
}

/*!
    \class ExtensionSystem::PluginViewModel

    \brief PluginViewModel class allows to display list of application plugins
    in item views.

    PluginViewModel groups plugins by PluginSpec::category in a tree structure.
    Each row represents a single plugin, each column provides data about
    PluginSpec's property.
*/

/*!
    Constructs PluginViewModel with the given \a parent.
*/
PluginViewModel::PluginViewModel(QObject *parent) :
    QAbstractItemModel(parent),
    d_ptr(new PluginViewModelPrivate)
{
    Q_D(PluginViewModel);

    connect(d->manager, SIGNAL(pluginsChanged()), this, SLOT(updateModel()));
    foreach (PluginSpec *spec, d->manager->plugins()) {
        d->node(spec);
    }
}

/*!
    Destroys PluginViewModel.
*/
PluginViewModel::~PluginViewModel()
{
    delete d_ptr;
}

/*!
    \reimp
*/
int PluginViewModel::columnCount(const QModelIndex &/*parent*/) const
{
    return ColumnCount;
}

/*!
    \reimp
*/
QVariant PluginViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::CheckStateRole) {
        Node *node = static_cast<Node*>(index.internalPointer());
        if (!node->isCategory) {
            if (index.column() == LoadsOnStartup) {
                return node->spec->loadOnStartup() ? 2 : 0;
            }
            if (index.column() == Enabled) {
                return node->spec->loaded() ? 2 : 0;
            }
        }
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        Node *node = static_cast<Node*>(index.internalPointer());
        if (node->isCategory) {
            if (index.column() == 0) {
                return node->categoryName;
            }
        } else {
            switch (index.column()) {
            case Name: return node->spec->name();
//            case LoadsOnStartup: return node->spec->loadOnStartup();
//            case Enabled: return node->spec->loaded();
            case Version: return node->spec->version().toString();
            case CompatibilityVersion: return node->spec->compatibilityVersion().toString();
            case Vendor: return node->spec->vendor();
            case Url: return node->spec->url();
            case LibraryPath: return node->spec->libraryPath();
            case Description: return node->spec->description();
            case Copyright: return node->spec->copyright();
            case License: return node->spec->license();
            case Dependencies: QString result;
                foreach (PluginDependency dep, node->spec->dependencies()) {
                    result += dep.name() + ' ' + '(' + dep.version().toString() + ") \n";
                }
                return result.mid(0, result.length()-2);
            }
        }
    }
    return QVariant();
}

/*!
    \reimp
*/
Qt::ItemFlags PluginViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == LoadsOnStartup || index.column() == Enabled) {
        flags |= /*Qt::ItemIsEditable |*/ Qt::ItemIsUserCheckable;
        Node *node = static_cast<Node*>(index.internalPointer());
        if (!node->isCategory && !node->spec->canBeUnloaded()) {
            flags &= ~Qt::ItemIsEnabled;
        }
    }
    return flags;
}

/*!
    \reimp
*/
QVariant PluginViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case Name: return tr("Name");
        case LoadsOnStartup: return tr("Loads on Startup");
        case Enabled: return tr("Enabled");
        case Version: return tr("Version");
        case CompatibilityVersion: return tr("Compatibility Version");
        case Vendor: return tr("Vendor");
        case Url: return tr("Url");
        case LibraryPath: return tr("Library Path");
        case Description: return tr("Description");
        case Copyright: return tr("Copyright");
        case License: return tr("License");
        case Dependencies: return tr("Dependencies");
        }
    }

    return QVariant();
}

/*!
    \reimp
*/
QModelIndex PluginViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Node *parentNode;

    if (!parent.isValid())
        parentNode = d_func()->rootNode;
    else
        parentNode = static_cast<Node *>(parent.internalPointer());

    Node *childNode = parentNode->children.at(row);
    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}

/*!
    \reimp
*/
QModelIndex PluginViewModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Node *childNode = static_cast<Node *>(index.internalPointer());
    Node *parentNode = childNode->parent;
    if (parentNode == d_func()->rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

/*!
    \reimp
*/
int PluginViewModel::rowCount(const QModelIndex &parent) const
{
    Node *parentNode;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentNode = d_func()->rootNode;
    else
        parentNode = static_cast<Node *>(parent.internalPointer());

    return parentNode->childCount();
}

/*!
    \reimp
*/
bool PluginViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return true;

    if (role == Qt::CheckStateRole) {
        if (index.column() == LoadsOnStartup) {
            Node *node = static_cast<Node *>(index.internalPointer());
            // in fact we receive 0, 1 or 2 but 0 is false and 2 is true so everything ok
            node->spec->setLoadOnStartup(value.toBool());
            return true;
        }
        if (index.column() == Enabled) {
            Node *node = static_cast<Node *>(index.internalPointer());
            node->spec->setLoaded(value.toBool());
            return node->spec->loaded() == value.toBool();
        }
    }
    return false;
}

/*!
    \internal
*/
void PluginViewModel::updateModel()
{
    Q_D(PluginViewModel);

    foreach (PluginSpec *spec, d->manager->plugins()) {
        if (!d->nodesForCategories.contains(spec->category())) {
            const QString &category = spec->category();
            int rows = rowCount();
            beginInsertRows(QModelIndex(), rows, rows);
            d->node(category);
            endInsertRows();
        }
        if (!d->nodesForSpecs.contains(spec)) {
            QModelIndex categoryIndex = index(d->node(spec->category())->row(), 0);
            int rows = rowCount(categoryIndex);
            beginInsertRows(categoryIndex, rows, rows);
            d->node(spec);
            endInsertRows();
        }
    }
}
