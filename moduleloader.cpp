#include "moduleloader.h"
#include <QPluginLoader>
#include <QDebug>
// Builtins 
#include "FileSystem.h"
#include "Process.h"

#if defined(Q_OS_WIN32)
#define _LIB_END ".dll"
#else
#define _LIB_END ".so"
#endif

QObject* ModuleLoader::load(QString name)
{
    if (name.endsWith(".js")) {
	/* not implemented */
        return NULL;
    }
    if (isBuiltin(name)) {
        return getBuiltin(name);
    }
    QPluginLoader* pluginloader=new QPluginLoader(this);
    pluginloader->setLoadHints(QLibrary::ResolveAllSymbolsHint);
    pluginloader->setFileName(this->getModulePath(name));
    if (pluginloader->load()){
        return pluginloader->instance();
    } else {
        return NULL;
    }
}

QString ModuleLoader::getModulePath(QString mod)
{
  qDebug() << "Load Module:" << mod;
  if (!_dirModule.exists(mod)) {
    if (QDir::current().exists(mod+_LIB_END)) {
      return QDir::current().filePath(mod+_LIB_END);
    }else{
      qDebug() << "Error: Module file not found in current directory";
      return NULL;
    }
  }else{
    QDir moddir(_dirModule.filePath(mod));
    if (!moddir.exists("lib"+mod+_LIB_END)) {
      qDebug() << "Error: Module file " << "lib"+mod+_LIB_END << " not exist";
      return NULL;
    }else{
      return moddir.filePath("lib"+mod+_LIB_END);
    }
  }
}

bool ModuleLoader::isBuiltin(QString name)
{
    if (name == "FileSystem")
        return true;
    else if (name == "Process")
      return true;
    return false;
}

QObject* ModuleLoader::getBuiltin(QString name)
{
    if (name == "FileSystem")
        return new FileSystem();
    else if (name == "Process")
        return new Process();
    return NULL;
}

#undef _LIB_END
