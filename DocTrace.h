#ifndef DOCTRACE_H
#define DOCTRACE_H
#include "maketras.h"


class DocTras :public QObject
{
public:
 DocTras();
 void AddTras(float B, float D, int H, kmb_trace::TargetType type_tar1);
 QVector<Tras> m_Trackes; // вектор трасс
 int FindTras (int cur_tras,float B, float D
  , float db,float dd,int& num_point);
 void DeleteAllTras();
 void Save(QString fname);
 void Load(QString fname);
 void SetStartTime();

/// int GetNumTras(double t);
};


#endif // DOCTRACE_H
