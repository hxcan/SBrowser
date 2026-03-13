#include "IconUrlCallback.h"

/*!
   * \brief IconUrlCallback::operator () 函子操作符。
   * \param clientHeightResult The running result.
   */
void IconUrlCallback::operator() (const QVariant & clientHeightResult)
{
//  auto iconUrl=clientHeightResult.toString();
  auto iconUrl=clientHeightResult;

  webView->iconUrlFound(iconUrl);
}

void IconUrlCallback::setWebView(StbWebView *value)
{
  webView = value;
} //void IconUrlCallback::operator() (const QVariant & clientHeightResult)
