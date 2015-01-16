#include "pid.h"
#include "includes.h"

void iPID_Init (iPidObj *pid, long SetPoint,long Proportion,long Integral,long Derivative,long atten)
{
  pid->SetPoint=SetPoint;                                                       //��ֵ̬
  pid->Proportion=Proportion;                                                   //��������
  pid->Integral=Integral;                                                       //���ֲ���
  pid->Derivative=Derivative;                                                   //΢�ֲ���
  pid->LastError=0;                                                             //Error[-1]
  pid->SumError=0;                                                              //Sums of Errors
  pid->atten = atten;                                                           //˥������
}

void iQPID_Init (iQPidObj *pid,long Proportion,long Integral,long Derivative,long atten)
{
  pid->Proportion=Proportion;                                                   //��������
  pid->Integral=Integral;                                                       //���ֲ���
  pid->Derivative=Derivative;                                                   //΢�ֲ���
  pid->LastError=0;                                                             //  Error[-1]
  pid->PrevError=0;                                                             //  Error[-2]
  pid->atten = atten;
}

long iPID_Calc(iPidObj *pid, int NextPoint )
{
    long long dError,Error;
    Error = pid->SetPoint - NextPoint;                                           // ƫ��
    pid->SumError += Error;                                                      // ����
    dError = Error - pid->LastError;                                             // ��ǰ΢��
    pid->LastError = Error;
    
    return (pid->Proportion * Error +                                            // ������
           pid->Integral * pid->SumError +                                      // ������
           pid->Derivative * dError) /                                          // ΢����
          (pid->atten);                                                         // ����˥��
}

long iQPID_Calc(iQPidObj *pid,long Error)
{
     long long OutDelta;
                                                              //����������
     OutDelta = pid->Proportion * (Error - pid->LastError) +
                pid->Integral * Error +
                pid->Derivative * (Error - 2 * pid->LastError + pid->PrevError);
     pid->PrevError = pid->LastError;                                           //e(k-2)
     pid->LastError = Error;                                                    //e(k-1)
     return OutDelta/pid->atten;                                                //����˥��
}



