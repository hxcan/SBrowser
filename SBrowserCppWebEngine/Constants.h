#ifndef CONSTANTS_H
#define CONSTANTS_H

/*!
 * \brief The TransparentMode enum 透明模式。
 */
enum TransparentMode
{
    BlackAsTransparent=0, //!<黑色的点作为透明颜色。
    WhiteAsTransparent=1 //!<白色的点画成透明颜色。
}; //enum TransparentMode


const QByteArray FirefoxUserAgent="Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0 SBrowser/2017.8.15"; //!<火狐浏览器的代理字符串。

const QString DefaultUserAgent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) QtWebEngine/5.10.1 Chrome/61.0.3163.140 Safari/537.36"; //!<默认的用户代理字符串。

#endif // CONSTANTS_H
