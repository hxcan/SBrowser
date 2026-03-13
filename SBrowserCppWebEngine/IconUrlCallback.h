#ifndef ICON_URL_CALLBACK_H
#define ICON_URL_CALLBACK_H

#include "StbWebView.h" // StbWebView

QT_FORWARD_DECLARE_CLASS(StbWebView)

class IconUrlCallback
{
public:
  void operator() (const QVariant & clientHeightResult); //!< 函子操作符。
  void setWebView(StbWebView *value); //!< Set the web view object.

private:
  StbWebView * webView=nullptr; //!< The web view.
};

#endif
