#include "fullpluginview.h"
#include "ui_fullpluginview.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDataWidgetMapper>
#else
#include <QtGui/QDataWidgetMapper>
#endif

#include <ExtensionSystem/PluginViewModel>

using namespace ExtensionSystem;

/*!
    \class ExtensionSystem::FullPluginView
    \brief FullPluginView provides detalied view of a single plugin.

    \image html fullpluginview.png
*/

/*!
    Creates FullPluginView with given \a parent.
*/
FullPluginView::FullPluginView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FullPluginView),
    m_mapper(new QDataWidgetMapper(this)),
    m_model(0)
{
    ui->setupUi(this);
}

/*!
    Destroys FullPluginView.
*/
FullPluginView::~FullPluginView()
{
    delete ui;
}

/*!
    Returns current model.
*/
PluginViewModel * FullPluginView::model() const
{
    return m_model;
}

/*!
    Sets current model.
*/
void FullPluginView::setModel(PluginViewModel * model)
{
    if (m_model == model)
        return;

    m_model = model;
    m_mapper->clearMapping();
    m_mapper->setModel(m_model);
    if (m_model != 0) {
        m_mapper->setRootIndex(m_model->index(0, 0));
        m_mapper->addMapping(ui->label_Name, PluginViewModel::Name, "text");
        m_mapper->addMapping(ui->label_Version, PluginViewModel::Version, "text");
        m_mapper->addMapping(ui->label_CompatibilityVersion, PluginViewModel::CompatibilityVersion, "text");
        m_mapper->addMapping(ui->label_Vendor, PluginViewModel::Vendor, "text");
        m_mapper->addMapping(ui->label_Url, PluginViewModel::Url, "text");
        m_mapper->addMapping(ui->lineEdit_Location, PluginViewModel::LibraryPath, "text");
        m_mapper->addMapping(ui->textBrowser_Description, PluginViewModel::Description, "plainText");
        m_mapper->addMapping(ui->label_Copyright, PluginViewModel::Copyright, "text");
        m_mapper->addMapping(ui->textBrowser_License, PluginViewModel::License, "plainText");
        m_mapper->addMapping(ui->textBrowser_Dependencies, PluginViewModel::Dependencies, "plainText");
    }
}

/*!
    Sets current model \a index corresponding to a specified plugin.
*/
void FullPluginView::setIndex(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
}
