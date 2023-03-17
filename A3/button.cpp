#include "button.h"
#include <QDebug>

//button constructor class
Button::Button(QTextBrowser *browser, QObject *parent) : QObject(parent)
{
    m_browser = browser;
}

/*
 * Name :
 *      illuminate
 *
 * Params :
 *      int index : elevator number
 *      QString : either will be "Floor" or "Elevator" to indicate which button should be illuminated
 *      int ifcab_floor : optional param , as specified in the "button.h" file , only needed if the button is illuminated inside a cab , need to know the index of the cab
 *
 * returns :
 *      void
 *
 */
void Button::illuminate(const int index , const QString location , const int ifcab_floor)
{
    if (location == "Floor"){
        m_browser->append("Floor (" + QString::number(index) + ") - button illuminated!");
    }
    else{
         m_browser->append("Cab (" + QString::number(index + 1) + ") to floor (" + QString::number(ifcab_floor) + ") - button illuminated!");
    }

}

/*
 * Name :
 *      illuminate
 *
 * Params :
 *      int index : elevator number
 *      QString : either will be "Floor" or "Elevator" to indicate which button should be deilluminated
 *      int ifcab_floor : optional param , as specified in the "button.h" file , only needed if the button is deilluminated inside a cab , need to know the index of the cab
 *
 * returns :
 *      void
 *
 */
void Button::deilluminate(const int index , const QString location, const int ifcab_floor)
{
    if (location == "Floor"){
        m_browser->append("Floor (" + QString::number(index) + ") - button de-illuminated!");
    }
    else{
         m_browser->append("Cab (" + QString::number(index + 1) + ") to floor (" + QString::number(ifcab_floor) + ") - button de-illuminated!");
    }
}

