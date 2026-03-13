#include "VideoUrlCallback.h"

/*!
   * \brief IconUrlCallback::operator () 函子操作符。
   * \param clientHeightResult The running result.
   */
void VideoUrlCallback::operator() (const QVariant & clientHeightResult)
{
//  auto iconUrl=clientHeightResult.toString();
  auto iconUrl=clientHeightResult;

  webView->videoUrlFound(iconUrl);
}

void VideoUrlCallback::setWebView(StbWebView *value)
{
  webView = value;
} //void IconUrlCallback::operator() (const QVariant & clientHeightResult)
