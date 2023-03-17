#include "floor.h"

//floor constructor
Floor::Floor(QTextBrowser *browser, int floor_number, QObject *parent) : QObject(parent)
{
    m_browser = browser;
    m_floor_number = floor_number;
}

/*
 * Name :
 *      open_Door
 *
 * Params :
 *      none
 *
 * returns :
 *      void
 *
 */
void Floor::open_Door()
{
    m_browser->append("Floor(" + QString::number(m_floor_number) + ") :  doors opened");
}

/*
 * Name :
 *      close_Door
 *
 * Params :
 *      none
 *
 * returns :
 *      void
 *
 */
void Floor::close_Door()
{
    m_browser->append("Floor(" + QString::number(m_floor_number) + ") :  doors closed");
}
