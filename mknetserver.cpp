#include <qmessagebox.h>
#include <QtNetwork>

#include "mknetsalut.h"
#include "DocTrace.h"

MKNetServer::MKNetServer(int nPort,DocTras* m_ppDocument, QWidget* pwgt )
    : QObject{}
{
    m_ptcpServer = new QTcpServer(pwgt);
    this->m_pDocument=m_ppDocument;

    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + m_ptcpServer->errorString()
                             );
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );

}


/*virtual*/ void MKNetServer::slotNewConnection()
{
    pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            this, SLOT(deleteLater())
           );

    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );
    qDebug() << "New Connetion";


}

void MKNetServer::slotReadClient   ()
{
    pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_10);
    double trace_time;
    in >> trace_time;
    qDebug()<< "trace_time1 " << trace_time;
    MkSendToClient(trace_time);


}
void MKNetServer::MkSendToClient(double trace_time)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    qsizetype tt=m_pDocument->m_Trackes.count();

    qDebug()<< "tt " << tt;

    out << quint16(0) << qsizetype(0);
    for (qsizetype i=0;i<m_pDocument->m_Trackes.count();i++)
    {
      TargetPositionNet tracePos;

      if (m_pDocument->m_Trackes[i].GetCoor(trace_time,tracePos))
      {
          out << tracePos;
      }
      else tt--;
    }
    out.device()->seek(sizeof(quint16));
    out << tt;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pClientSocket->write(arrBlock);

}
