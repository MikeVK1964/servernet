#include <QMessageBox>
#include <QMessageBox>
#include "DocTrace.h"
//____________________________________
DocTras::DocTras()
{
//    m_pTrackes = new QVector<Tras>; // вектор трасс

}

void DocTras::AddTras(float B, float D, int H, kmb_trace::TargetType type_tar1)
{

 Tras tr(B,D,H,type_tar1);
 m_Trackes.push_back(tr);

}
//*********************************************
// Возвращает номер трассы
// -1 - если не найдена
//*********************************************
// num_point = 1 - поиск любой точки
// num_point = 0 - поиск последней точки
// db - дельта пеленга dd - дельта дальности
int DocTras::FindTras (int cur_tras,float B, float D
 , float db,float dd,int& num_point)
{
 int i;
 // Узнаем размер контейнера точек
 int ntrace = m_Trackes.size();
 // Уходим , если он пуст
 int last_el;
 if (ntrace==0) // Если нет трасс уходим
        return -1;


 int j;
 if (cur_tras!=-1)  // если не стоим на новой трассе
 {
    last_el=m_Trackes[cur_tras].m_TrasPoint.size();
   if (num_point==0)
   {
    if ( fabs(m_Trackes[cur_tras].m_TrasPoint[last_el-1].D
     -D)<dd &&
    fabs(m_Trackes[cur_tras].m_TrasPoint[last_el-1].B -B)<db)
    return cur_tras;
   }
   else
   {
    for (j=0;j<last_el;j++)
    {
     if ( fabs(m_Trackes[cur_tras].m_TrasPoint[j].D
      -D)<dd &&
     fabs(m_Trackes[cur_tras].m_TrasPoint[j].B -B)<db)
     {
         num_point=j;
         return cur_tras;
     }
    }
   }
 }
  for ( i=0;i<ntrace;i++)
  {
   last_el=m_Trackes[i].m_TrasPoint.size();
   if (num_point==0)
   {
    if ( fabs(m_Trackes[i].m_TrasPoint[last_el-1].D
     -D)<dd &&
    fabs(m_Trackes[i].m_TrasPoint[last_el-1].B -B)<db)
    return i;
   }
   else
   {
    for (j=0;j<last_el;j++)
    {
     if ( fabs(m_Trackes[i].m_TrasPoint[j].D
      -D)<dd &&
     fabs(m_Trackes[i].m_TrasPoint[j].B -B)<db)
     {
         num_point=j;
         return i;
     }
    }
   }
  }
 return -1;

}
// удалить все трассы
void DocTras::DeleteAllTras()
{
 m_Trackes.clear();

}
// Сохранение документа в файле
// QString fname  ---  имя файла с путем
void DocTras::Save(QString fname)
{
    QFile f(fname);
    if (f.open(QIODevice::WriteOnly ))
    {
     char head_f[]="SAL02";
     f.write(head_f,5); // сохранили заголовок
   //  New_traceView* pView = dynamic_cast<New_traceView*> (centralWidget());
   //  DocTras* pdoc= pView->GetDocument();
     // открываем поток для записи
     QDataStream stream(&f);
     stream.setVersion(QDataStream::Qt_5_10);
/////     SetStartTime();    --- реализовать позже

     stream << m_Trackes.count(); // число  трасс

     for (qsizetype i=0;i<m_Trackes.count();i++)
             stream << m_Trackes[i];
  //    m_Trackes[i].Serialize(ar);


     f.flush();
     f.close();
    }
    else
      qDebug("error open file\n");

}
void DocTras::Load(QString fname)
{
    QFile file(fname);
   // file.setFileName(str);
    if (!file.open(QIODevice::ReadOnly))
    {
      QMessageBox::warning(nullptr,tr("Ошибка открытия  "),
                           tr("файл %1\n%2 ").arg(file.fileName()).arg(file.errorString()));

      return ;
    }

    QDataStream stream(&file);
  //  stream.setDevice((QIODevice *)&file);


    stream.setVersion(QDataStream::Qt_5_10);
    char head_f[5];
    if (stream.readRawData((char*)&head_f,5)==-1)
    {
       QMessageBox::about(nullptr,tr(""),tr("Слишком мало байт в файле"));
       file.close();
       return ;
    }
    if (strncasecmp("SAL02",head_f,5)!=0 )
    {
        QMessageBox::warning(nullptr,tr("Ошибка  "),tr("Неправильный заголовок файла  "));
        file.close();

        return;
    }

    qsizetype cntTrace; // число трасс
    stream >> cntTrace;
    DeleteAllTras();   // очистка вектора трасс

    Tras tr;
    for (qsizetype i=0;i<cntTrace;i++)
    {
     m_Trackes.push_back(tr);
     //m_Trackes[i].Serialize(ar);
     stream >> m_Trackes[i];
     int rc=m_Trackes[i].CheckTras();
     if (rc!=0)
         QMessageBox::about(0,"error","");
    }


}

void DocTras::SetStartTime()
{
    int i,j;
    double t_max=0;
    for (i=0;i< m_Trackes.size() ;i++)
    {
     m_Trackes[i].start_time = 0;
     if (m_Trackes[i].m_TrasPoint.size()==1)
         continue;
     for (j=1;j<= m_Trackes[i].NumTR ; j++)
     {
        float DT;
        float VX0,VY0,VZ0;
        m_Trackes[i].CalcV(j,VX0,VY0,VZ0,DT);

       m_Trackes[i].start_time -= DT;
     }
     if (m_Trackes[i].start_time<t_max) t_max=m_Trackes[i].start_time;
    }
    for (i=0;i<m_Trackes.size();i++)
     m_Trackes[i].start_time -= t_max;

}
