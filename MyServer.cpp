/* ======================================================================
**  MyServer.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtNetwork>
#include <QtGui>
#include <qmessagebox.h>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <qfiledialog.h>
#include "MyServer.h"
#include "DocTrace.h"

// ----------------------------------------------------------------------
MyServerWidget::MyServerWidget(int nPort ,QWidget* pwgt /*=0*/) : QWidget(pwgt)
                                                    , m_nNextBlockSize(0)
{


    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    QPushButton* pcmd = new QPushButton("&Load");
    connect(pcmd, SIGNAL(clicked()), SLOT(Load()));


    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;    
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    pvbxLayout->addWidget(pcmd);
    setLayout(pvbxLayout);
    m_pDocument = new DocTras;
    m_pmkServer = new MKNetServer(nPort,m_pDocument,this);

}

// ----------------------------------------------------------------------
/*virtual*/ void MyServerWidget::slotNewConnection()
{
    // nextPendingConnection() -- возвратит предстоящий сокет
    pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );
    qDebug() << "New Connetion";
//////    sendToClient(pClientSocket, "Server Response: Connected!");
}

// ----------------------------------------------------------------------
void MyServerWidget::slotReadClient()
{
    pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_10);
    double trace_time;
    in >> trace_time;
    qDebug()<< "trace_time1 " << trace_time;
/////    sendToClient(pClientSocket, "Server Response: trace_time has received!");
    MkSendToClient(trace_time);
}

// ----------------------------------------------------------------------

//    double trace_time; // время движения с начала имитации в секундах

void MyServerWidget::MkSendToClient(double trace_time)
{
//    QString str="1234";
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);


    qsizetype tt=m_pDocument->m_Trackes.count();
    out << quint16(0) << qsizetype(0);
    for (qsizetype i=0;i<m_pDocument->m_Trackes.count();i++)
    {
      TargetPositionNet tracePos;

 //     float fvx1, fvy1, fvz1, fax1, fay1, faz1;
 //     if (m_pDocument->m_Trackes[i].GetCoor(trace_time,tracePos.fx_km,tracePos.fy_km,tracePos.ih_m,
 //       fvx1, fvy1, fvz1, fax1, fay1, faz1  ))
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
void MyServerWidget::Load()
{
    QString str= QFileDialog::getOpenFileName(this,"Open files","","*.trc");//После закрытия диалогового окна мы проверяем
//    QString str= QFileDialog::getOpenFileName(this, "Open File",
//                                                    "/home","*.trc");
//                                                    //"Images (*.png " ));
    if (str.isEmpty())  return;
    m_pDocument->Load(str);
    m_pmkServer->SetpDoc(m_pDocument);
}
