#include "ecs.h"
#include <QDebug>

ECS::ECS(QTextBrowser *browser, QObject *parent) : QObject(parent)
{
    m_browser = browser;
}

void ECS::emergency(const QString& em)
{
    m_browser->append(em);
}
