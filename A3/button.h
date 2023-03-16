#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QTextBrowser>

class Button : public QObject
{
    Q_OBJECT
public:
    explicit Button(QTextBrowser *browser, QObject *parent = nullptr);

public slots:
    void illuminate(const int index , const QString location , const int ifcab_floor = 0);
    void deilluminate(const int index , const QString location , const int ifcab_floor = 0);

private:
    QTextBrowser *m_browser;
};

#endif // BUTTON_H
