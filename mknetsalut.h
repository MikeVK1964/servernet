#ifndef MKNETSERVER_H
#define MKNETSERVER_H

#include <QTcpServer>
#include <QObject>

class DocTras;
class MKNetServer : public QObject
{
    Q_OBJECT
public:
    explicit MKNetServer(int nPort,DocTras* ppDocTras,QWidget* pwgt = 0);
    void SetpDoc(DocTras* m_pDocument){this->m_pDocument=m_pDocument;}
public slots:
    virtual void slotNewConnection();


    void slotReadClient   ();

private:
    QTcpSocket* pClientSocket = nullptr;
    QTcpServer* m_ptcpServer;
    DocTras* m_pDocument;

    void MkSendToClient(double trace_time);

};

#endif // MKNETSERVER_H
