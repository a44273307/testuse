#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#ifdef _WIN32
#ifdef CONTROLLER_EXPORTS
#define CONTROLLER_API __declspec(dllexport)
#else
#define CONTROLLER_API __declspec(dllimport)
#endif
#else
#define CONTROLLER_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // �˶�����ָ���� ---------------------------------------------------

    /**
     * @brief ������Բ����ת��ָ����λ
     * @param diskNumber Ŀ���λ��ţ���Ч��Χ��1-8��
     * @return �μ�sendorderandcheckrsp����ֵ
     */
    CONTROLLER_API int controlTopDiskRotation(int diskNumber);

    /**
     * @brief ������Բ����ת��ָ����λ
     * @param diskNumber Ŀ���λ��ţ���Ч��Χ��1-8��
     * @return �μ�sendorderandcheckrsp����ֵ
     */
    CONTROLLER_API int controlBottomDiskRotation(int diskNumber);

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
    CONTROLLER_API int readTopDiskInfo(int order, long long* getRspData);

    /**
     * @brief ��ȡ��Բ��״̬��Ϣ������ͬ��Բ�̣�
     */
    CONTROLLER_API int readBottomDiskInfo(int order, long long* getRspData);

    /**
     * @brief ��ȡ�۽�����״̬��Ϣ������ͬ��Բ�̣�
     */
    CONTROLLER_API int readfocusInfo(int order, long long* getRspData);

    /**
     * @brief ��ȡ��Ȧ����״̬��Ϣ������ͬ��Բ�̣�
     */
    CONTROLLER_API int readapertureInfo(int order, long long* getRspData);

    // �����˶�����ָ���� ------------------------------------------------

    /**
     * @brief ���ƹ�Ȧλ��
     * @param value Ŀ��λ�ã���Ч��Χ��0-1500000��
     * @return �μ�sendorderandcheckrsp����ֵ
     */
    CONTROLLER_API int controlapertureRotation(long long value);

    /**
     * @brief ���ƾ۽�λ��
     * @param value Ŀ��λ�ã���Ч��Χ��0-1500000��
     * @return �μ�sendorderandcheckrsp����ֵ
     */
    CONTROLLER_API int controlfocusRotation(long long value);

    // ���Ժ���
    CONTROLLER_API int testa_add(int a,int b);

#ifdef __cplusplus
}
#endif

#endif // CONTROLLER_H
