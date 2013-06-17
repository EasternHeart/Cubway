#ifndef METRO_H
#define METRO_H

#include <QtGui/QWidget>
#include <QtWebKit/QWebSettings>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebView>
#include <QtGui/QKeyEvent>
#include "lua/lua.hpp"

class Metro : public QWebView
{
    Q_OBJECT
public:
    Metro(QWidget *parent = 0);
    ~Metro();
    static Metro *Mainview;
protected:
    lua_State *lua;
    void keyPressEvent(QKeyEvent *ke);
public slots:
    void javaScriptWindowObjectCleared();
    void QtAlert(QString str);
    QString System(QString str);
    QString OpenFile();
    QString GetColor();
    void RunLua(QString str);
    void RunLuaString(QString str);
    void Hide();
};

#endif // METRO_H
