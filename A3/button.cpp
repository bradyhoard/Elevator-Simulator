#include "button.h"
#include <QDebug>

Button::Button(QTextBrowser *browser, QObject *parent) : QObject(parent)
{
    m_browser = browser;
}

void Button::illuminate(const int index , const QString location , const int ifcab_floor)
{
    if (location == "Floor"){
        m_browser->append("Floor (" + QString::number(index) + ") - button illuminated!");
    }
    else{
         m_browser->append("Cab (" + QString::number(index + 1) + ") to floor (" + QString::number(ifcab_floor) + ") - button illuminated!");
    }

}

void Button::deilluminate(const int index , const QString location, const int ifcab_floor)
{
    if (location == "Floor"){
        m_browser->append("Floor (" + QString::number(index) + ") - button de-illuminated!");
    }
    else{
         m_browser->append("Cab (" + QString::number(index + 1) + ") to floor (" + QString::number(ifcab_floor) + ") - button de-illuminated!");
    }
}

