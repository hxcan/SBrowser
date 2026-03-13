#ifndef VIDEO_URL_CALLBACK_H
#define VIDEO_URL_CALLBACK_H

#include "StbWebView.h" // StbWebView

QT_FORWARD_DECLARE_CLASS(StbWebView)

class VideoUrlCallback
{
public:
  void operator() (const QVariant & clientHeightResult); //!< The actual operator.

  void setWebView(StbWebView *value);

private:
  StbWebView * webView=nullptr; //!< The web view.
};



#endif // VIDEO_URL_CALLBACK_H
