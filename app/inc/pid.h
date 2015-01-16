#ifndef _PID_H_
#define _PID_H_

/********λ��ʽPID********/
typedef struct _PID
{
  long  SetPoint;                                                         //  �趨Ŀ�� Desired Value
  long  Proportion;                                                       //  �������� Proportional Const
  long  Integral;                                                         //  ���ֳ��� Integral Const
  long  Derivative;                                                       //  ΢�ֳ��� Derivative Const
  long  LastError;                                                        //  Error[-1]
  long long  SumError;                                                    //  Sums of Errors
  unsigned long atten;                                                    //  ��������pid�����˥������
}iPidObj;
/********����ʽPID********/
typedef struct _QPID
{
  long  Proportion;                                                       //  �������� Proportional Const
  long  Integral;                                                         //  ���ֳ��� Integral Const
  long  Derivative;                                                       //  ΢�ֳ��� Derivative Const
  float  LastError;                                                        //  Error[-1]
  float  PrevError;                                                        //  Error[-2]
  unsigned long atten;                                                    //  ��������pid�����˥������
}iQPidObj;
/***********************************************�����ⲿ����*********************************************/

extern void iPID_Init (iPidObj *pid, long SetPoint,long Proportion,long Integral,long Derivative,long atten);
extern void iQPID_Init (iQPidObj *pid,long Proportion,long Integral,long Derivative,long atten);
extern long iPID_Calc(iPidObj *pid, int NextPoint );
extern long iQPID_Calc(iQPidObj *pid,long Error);

#endif
