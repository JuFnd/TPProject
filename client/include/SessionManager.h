#pragma once

#include "Authorization.h"
#include "Messeger.h"
#include "Registration.h"
#include "qobject.h"

class SessionManager : public QObject
{
    Q_OBJECT
public:
    SessionManager();
    ~SessionManager();

    void start();

public slots:
    void slotAuthTrue();

    void onRegistration();

private:
    Messeger *w;
    Authorization *auth;
    Registration *reg;
    QTcpSocket *socket;
};
