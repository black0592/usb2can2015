#ifndef _PID_H_
#define _PID_H_

/********位置式PID********/
typedef struct _PID
{
  long  SetPoint;                                                         //  设定目标 Desired Value
  long  Proportion;                                                       //  比例常数 Proportional Const
  long  Integral;                                                         //  积分常数 Integral Const
  long  Derivative;                                                       //  微分常数 Derivative Const
  long  LastError;                                                        //  Error[-1]
  long long  SumError;                                                    //  Sums of Errors
  unsigned long atten;                                                    //  用于整型pid运算的衰减常数
}iPidObj;
/********增量式PID********/
typedef struct _QPID
{
  long  Proportion;                                                       //  比例常数 Proportional Const
  long  Integral;                                                         //  积分常数 Integral Const
  long  Derivative;                                                       //  微分常数 Derivative Const
  float  LastError;                                                        //  Error[-1]
  float  PrevError;                                                        //  Error[-2]
  unsigned long atten;                                                    //  用于整型pid运算的衰减常数
}iQPidObj;
/***********************************************声明外部函数*********************************************/

extern void iPID_Init (iPidObj *pid, long SetPoint,long Proportion,long Integral,long Derivative,long atten);
extern void iQPID_Init (iQPidObj *pid,long Proportion,long Integral,long Derivative,long atten);
extern long iPID_Calc(iPidObj *pid, int NextPoint );
extern long iQPID_Calc(iQPidObj *pid,long Error);

#endif
