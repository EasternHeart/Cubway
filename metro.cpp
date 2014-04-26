#include <QDebug>
#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QStringList>
#include <QByteArray>
#include <QMultiMap>
#include <QtGui/QDialog>
#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>
#include <QtGui/QFontDialog>
#include <QtGui/QKeyEvent>
#include <QProcess>
#include <QPluginLoader>
#include <QMessageBox>
#include "metro.h"
#include "moduleloader.h"
#include "MFile.h"

Metro::Metro(QWidget *parent)
    : QWebView(parent)
{
    QWebSettings* defaultSettings = QWebSettings::globalSettings();  
    QWebSettings::enablePersistentStorage("");  
    // Inspect Support
    defaultSettings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    defaultSettings->setAttribute(QWebSettings::JavascriptEnabled, true);  
    // Plug-ins must be set to be enabled to use plug-ins.  
    defaultSettings->setAttribute(QWebSettings::PluginsEnabled,true);  
    defaultSettings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);  
    defaultSettings->setObjectCacheCapacities(0, 0, 0); 
    setWindowTitle("QtMetro - Cubway");
//    setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
    if(QApplication::arguments().length() <= 1)
        load(QUrl("http://erhandsome.org/subway"));
    else
        load(QUrl(QApplication::arguments()[1]));
//    showFullScreen();

    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(HandleMetaData()) );
    connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(javaScriptWindowObjectCleared()) );
  addObject("MetroView", this);
  addObject("require",&_moduleLoader);
    Mainview = this;
}
Metro* Metro::Mainview = NULL;

Metro::~Metro()
{
    //lua_close(lua);
    Mainview = NULL;
}


void Metro::HandleMetaData(){
  QMultiMap<QString, QString> Data = page()->mainFrame()->metaData();
  if(!Data.values("subway_title").isEmpty()) setWindowTitle(Data.values("subway_title").at(0));
  if(!Data.values("subway_size").isEmpty()){
    QString sizeStr = Data.values("subway_size").at(0);
    QStringList sizeXY = sizeStr.split("x");
    if(sizeXY.size()==2)
      resize(sizeXY[0].toInt(),sizeXY[1].toInt());
  }

  show();
}


void Metro::addObject(QString name, QObject *_object){
  page() -> mainFrame() -> addToJavaScriptWindowObject(name, _object);
  _objects[name] = _object;
}


void Metro::javaScriptWindowObjectCleared()
{
    QHashIterator<QString, QObject*> i(_objects);
    while (i.hasNext()) {
        i.next();
        page()->mainFrame()->addToJavaScriptWindowObject(i.key(),i.value());
    }
}

QString Metro::GetArg(int n)
{
  if(n>QApplication::arguments().length()||n<0){
    return "undefined";
  }else{
    return  QApplication::arguments()[n];
  }
}

int Metro::GetArgsLen()
{
  return  QApplication::arguments().length();
}

QString Metro::System(QString str)
{
    QProcess *qp = new QProcess;
    qp->start(str);
    if (!qp->waitForStarted())
      return "1";
    if (!qp->waitForFinished())
      return "2";
    QByteArray result = qp->readAll();
    return QString(result);
}

void Metro::Echo(QString str)
{
qDebug()<<str;
}

QString Metro::Exec(QString str,QString args,int wait)
{
    QProcess *qp = new QProcess;
    if (!args.isNull()||!args.isEmpty()){
    QStringList Args=args.split(' ');
    qp->start(str,Args);
    }else qp->start(str);

    if (wait==0){
    if (!qp->waitForStarted())
      return "1";
    if (!qp->waitForFinished())
      return "2";
    QByteArray result = qp->readAll();
    return QString(result);
    } else return "-1";
}

void Metro::aExec(QString str)
{
    QProcess *qp = new QProcess;
    qp->start(str);
}

QString Metro::OpenFile()
{
    return QFileDialog::getOpenFileName(this,tr("Open File"));
}

