#include "SbPreCompile.h" //UserScriptManager

class UserScriptManager : public QObject
{
  Q_OBJECT

public:
    UserScriptManager(); //!<构造函数
    virtual ~UserScriptManager();

private:
    void initializeWebSettings(); //!<初始化网页设置。
    QWebEngineScript loadUserScript(const QString & ScriptsremoveBlmzzaqwzdllshjs) const; //!<载入用户脚本。
};
