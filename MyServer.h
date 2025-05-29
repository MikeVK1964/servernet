/* ======================================================================
**  MyServer.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _MyServer_h_
#define _MyServer_h_

#include <QWidget>
#include "mknetsalut.h"

class QTcpServer;
class QTextEdit;
class QTcpSocket;
class DocTras;


// ======================================================================
class MyServerWidget : public QWidget {
Q_OBJECT

private:
    QTcpServer* m_ptcpServer;
    MKNetServer* m_pmkServer;

    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;
    QTcpSocket* pClientSocket = nullptr;
private:
//    void sendToClient(QTcpSocket* pSocket, const QString& str);
//    double trace_time; // время движения с начала имитации в секундах
   DocTras* m_pDocument;
   void MkSendToClient(double trace_time);

public:
    MyServerWidget(int nPort, QWidget* pwgt = 0);

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
            void Load();

};
//class MyWidget :
#endif  //_MyServer_h_
