#include "pid.h"
#include "includes.h"

void iPID_Init (iPidObj *pid, long SetPoint,long Proportion,long Integral,long Derivative,long atten)
{
  pid->SetPoint=SetPoint;                                                       //稳态值
  pid->Proportion=Proportion;                                                   //比例参数
  pid->Integral=Integral;                                                       //积分参数
  pid->Derivative=Derivative;                                                   //微分参数
  pid->LastError=0;                                                             //Error[-1]
  pid->SumError=0;                                                              //Sums of Errors
  pid->atten = atten;                                                           //衰减因子
}

void iQPID_Init (iQPidObj *pid,long Proportion,long Integral,long Derivative,long atten)
{
  pid->Proportion=Proportion;                                                   //比例参数
  pid->Integral=Integral;                                                       //积分参数
  pid->Derivative=Derivative;                                                   //微分参数
  pid->LastError=0;                                                             //  Error[-1]
  pid->PrevError=0;                                                             //  Error[-2]
  pid->atten = atten;
}

long iPID_Calc(iPidObj *pid, int NextPoint )
{
    long long dError,Error;
    Error = pid->SetPoint - NextPoint;                                           // 偏差
    pid->SumError += Error;                                                      // 积分
    dError = Error - pid->LastError;                                             // 当前微分
    pid->LastError = Error;
    
    return (pid->Proportion * Error +                                            // 比例项
           pid->Integral * pid->SumError +                                      // 积分项
           pid->Derivative * dError) /                                          // 微分项
          (pid->atten);                                                         // 线性衰减
}

long iQPID_Calc(iQPidObj *pid,long Error)
{
     long long OutDelta;
                                                              //控制量增量
     OutDelta = pid->Proportion * (Error - pid->LastError) +
                pid->Integral * Error +
                pid->Derivative * (Error - 2 * pid->LastError + pid->PrevError);
     pid->PrevError = pid->LastError;                                           //e(k-2)
     pid->LastError = Error;                                                    //e(k-1)
     return OutDelta/pid->atten;                                                //线性衰减
}



