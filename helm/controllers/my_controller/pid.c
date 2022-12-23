/***************(C) COPYRIGHT SWUST_ROBOCON *************BEGIN OF FILE***************/   
/**
  ******************************************************************************
  * @file      pid.c
  * @author    SWUST_ROBOCON
  * @version   V1.0.0
  * @date      2022-10-27
  * @brief     PID�㷨
  ******************************************************************************
  * @attention
  * 
  *
  *
  ******************************************************************************
  */
#include "pid.h"
#include "stdio.h"

/**
  * @brief     PID�ṹ���ʼ������
  * @param     pid	PID�ṹ��ָ��		mode	PIDģʽ		Kp��Ki��Kd	PIDϵ��		max_iout	΢�������ֵ		max_out	PID������ֵ
  * @retval    None
  * @return    None
  * @note      None
  */
void pid_init(PidTypeDef *pid, PID_MODE mode, const float Kp, const float Ki, const float Kd, const float max_iout, const float max_out)
{
	pid->mode = mode;

	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
	
	pid->max_iout = max_iout;
	pid->max_out = max_out;
	
	pid->measure = 0.0f;
	pid->target = 0.0f;
	
	pid->Pout = 0.0f;
	pid->Iout = 0.0f;
	pid->Dout = 0.0f;
	pid->PID_out = 0.0f;
	
	pid->Dbuf[0] = 0.0f;
	pid->Dbuf[1] = 0.0f;
	pid->Dbuf[2] = 0.0f;
	pid->error[0] = 0.0f;
	pid->error[1] = 0.0f;
	pid->error[2] = 0.0f;
}


/**
  * @brief     PID���㺯��
  * @param     pid	PID�ṹ��ָ��		target	Ŀ��ֵ		measure ����ֵ
  * @retval    float
  * @return    PID���ֵ
  * @note      None
  */
float pid_control(PidTypeDef * pid, float target, float measure)
{
	if (pid == NULL)
	{
		return 0.0f;
	}
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];
	pid->measure = measure;
	pid->target = target;
	pid->error[0] = target - measure;
	if (pid->mode == POSITION_MODE)
	{
		pid->Pout = pid->Kp * pid->error[0];
		pid->Iout += pid->Ki * pid->error[0];
		pid->Dbuf[2] = pid->Dbuf[1];
		pid->Dbuf[1] = pid->Dbuf[0];
		pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
		pid->Dout = pid->Kd * pid->Dbuf[0];
		LimitMax(pid->Iout, pid->max_iout);
		pid->PID_out = pid->Pout + pid->Iout + pid->Dout;
		LimitMax(pid->PID_out, pid->max_out);
	}
	else if (pid->mode == INCREMENT_MODE)
	{
		pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
		pid->Iout = pid->Ki * pid->error[0];
		pid->Dbuf[2] = pid->Dbuf[1];
		pid->Dbuf[1] = pid->Dbuf[0];
		pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
		pid->Dout = pid->Kd * pid->Dbuf[0];
		LimitMax(pid->Iout, pid->max_iout);
		pid->PID_out += pid->Pout + pid->Iout + pid->Dout;
		LimitMax(pid->PID_out, pid->max_out);
	}
	return pid->PID_out;
}


/**
  * @brief     ����PID���㺯��
  * @param     pid	�⻷PID�ṹ��ָ��		pid	�ڻ�PID�ṹ��ָ��		pid_outer_target	�⻷Ŀ��ֵ		pid_outer_mersure �⻷����ֵ		pid_inside_mersure �ڻ�����ֵ
  * @retval    float
  * @return    PID���ֵ
  * @note      None
  */
float pid_cascade_control(PidTypeDef  * pid_outer, PidTypeDef * pid_inside, float pid_outer_target, float pid_outer_mersure, float pid_inside_mersure)
{
	if (pid_outer == NULL)
	{
		return 0.0f;
	}
	if (pid_inside == NULL)
	{
		return 0.0f;
	}
	return pid_control(pid_inside, pid_control(pid_outer, pid_outer_target, pid_outer_mersure), pid_inside_mersure);
}


/**
  * @brief     PID�ṹ���������
  * @param     pid	PID�ṹ��ָ��
  * @retval    None
  * @return    None
  * @note      None
  */
void pid_clear(PidTypeDef *pid)
{
  if (pid == NULL)
	{
		return;
	}
	pid->measure = 0.0f;
	pid->target = 0.0f;
	
	pid->Dbuf[0] = 0.0f;
	pid->Dbuf[0] = 0.0f;
	pid->Dbuf[0] = 0.0f;
	pid->error[0] = 0.0f;
	pid->error[0] = 0.0f;
	pid->error[0] = 0.0f;
	
	pid->Pout = 0.0f;
	pid->Iout = 0.0f;
	pid->Dout = 0.0f;
	pid->PID_out = 0.0f;
}

/**
  * @}
  */

/***************(C) COPYRIGHT SWUST_ROBOCON *************END OF FILE********************/   
