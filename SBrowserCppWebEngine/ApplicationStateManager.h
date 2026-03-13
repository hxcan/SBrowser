#include "SbPreCompile.h" // QTimer

#ifndef APPLICATION_STATE_MANAGER_H
#define APPLICATION_STATE_MANAGER_H

class ApplicationStateManager : public QObject
{
  Q_OBJECT
public:
  ApplicationStateManager();
  void rememberApplicationState(const Qt::ApplicationState & state); //!< Remember application state. Chen xin.

private:
  QTimer idleQuitTimer; //!< Idle quit timer.

signals:
  void shouldCheckClipboardUrl(); //!< Should check clipboard url.
  void shouldQuit() const; //!< Idle , then quit.
  void shouldPauseVideo() const; //!< Should pause video.
};

#endif // APPLICATION_STATE_MANAGER_H
