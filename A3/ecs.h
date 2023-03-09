#ifndef ECS_H
#define ECS_H

#include <QObject>
#include <QTextBrowser>

class ECS : public QObject
{
    Q_OBJECT
public:
    explicit ECS(QTextBrowser *browser, QObject *parent = nullptr);

public slots:
    void emergency(const QString& em);

private:
    QTextBrowser *m_browser;
};

#endif // ECS_H
