#pragma once

#include <QMutex>
#include <QQuickWindow>

#include <set>

namespace qmlOsgBridge
{

class WindowsStorage
{
public:
  static WindowsStorage& get();

  void addQuickWindow(const QPointer<QQuickWindow>& window);
  void closeAll();

private:
  std::set<QPointer<QQuickWindow>> m_windows;
  QMutex m_mutex;

  WindowsStorage() = default;

};

}
