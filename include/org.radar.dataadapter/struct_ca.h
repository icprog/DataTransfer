#ifndef struct_ca_H_
#define struct_ca_H_


#define RGates_CA  800				//c BAND �����ʾ������
#define VGates_CA  1600				//c BAND �ٶȾ������
#define WGates_CA  1600				//c BAND �׿�������

typedef struct _tagBaseDataCA
{
    unsigned short		temp1[7];						//����
    unsigned short		RadarStatus;					//1 - ��ʾΪ�״�����
    unsigned short		temp2[6];						//����
    unsigned int		mSeconds;						//���������ռ�ʱ��
    unsigned short		JulianDate;						//��1970/1/1�������
    unsigned short		URange;							//��ģ������
    unsigned short		Az;								//��λ�Ƕ�
    unsigned short		RadialNumber;					//�����������
    unsigned short		RadialStatus;					//��������״̬
    unsigned short		El;								//����
    unsigned short		ElNumber;						//��ɨ�ڵ����Ǳ��
    short				RangeToFirstGateOfRef;			//��һ�����������ݱ�ʾ��ʵ�ʾ���(m)
    short				RangeToFirstGateOfDop;			//��һ�����������ݱ�ʾ��ʵ�ʾ���(m)
    unsigned short		GateSizeOfReflectivity;			//���������ݵľ���ⳤ(m)
    unsigned short		GateSizeOfDoppler;				//���������ݵľ���ⳤ(m)
    unsigned short		GatesNumberOfReflectivity;		//���������ݵľ������
    unsigned short		GatesNumberOfDoppler;			//���������ݵľ������
    unsigned short		CutSectorNumber; 				//������
    unsigned int		CalibrationConst; 				//�궨����
    unsigned short		PtrOfReflectivity;				//����������ָ��
    unsigned short		PtrOfVelocity;					//�ٶ�����ָ��
    unsigned short		PtrOfSpectrumWidth;				//�׿�����ָ��
    unsigned short		ResolutionOfVelocity;			//�������ٶȷֱ���
    unsigned short		VcpNumber;						//��ɨ��
    unsigned short		temp4[4];						//����
    unsigned short		PtrOfArcReflectivity;			//����������ָ��
    unsigned short		PtrOfArcVelocity;				//�ٶ�����ָ��
    unsigned short		PtrOfArcWidth;					//�׿�����ָ��
    unsigned short		Nyquist;						//��ģ���ٶ�
    unsigned short      temp46;                         //����
    unsigned short      temp47;                         //����
    unsigned short      temp48;                         //����
    unsigned short      CircleTotal;					//������
    char				SiteCode[8];					//�״�վ����
    unsigned char		temp5[22];						//����
    unsigned char		Echodata[RGates_CA+VGates_CA+WGates_CA];	//129��588 ��460�ֽڷ���������
    unsigned char		temp[4];						//����
    //129��1508 ��1380�ֽ��ٶ�����
    //129��2428 ��2300�ֽ��׿�����
} CaRadialData;

#endif
