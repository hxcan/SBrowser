#include "TdPreCompile.h" //cout

#include "mapReduceFunctions.h" //addToSubDirList

/*!
 * \brief addToSubDirList 将某个目录下列出的子目录归并到总的结果列表中。
 * \param result 归并之后的结果列表。
 * \param partListedSubDirs 要归并的中间结果。
 */
void addToSubDirList(QStringList &result,const QList<QString> & partListedSubDirs)
{
    result.append(partListedSubDirs); //追加到列表末尾。
} //void addToSubDirList(QStringList &result,const QList<QString> & partListedSubDirs)

/*!
 * \brief getSubDirs 获取某个目录下的子目录。
 * \param dir2Scan 要扫描的目录。
 * \return 扫描得到的结果。
 */
QList<QString> getSubDirs(const QString dir2Scan)
{
    QList<QString> result; //结果。

    QDir CrtDir(dir2Scan); //当前目录。
    QDir::Filters DirFlt= QDir::Dirs | QDir::Hidden; //不列出文件。
    CrtDir.setFilter(DirFlt); //设置过滤器。
    QFileInfoList FlInfLst=CrtDir.entryInfoList(); //获取文件信息列表。


    QFileInfo CrtFlInfo; //当前的文件信息。

    foreach(CrtFlInfo,FlInfLst) //一个个文件地显示。
    {
        if ((CrtFlInfo.fileName()==".") || (CrtFlInfo.fileName()=="..")) //忽略目录自身及上级目录。
        {

        } //if ((CrtFlInfo.fileName()==".") || (CrtFlInfo.fileName()=="..")) //忽略目录自身及上级目录。
        else //其它目录都添加进去。
        {
            result << CrtFlInfo.absoluteFilePath(); //加入到结果列表中。
        } //else //其它目录都添加进去。


    } //Q_FOREACH


    return result;
} //QList<QString> getSubDirs(const QString dir2Scan)

/*!
 * \brief addToFileList 将某个目录下列出的文件列表归并到总的结果列表中。
 * \param result 结果列表。
 * \param partListedFiles 当前目录下列出的文件列表。
 */
void addToFileList(QStringList &result,const QList<QString> &partListedFiles)
{
    result.append (partListedFiles); //追加到列表末尾。

    return;
} //void addToFileList(QStringList &result,const QList<QString> &partListedFiles)

QList<QString> getSubDirFileList(const QString dir2Scan) //!<获取某个目录下的文件列表。
{
    QList<QString> result; //结果。

    QDir CrtDir(dir2Scan); //当前目录。
    QDir::Filters DirFlt=QDir::Files  | QDir::Hidden; //列出文件。不列出子目录。
    CrtDir.setFilter(DirFlt); //设置过滤器。
    QFileInfoList FlInfLst=CrtDir.entryInfoList(); //获取文件信息列表。


    QFileInfo CrtFlInfo; //当前的文件信息。

    if (FlInfLst.isEmpty()) //目录里面没有文件。可能有子目录。
    {
        if (CrtDir.isEmpty()) //此目录为空。
        {
            CrtDir.removeRecursively(); //删除此目录。
        } //if (CrtDir.isEmpty()) //此目录为空。
    } //if (FlInfLst.isEmpty()) //目录里面没有文件。可能有子目录。
    else //目录里面有文件。
    {

    foreach(CrtFlInfo,FlInfLst) //一个个文件地显示。
    {
        result << CrtFlInfo.absoluteFilePath(); //加入到结果列表中。
    } //Q_FOREACH
    } //else //目录里面有文件。


    return result;
} //QList<QString> getSubDirFileList(const QString dir2Scan)

/*!
 * \brief addToMediaList 将某个文件的识别结果添加到结果列表中。
 * \param result 总的结果列表。
 * \param fileRecognized 识别出来的有用的多媒体文件的文件名。如果为空，则表示所对应的那个文件实际上不是多媒体文件。
 */
void addToMediaList(QStringList &result,const QString &fileRecognized)
{
    if (fileRecognized.isEmpty ()) //为空，则不是多媒体文件。
    {

    } //if (fileRecognized.isEmpty ()) //为空，则不是多媒体文件。
    else //是有用的多媒体文件。
    {
        result << fileRecognized; //加入到结果列表中。
    } //else //是有用的多媒体文件。

    return;
} //void addToMediaList(QStringList &result,const QString &fileRecognized)

