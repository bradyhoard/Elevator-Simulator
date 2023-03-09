#include "button.h"
#include <QDebug>

Button::Button(QTextBrowser *browser, QObject *parent) : QObject(parent)
{
    m_browser = browser;
}

void Button::illuminate()
{
    m_browser->append("Button illuminated!");
}

void Button::deilluminate()
{
    m_browser->append("Button deilluminated!");
}

