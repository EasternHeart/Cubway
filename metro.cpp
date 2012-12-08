#include "metro.h"
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QProcess>
#include "lua/lua.hpp"


Metro::Metro(QWidget *parent)
    : QWebView(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    if(QApplication::arguments().length() <= 1)
        load(QUrl("https://metro-subway.rhcloud.com/MT.php"));
    else
        load(QUrl(QApplication::arguments()[1]));
    showFullScreen();
    connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(javaScriptWindowObjectCleared()));
    lua = luaL_newstate();
	RunLua("libmetro.lua");
}

Metro::~Metro()
{
    lua_close(lua);
}

void Metro::javaScriptWindowObjectCleared()
{
    page()->mainFrame()->addToJavaScriptWindowObject("MetroView", this);
}

void Metro::QtAlert(QString str)
{
    QMessageBox::information(this,"QtAlert",str);
}

void Metro::System(QString str)
{
    QProcess *qp = new QProcess;
    qp->start(str);
}

void Metro::RunLua(QString str)
{
    luaL_loadfile(lua,str.toAscii());
    lua_pcall(lua,0,LUA_MULTRET,0);
}

void Metro::RunLuaString(QString str)
{
    luaL_loadstring(lua,str.toAscii());
    lua_pcall(lua,0,LUA_MULTRET,0);
}
