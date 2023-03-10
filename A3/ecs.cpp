#include "ecs.h"
#include <QDebug>

ECS::ECS(QTextBrowser *browser, QList<Elevator*> *elevators, QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_elevators = *elevators;

}

void ECS::emergency(const QString& em)
{
    m_browser->append(em);
}

