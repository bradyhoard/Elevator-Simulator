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
    void illuminate();
    void deilluminate();

private:
    QTextBrowser *m_browser;
};

#endif // BUTTON_H