/*!
 * \brief recognizeFile 识别一个文件是不是有用的多媒体文件。如果是，则返回文件名。如果不是，则返回空字符串。
 * \param file2Recog 要识别的文件名。
 * \return 识别的结果。如果是有用的多媒体文件，则原样返回文件名。如果不是，则返回空字符串。
 */
QString recognizeFile(const QString CrtFlNm)
{
    QString result=""; //结果。

    QMimeDatabase db; //Mime databse.
    QMimeType mime = db.mimeTypeForFile(CrtFlNm); //Judge the mime type.



    if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/x-font-ttf")) //It is font file.
    {
        result=CrtFlNm; //是有用的文件。
    }
    else if (mime.inherits ("application/x-xz")) //xz文件.
    {
        result=CrtFlNm; //是有用的文件。
    }
    else if (mime.inherits ("image/x-tga")) //tga图片。
    {
        result=CrtFlNm; //是有用的文件。
    }
    else if (mime.inherits ("image/svg+xml")) //svg图片。
    {
        result=CrtFlNm; //是有用的文件。
    }
    else if (mime.inherits ("image/webp")) //webp图片。
    {
        result=CrtFlNm; //是有用的文件。
    }
    else if (mime.inherits ("video/x-msvideo")) //avi视频文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //else if (mime.inherits ("video/x-msvideo")) //avi视频文件。
    else if (mime.inherits ("video/quicktime")) //quicktime视频文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //else if (mime.inherits ("video/x-msvideo")) //avi视频文件。
    else if (mime.inherits ("application/font-woff")) //It is font file.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/pdf")) //PDF文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("audio/x-wav")) //It is audio file.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("audio/AMR")) //这是一个声音文件.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/x-shockwave-flash")) //It is flash file.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("audio/mpeg")) //It is mp3 file.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("audio/mp4")) //It is m4a file.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("video/mpeg")) //It is mpeg video file.
    {
        result=CrtFlNm; //是有用的文件。
    } //else if (mime.inherits ("video/mpeg")) //It is mpeg video file.
    else if (mime.inherits ("video/x-flv")) //flash视频。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("video/mp4")) //It is mp4 video file.
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/ogg")) //ogg多媒体文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/vnd.rn-realmedia")) //real media文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/vnd.ms-powerpoint")) //ppt文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/msword")) //doc文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("audio/x-riff")) //riff声音文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("image/png")) //png文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/zip")) //zip文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/x-sqlite3")) //sqlite3文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //if (mime.inherits ("video/x-ms-asf")) //It is asf video file.
    else if (mime.inherits ("application/gzip")) //gzip压缩文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //else if (mime.inherits ("application/x-java-keystore")) //java keystore文件。
    else if (mime.inherits ("text/x-python")) //python源代码文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //else if (mime.inherits ("application/x-java-keystore")) //java keystore文件。
    else if (mime.inherits ("application/x-java-keystore")) //java keystore文件。
    {
        result=CrtFlNm; //是有用的文件。
    } //else if (mime.inherits ("application/x-java-keystore")) //java keystore文件。
    else //不是任何能够通过QT5自动判断出来的类型。
    {
        qDebug() << __FILE__ << __LINE__ << __func__ << QObject::tr("mime type:") << mime.name(); //Debug.

        //Try with image formats:
        QImage CrtPxmp; //The pixmap object.用于尝试载入图片文件的图片对象。

        bool LdRst=false; //Whether loaded successfully.

        QString CrtFmt; //Current format.
        QStringList FmtStrLst; //Formats string list.

        FmtStrLst << "JPG" << "PNG" << "BMP"  << "JPEG" << "PBM" << "PGM" << "PPM" << "XBM" << "XPM"; //Add the formats.


        foreach(CrtFmt,FmtStrLst) //Test the formats one by one.
        {
            LdRst=CrtPxmp.load(CrtFlNm,CrtFmt.toUtf8().data()); //Try to load.

            if (LdRst) //Load successfully.
            {

                break; //No need to try other formats.

            } //if (CrtPxmp.load(CrtFlNm)) //Load successfully.
        } //foreach(CrtFmt,FmtStrLst) //Test the formats one by one.


        if (LdRst) //成功地通过QT5以图片的方式载入。则认为是一张图片。
        {
            result=CrtFlNm; //是有用的文件。
        } //if (CrtPxmp.load(CrtFlNm)) //Load successfully.
    } //else //Not video file.

    return result;
} //QString recognizeFile(const QString file2Recog)
