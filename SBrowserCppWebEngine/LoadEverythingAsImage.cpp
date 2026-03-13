
#include "SbPreCompile.h" //QMainWindow

//#include "Toast.h" //Toast
#include "SBrowserCppWebEngine.h" //SBrowserWindow
#include "ui_passworddialog.h" //PasswordDialog

/*!
 * \brief SBrowserWindow::parseImageRaw 解析成图片。
 * \param imageFileName 文件名。
 * \return 解析得到的图片。
 */
QImage LoadEverythingAsImage::parseImageRaw(QString imageFileName)
{
    QImage result; //结果图片。
    
    
    QFile imageFile(imageFileName);
    imageFile.open(QIODevice::ReadOnly);
    
    QByteArray content=imageFile.readAll();
    
    imageFile.close();

    QByteArray * filecontent=new QByteArray(content); //复制一份

    
    int byteAmount=content.length(); //字节数。
    
    int width=qSqrt(byteAmount/4); //计算出宽度。
    
    if (width) //宽度存在。
    {
        int height=byteAmount/4/width; //高度。
        
//        cout << __FILE__ << ", " << __LINE__ << ", " << __func__  <<  ", width: " << width << ", height: " << height << endl;
        
        int bytesPerLine=4*width; //每行的字节数。
        
        
        
        result=QImage((uchar*)(filecontent->data()),width,height,bytesPerLine,QImage::Format_RGBA8888);
        
        
//        result.save("resultRaw.png");
    } //if (width) //宽度存在。
    
    
    return result;
} //QImage SBrowserWindow::parseImageRaw(QString imageFileName)

/*!
 * \brief SBrowserWindow::parseImage 解析成图片。
 * \param imageFileName 文件名。
 * \return 解析得到的图片。
 */
QImage LoadEverythingAsImage::parseImage(QString imageFileName)
{
    QFile imageFile(imageFileName);
    imageFile.open(QIODevice::ReadOnly);
    
    QByteArray content=imageFile.readAll();
    
    imageFile.close(); //關閉文件。
    
    int byteAmount=content.length(); //字节数。
    
    int width=qSqrt(byteAmount/4); //计算出宽度。
    
    int height=0; //高度。

    if (width) //宽度存在。
    {
        height=byteAmount/4/width; //高度。
    } //if (width) //宽度存在。
    
    QImage result(QSize(width,height),QImage::Format_ARGB32); //结果图片。
    
    quint8 red=0;
    quint8 green=0;
    quint8 blue=0;
    quint8 alpha=0;
    
    for(int y=0;y<height;y++) //一行行地解析。
    {
        for(int x=0;x<width;x++) //一列列地解析。
        {
            int redByteIndex=(y*width+x)*4;
            int greenByteIndex=(y*width+x)*4+1;
            int blueByteIndex=(y*width+x)*4+2;
            int alphaByteIndex=(y*width+x)*4+3;
            
            
            red+=content[redByteIndex];
            green+=content[greenByteIndex];
            blue+=content[blueByteIndex];
            alpha+=content[alphaByteIndex];
            
            result.setPixel(x,y,qRgba(red,green,blue,alpha));
        } //for(int x=0;x<width;x++) //一列列地解析。
    } //for(int y=0;y<height;y++) //一行行地解析。
    
    return result;
} //QImage SBrowserWindow::parseImage(QString imageFileName)

/*!
 * \brief SBrowserWindow::decodeReawFile 解码选中的图片。
 * \param imageFileName 图片文件。
 */
void LoadEverythingAsImage::decodeRawFile(QString imageFileName)
{
    QImage result(imageFileName); //结果图片。
    
    quint8 red=0;
    quint8 green=0;
    quint8 blue=0;
    quint8 alpha=0;
    
    int width=result.width(); //计算出宽度。
    
    int height=result.height(); //高度。
    
    QFile imageFile("decodedFile.file");
    imageFile.open(QIODevice::WriteOnly);
    
    QByteArray content;
    
    
    for(int y=0;y<height;y++) //一行行地解析。
    {
        for(int x=0;x<width;x++) //一列列地解析。
        {
            red=qRed(result.pixel(x,y));
            green=qGreen(result.pixel(x,y));
            blue=qBlue(result.pixel(x,y));
            alpha=qAlpha(result.pixel(x,y));
            

            
            content.append(char(red));
            content.append(char(green));
            content.append(char(blue));
            content.append(char(alpha));
        } //for(int x=0;x<width;x++) //一列列地解析。
    } //for(int y=0;y<height;y++) //一行行地解析。
    
    imageFile.write(content);

    imageFile.close();
    
} //void SBrowserWindow::decodeReawFile(QString imageFileName)
