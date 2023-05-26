#include "WindowsStorage.h"

namespace qmlOsgBridge
{

WindowsStorage& WindowsStorage::get()
{
  static WindowsStorage instance;
  return instance;
}

void WindowsStorage::addQuickWindow(const QPointer<QQuickWindow>& window)
{
  QMutexLocker locker(&m_mutex);
  if (m_windows.count(window) == 0)
  {
    QObject::connect(window, &QQuickWindow::sceneGraphInvalidated, [this, window]()
    {
      QMutexLocker locker(&m_mutex);
      const auto it = m_windows.find(window);
      if (it == m_windows.end())
      {
        return;
      }

      m_windows.erase(it);
    });
    m_windows.insert(window);
  }
}

void WindowsStorage::closeAll()
{
  QMutexLocker locker(&m_mutex);
  for (const auto& window : m_windows)
  {
    if (window)
    {
      window->close();
    }
  }
  m_windows.clear();
}

}
