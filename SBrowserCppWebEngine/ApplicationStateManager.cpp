#include "ApplicationStateManager.h"

ApplicationStateManager::ApplicationStateManager()
{
  idleQuitTimer.setInterval (10*60*1000); // 10 minutes.
  idleQuitTimer.setSingleShot (true); // Single shot.

  connect (&idleQuitTimer, &QTimer::timeout, this, &ApplicationStateManager::shouldQuit); // Idle , then quit.
} // ApplicationStateManager::ApplicationStateManager()

/*!
 * \brief ApplicationStateManager::rememberApplicationState Remember application state. Chen xin.
 * \param state
 */
void ApplicationStateManager::rememberApplicationState(const Qt::ApplicationState & state)
{
  switch (state)
  {
  case Qt::ApplicationInactive: // In active
    idleQuitTimer.start(); // Start the idle quit timer.

    emit shouldPauseVideo(); // Should pause video.

    break;

  case Qt::ApplicationActive: // Active
    idleQuitTimer.stop(); // Stop the idle quit timer.
    emit shouldCheckClipboardUrl(); // Should check clipboard url.

    break;

  default:
    break;
  } // switch (state)
} // void ApplicationStateManager::rememberApplicationState(const Qt::ApplicationState & state)