QString Metro::GetColor()
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()){
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    return "rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")";
    }else{
    return "-1";
    }
}

QString Metro::GetFont(QString family,int size,QString weight,QString style)
{
    bool ok;
    bool _style;
    QFont font;
    QString font_style;
    QString font_weight;
    QString font_family;
    if (style=="italic") _style=true; else _style=false; 
    if(weight=="bold")
    font = QFontDialog::getFont(&ok,QFont(family, size,QFont::Bold,_style),this);
    else
    font = QFontDialog::getFont(&ok,QFont(family, size,QFont::Normal,_style),this);
    if (ok) {
    font_family=font.family();
    if(font.italic()) font_style="italic"; else font_style="normal";
    if(font.bold()) font_weight="bold"; else font_weight="normal";
//    return "font-family:"+font_family.replace(QString(" "), QString("-"))+";font-size:"+QString::number(font.pointSize())+";font-style:"+font_style+";font-weight:"+font_weight+";";
//    return font_family.replace(QString(" "), QString("-"))+" "+QString::number(font.pointSize())+"px "+font_style+" "+font_weight;
    return font_style+" "+font_weight+" "+QString::number(font.pointSize())+"px "+'"'+font_family+'"';
    }else{
    return "-1";
    }
}

void Metro::WinTitle(QString title)
{
    Mainview->setWindowTitle(title);
}

void Metro::WinResize(int w,int h)
{
    Mainview->resize(w,h);
}

void Metro::WinPos(int x,int y)
{
    QDesktopWidget* desktop = QApplication::desktop();
    if(x==-1&&y==-1){
    Mainview->move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    }else{
    Mainview->move(x,y);
    }
}

void Metro::WinFlag(QString flag)
{
    Qt::WindowFlags flags = 0;
    flags = Qt::Window;
    if(flag=="below") flags |= Qt::WindowStaysOnBottomHint;
    if(flag=="above") flags |= Qt::WindowStaysOnTopHint;
    Mainview->setWindowFlags(flags);
    show();
}

void Metro::WinFullScreen()
{
    Mainview->showFullScreen();
}

void Metro::WinMaximize()
{
    Mainview->showMaximized();
}

void Metro::WinMinimize()
{
    Mainview->showMinimized();
}

void Metro::WinNormal()
{
    Mainview->showNormal();
}

void Metro::QtAlert(QString str)
{
    QMessageBox::information(this,"QtAlert",str);
}

void Metro::keyPressEvent(QKeyEvent *ke)
{
    char *buf;
    buf = new char[snprintf(NULL,0,"onKeyPressEvent(%d)",
        ke->key())+1];
    sprintf(buf,"onKeyPressEvent(%d)",
        ke->key());
    page()->mainFrame()->evaluateJavaScript(buf);
    delete[] buf;
    QWebView::keyPressEvent(ke);
}

void Metro::keyReleaseEvent(QKeyEvent *ke)
{
    char *buf;
    buf = new char[snprintf(NULL,0,"onKeyReleaseEvent(%d)",
        ke->key())+1];
    sprintf(buf,"onKeyReleaseEvent(%d)",
        ke->key());
    page()->mainFrame()->evaluateJavaScript(buf);
    delete[] buf;
    QWebView::keyPressEvent(ke);
}

void Metro::resizeEvent(QResizeEvent * event)
{
	char *buf;
	buf = new char[snprintf(NULL,0,"onResizeEvent(%d,%d,%d,%d)",
		event->size().width(),event->size().height(),
        event->oldSize().width(),event->oldSize().height())+1];
	sprintf(buf,"onResizeEvent(%d,%d,%d,%d)",
		event->size().width(),event->size().height(),
		event->oldSize().width(),event->oldSize().height());
	page()->mainFrame()->evaluateJavaScript(buf);
	delete[] buf;
	QWebView::resizeEvent(event);
}

void Metro::Hide()
{
    hide();
}

