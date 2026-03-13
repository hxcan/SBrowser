#include "SbPreCompile.h" //QShortcut

#include "StbWebView.h" //StbWebView
#include "StartSlideGoBackTop.h" //StartSlideGoBackTop
#include "StbWebPage.h" //StbWebPage

/*!
 * \brief StbWebPage::certificateError 证书错误。
 * \param certificateError 证书错误对象。
 * \return 是否接受该证书。
 */
bool StbWebPage::certificateError(const QWebEngineCertificateError & certificateError)
{
  bool result=false; //结果。

  // check if SSL certificate has been trusted already
  QString replyHost = certificateError.url().host() + QString(":%1").arg(certificateError.url().port());
  if(! sslTrustedHostList.contains(replyHost)) //不在 信任 主機列表中。
  {
    QStringList errorStrings;
    QString errors = certificateError.description (); // 获取错误描述。

    int ret = QMessageBox::warning(0, QCoreApplication::applicationName(),
                                   tr("SSL Errors:\n\n%1\n\n%2\n\n"
                                      "Do you want to ignore these errors for this host?").arg(certificateError.url().toString()).arg(errors),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
      result=true; //忽略该错误。
      sslTrustedHostList.append(replyHost);
    }
  } //if(! sslTrustedHostList.contains(replyHost)) //不在 信任 主機列表中。

  return result;
} //bool StbWebPage::certificateError(const QWebEngineCertificateError & certificateError)

