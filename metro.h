#ifndef METRO_H
#define METRO_H

#include <iostream>
#include <QtGui/QWidget>
#include <QSize>
#include <QtWebKit/QWebSettings>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebView>
#include <QtGui/QKeyEvent>
#include <QMultiMap>
#include <QVariant>
#include <QHash>
#include "moduleloader.h"
using namespace std;


class QFile;
class QDir;


class Metro : public QWebView
{
    Q_OBJECT
public:
    Metro(QWidget *parent = 0);
    ~Metro();
    static Metro *Mainview;
protected:
//    lua_State *lua;
    void keyPressEvent(QKeyEvent *ke);
    void keyReleaseEvent(QKeyEvent *ke);
    void resizeEvent(QResizeEvent * event); 
    void addObject(QString name, QObject *_object);
    void initFolders();
    void doWebSettings();
private:
    QMultiMap<QString, QString> MetaData;
    QString InitFunction;
    QHash<QString, QObject*> _objects;
    ModuleLoader _moduleLoader;
    QHash<QString, bool> EventsEnabled;
    QFile *file;
    QString dir;
    QDir *userDir;
    QDir *settingsDir;
signals:
    void LinkFragment(QString _fragment);
public slots:
    void HandleMetaData();
    void javaScriptWindowObjectCleared();
    QString GetArg(int n);
    int GetArgsLen();
    void QtAlert(QString str);
    void Echo(QString str);
    /*
    QString System(QString str);
    QString Exec(QString str,QString args,int wait);
    void aExec(QString str);
    */
    QString OpenFile(QString Dir, QString Filters);
    QVariantMap GetColor(QVariantMap initial);
    QString GetFont(QString family,int size,QString weight,QString style);
    QVariantMap WinSizeHint();
    void WinTitle(QString title);
    void WinResize(int w,int h);
    void WinPos(int x,int y);
    void WinFullScreen();
    void WinMaximize();
    void WinMinimize();
    void WinNormal();
    QVariantMap ScrollBar();
    void LinkClicked(const QUrl &_url);
    QString getFileDir();
    QString getSettings(QString AppName);
    void setSettings(QString AppName, QString str);
};

#endif //METRO_H
