#include "floor.h"
#include <QDebug>

Floor::Floor(QTextBrowser *browser, QObject *parent) : QObject(parent)
{
    m_browser = browser;
}

void Floor::open_Door()
{
    m_browser->append("Door opening...");
}

void Floor::close_Door()
{
    m_browser->append("Door closing...");
}
