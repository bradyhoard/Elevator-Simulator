#include "floor.h"
#include <QDebug>

Floor::Floor(QTextBrowser *browser, int floor_number, QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_floor_number = floor_number;
}

void Floor::open_Door()
{
    m_browser->append("Door opening...");
}

void Floor::close_Door()
{
    m_browser->append("Door closing...");
}
