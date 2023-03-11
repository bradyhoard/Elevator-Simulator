#include "floor.h"

Floor::Floor(QTextBrowser *browser, int floor_number, QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_floor_number = floor_number;
}

void Floor::open_Door()
{
    m_browser->append("Floor" + QString::number(m_floor_number) + ":  doors opening...");
}

void Floor::close_Door()
{
    m_browser->append("Floor" + QString::number(m_floor_number) + ":  doors clsoing...");
}
