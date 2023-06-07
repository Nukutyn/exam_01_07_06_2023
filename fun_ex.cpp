#include "fun_ex.h"

QByteArray parce(QString mees)
{
    QStringList zapros = mees.split("&");

    if(zapros[0]=="start")
    {
        zapros.removeFirst();
        return start(zapros).toUtf8();
    }
    else if(zapros[0]=="break")
    {
        zapros.removeFirst();
        QString login = zapros[0];
        QString roomname = zapros[1];
    }
    else
    {
        zapros.clear();
        return "error";
    }

    return 0;
}


QString start(QStringList temp)
{
    QString ans;

    return ans;
}
