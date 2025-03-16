#pragma once
// controller.h
#ifndef CONTROLLER_H
#define CONTROLLER_H




// �˶�����ָ���� ---------------------------------------------------

/**
 * @brief ������Բ����ת��ָ����λ
 * @param diskNumber Ŀ���λ��ţ���Ч��Χ��1-8��
 * @return �μ�sendorderandcheckrsp����ֵ
 */
int controlTopDiskRotation(int diskNumber);

/**
 * @brief ������Բ����ת��ָ����λ
 * @param diskNumber Ŀ���λ��ţ���Ч��Χ��1-8��
 * @return �μ�sendorderandcheckrsp����ֵ
 */
int controlBottomDiskRotation(int diskNumber);

// ״̬��ȡָ���� ---------------------------------------------------

/**
 * @brief ��ȡ��Բ��״̬��Ϣ
 * @param order ��ѯָ�����ͣ�
 *              - 02: ʵʱλ��
 *              - 03: Ŀ��λ��
 *              - 04: ��λ��ɱ�־
 *              - 05: ���е���
 * @param getRspData ����������������״̬����
 * @return �μ�sendorderandrspans����ֵ
 */
int readTopDiskInfo(int order, long long* getRspData);

/**
 * @brief ��ȡ��Բ��״̬��Ϣ������ͬ��Բ�̣�
 */
int readBottomDiskInfo(int order, long long* getRspData);

/**
 * @brief ��ȡ�۽�����״̬��Ϣ������ͬ��Բ�̣�
 */
int readfocusInfo(int order, long long* getRspData);

/**
 * @brief ��ȡ��Ȧ����״̬��Ϣ������ͬ��Բ�̣�
 */
int readapertureInfo(int order, long long* getRspData);

// �����˶�����ָ���� ------------------------------------------------

/**
 * @brief ���ƹ�Ȧλ��
 * @param value Ŀ��λ�ã���Ч��Χ��0-1500000��
 * @return �μ�sendorderandcheckrsp����ֵ
 */
int controlapertureRotation(long long value);

/**
 * @brief ���ƾ۽�λ��
 * @param value Ŀ��λ�ã���Ч��Χ��0-1500000��
 * @return �μ�sendorderandcheckrsp����ֵ
 */
int controlfocusRotation(long long value);

#endif // CONTROLLER_H
