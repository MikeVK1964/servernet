#ifndef MAKETRAS_H
#define MAKETRAS_H
#include <QtCore>

namespace kmb_trace {
 enum TargetType:int{
  Unknown=0,
  Friend,
  Foe,
  Our_Hel

 };

}
class QPainter;

// класс текущего положения цели
// используется для передачи по сети
class TargetPositionNet {
 public:
    float  fx_km;
    float  fy_km;
    int ih_m;
    friend QDataStream &operator <<(QDataStream &stream, const TargetPositionNet &A){
     stream << A.fx_km << A.fy_km << A.ih_m;
     return stream;
    }
    friend QDataStream & operator>> (QDataStream& stream, TargetPositionNet& A){
        stream >> A.fx_km >> A.fy_km >> A.ih_m ;
        return stream;

    }

};

// Класс дейcтвительных точек трассы

class RealTT {
public:
 RealTT(){}
 virtual ~RealTT(){}
 float fvx,fvy,fvz;
 float fax,fay,faz;
 double time_move;

};

class TrasPoint
{
public:
 TrasPoint(){}

 TrasPoint(float B1,float D1,int H,unsigned int A=0,unsigned int V=0);
 float B;
 float D;
 int H;   // метры
 unsigned int A; // ускорение на маневре (м/с*с)
 unsigned int V; // скорость
 friend QDataStream &operator <<(QDataStream &stream, const TrasPoint &A){
  stream << A.B << A.D << A.H << A.A << A.V;
  return stream;
 }
 friend QDataStream & operator>> (QDataStream& stream, TrasPoint& A){
     stream >> A.B >> A.D >> A.H >> A.A >> A.V;
     return stream;

 }
};

class Tras
{
public:
 Tras() {}
 int AddTP(float B,float D,unsigned int H1,
        unsigned int A=1,unsigned int V=1);
 Tras(float B,float D,int Z1,
      kmb_trace::TargetType type_tar1=kmb_trace::Unknown);
 void Draw(QPainter& painter,QSize sz_pix,float dist);
 void DrawH(QPainter& painter,QSize sz_pix,float dist, int max_h_km);
// void ShowTrPos(QPainter& painter,QSize sz_pix,float dist,double time_c);
 void ShowTrPosSI(QPainter& painter,QSize sz_pix,float dist,int max_h_km,double time_c);

 QVector<TrasPoint> m_TrasPoint; // массив точек трасс
 kmb_trace::TargetType type_tar;
 int NumTR; // номер точки рубежа
 int CheckTras();
 double start_time=0.0;  //  begining time since imitation
 void CalcV(int i, float &VX, float &VY,float &VZ,float& DT);
 bool GetCurrentCoor_IKO(float& x_km,float& y_km,double trace_time);
 bool GetCurrentCoor_SI(int& iz_m,float& x_km,double trace_time);
 inline bool GetCoor( double tick_c,float& fx,float& fy,int& iz,
     float fvx1,float fvy1,float fvz1, float fax1,float fay1,float faz1 );
 bool GetCoor(double tick_c,TargetPositionNet& tpn);
private:
 void BDToXY(double B, double D, double &x, double &y);

 QVector<RealTT> m_RealTT; // вектор действительных точек трассы

 friend QDataStream &operator <<(QDataStream &stream, const Tras &A){
     stream << A.type_tar<< A.NumTR ;//<< A.m_TrasPoint.count();// << start_time;
     stream << A.m_TrasPoint;
//     for (qsizetype i=0;i<A.m_TrasPoint.count();i++)
//       stream << A.m_TrasPoint[i];
     return stream;
 };
//data_stream >> reinterpret_cast<QSet<qint32>&>(color_set2);
 friend QDataStream & operator>> (QDataStream& stream, Tras& A){
 //    qsizetype num_point_of_trace;
     stream >> reinterpret_cast<int&>(A.type_tar) >> A.NumTR; // >> num_point_of_trace;
     stream >> A.m_TrasPoint;

     return stream;
 }

};


#endif // MAKETRAS_H
