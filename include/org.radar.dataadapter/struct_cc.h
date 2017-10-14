/**
 * struct_cc.h
 * @Author: Fanghua Guo
 * @CreateDate: 2014-5-5
 * @Version 1.0
 * Copyright (C) 2014 NRIET
 */
#ifndef STRUCT_CC_H
#define STRUCT_CC_H
/*CINRAD/CC�������״���ɨ������5����֮��9�����������9��ɨ�裬
ÿһ�����������Ʒ���ز�ǿ�ȣ�Z���������ٶȣ�V�����ٶ��׿�(W)��
�״�ÿһ��ɨ�����512������ÿһ������洢500��Z��500��V��500��W��
���һ����ɨ�����ܹ�����27��ͼ�㡣
�ļ�����
ƽɨ��PPI����NNNNYYDDHH.MMP
��ɨ��RHI����NNNNYYDDHH.MMR
��ɨ��VPPI����NNNNYYDDHH.MMV
����ƣƣԣ�NNNNYYDDHH.MMF
�ȸ�PPI��CAPPI����NNNNYYDDHH.MMC
N����   Y����    D����    H��ʱ   M����
�磺2013051416.26V
������ɣ�
�����������ļ�ͷ(1��)�ͻ��ڼ�����ϵ��ԭʼ����(512������)��ɡ�
21505024�ֽ� = 1024�ֽ� + 14*512*1500*2�ֽ�
*/

//�ļ�ͷ��
//tagWEATHERRADAR�״���Ϣ�Ľṹ��1024���ֽ�
typedef struct tagWEATHERRADAR
{
    char FileType[16];						 //3830���ݱ�ʶ(CINRADC)
    char Country[30];						//������
    char Province[20];					        //ʡ��
    char Station[40];                           //վ��
    char StationNumber[10];                               //��վ��
    char RadarType[20];                                       //�״��ͺ�
    char Longitude[16];					       //�������ھ���
    char Latitude[16];					       //��������γ��
    long LongitudeValue;				      //���徭��
    long LatitudeValue;					       //����γ��
    long Height;						      //���ߺ��θ߶�
    short MaxAngle;					     //�����赲�������
    short OptAngle;                                             //��ѹ۲�����
    unsigned char SYear1;				      //�۲⿪ʼʱ�����ǧ��λ(19-20)
    unsigned char SYear2;				      //�۲⿪ʼʱ�����ʮ��λ(00-99)
    unsigned char SMonth;                                   //�۲⿪ʼʱ�����(1-12)
    unsigned char SDay;                                         //�۲⿪ʼʱ�����(1-31)
    unsigned char SHour;                                      //�۲⿪ʼʱ���ʱ(0-23)
    unsigned char SMinute;                                  //�۲⿪ʼʱ��ķ�(0-59)
    unsigned char SSecond;                                  //�۲⿪ʼʱ�����(0-59)
    unsigned char TimeFrom;                             //ʱ����Դ 0-�����ʱ��(1����δ��ʱ)   1-�����ʱ��(1�����Ѷ�ʱ)   2-GPS   3-����
    unsigned char EYear1;				      //�۲����ʱ�����ǧ��λ(19-20)
    unsigned char EYear2;                                   //�۲����ʱ�����ʮ��λ(00-99)
    unsigned char EMonth;                                 //�۲����ʱ�����(1-12)
    unsigned char EDay;                                      //�۲����ʱ�����(1-31)
    unsigned char EHour;				   //�۲����ʱ���ʱ(0-23)
    unsigned char EMinute;				   //�۲����ʱ��ķ�(0-59)
    unsigned char ESecond;				   //�۲����ʱ�����(0-59)
    unsigned char ScanMode;			  //ɨ�跽ʽ  1-RHI   10-PPI��ZPPI   1XX=VPPI(XXΪɨ��Ȧ��)
    unsigned long SmilliSecond;			  //��΢��Ϊ��λ��ʾ�����С��λ
    unsigned short RHIA;				   //RHI���ڵķ�λ��(0.01��Ϊ��λ)��PPI��VPPIʱΪFFFF
    short RHIL;                                                   //RHI���ڵ��������(0.01��Ϊ��λ)��PPI��VPPIʱΪFFFF
    short RHIH;                                                  //RHI���ڵ��������(0.01��Ϊ��λ)��PPI��VPPIʱΪFFFF
    unsigned short EchoType;                        //�ز�����  0x405a-Z  0x406a-V  0x407a-W�� 0x408a-ZVW��Ҫ��
    unsigned short ProdCode;                       //��������  0x8001-PPI����  0x8002-RHI���ݣ�0x8003-VPPI����
    //0x8004-��ǿ��PPI���ݣ�0x8005-CAPPI����
    unsigned char Calibration;			//��У״̬  0-��  1-�Զ�  2-1�������˹�   3-1�����˹�
    unsigned char remain1[3];			//������
    unsigned char remain2[660];             //������,��VPPISCANPARAMETER���ݣ��ýṹ��˵������������218���ֽڣ�22(�ֽ�/��)*30(��) = 660(�ֽ�)
    long AntennaG;                                            //��������(0.001dB)
    long Power;                                                   //��ֵ����(��)
    long Wavelength;					//����(΢��)
    unsigned short BeamH;			       //��ֱ�������(��)
    unsigned short BeamL;                            //ˮƽ�������(��)
    unsigned short Polarization;		      //����״̬ 0-ˮƽ 1-��ֱ 2-˫ƫ��  3-Բƫ�� 4-����
    unsigned short LogA;			      //������̬��Χ(0.01dB)
    unsigned short LineA;			      //���Զ�̬��Χ(0.01dB)
    unsigned short AGCP;			      //AGC�ӳ���(΢��)
    unsigned short FreqMode;		     //Ƶ�ʷ�ʽ	1-���ظ�Ƶ��  2-˫�ظ�Ƶ��3:2   3-˫�ظ�Ƶ��4:3
    unsigned short FreqRepeat;		     //�ظ�Ƶ��
    unsigned short PPPPulse;	            //PPP������
    unsigned short FFTPoint;                    //FFT�������
    unsigned short ProcessType;            //�źŴ���ʽ	1-PPP	2-ȫ��FFT   3-����FFT
    unsigned char  ClutterT;			    //�Ӳ�������ֵ(��STC)
    char Sidelobe;                                           //��һ�԰�(dB)
    unsigned char VelocityT;			   //�ٶ�����
    unsigned char FilderP;			   //����������ʽ   0-��   1-IIR�˲���1    2-IIR�˲���2   3-IIR�˲���3   4-IIR�˲���4
    unsigned char NoiseT;		          //����������ֵ(��ǿ������)
    unsigned char SQIT;				  //SQI����
    unsigned char IntensityC;                   //DVIPǿ��ֵ������õ�ͨ���� 1-����ͨ�� 2-����ͨ��
    unsigned char IntensityR;	         //ǿ��ֵ�����Ƿ���붩���� 0-��(dB) 1-�Ѷ���(dBZ)
    unsigned char CalNoise;			 //����ϵ���궨ֵ
    unsigned char CalPower;			 //���书�ʱ궨ֵ
    unsigned char CalPulseWidth;         //�����ȱ궨ֵ
    unsigned char CalWorkFreq;		//����Ƶ�ʱ궨ֵ
    unsigned char CalLog;			//����б�ʱ궨ֵ
    char Remain3[92];				//������
    unsigned long int DataOffset;	//����ƫ�Ƶ�ַ
} WEATHERRADAR;

//tagVPPISCANPARAMETER���ǲ�Ľṹ��22���ֽڡ� VPPI�м��������PPI  RHI  FFT����ֻ��дһ�㼴һ����
typedef struct tagVPPISCANPARAMETER
{
    unsigned short MaxV;			//���ɲ��ٶ�(����/��)
    unsigned short MaxL;			//���ɲ����(10��)
    unsigned short BindWidth;		//�ⳤ(��)
    unsigned short BinNumber;            //ÿ�������
    unsigned short RecordNumber;     //��Ȧ������
    unsigned short Arotate;                   //��Ȧת��(0.01��/��)
    unsigned short Prf1;                        //��Ȧ��һ���ظ�Ƶ��(0.1Hz)��Ӧ����Ƶ��˫��Ƶ�ĸ���
    unsigned short Prf2;                        //��Ȧ�ڶ����ظ�Ƶ��(0.1Hz)��Ӧ˫��Ƶ�ĵ���
    unsigned short SpulseW;                 //��Ȧ����(΢��)
    short                 Angle;                       //����(0.01��)
    unsigned char  SweepStatus;		//1=��Ҫ��	2=��Ҫ��(����Ƶ)	3=��Ҫ��(˫��Ƶ)
    unsigned char  Ambiguousp;		//0=�������ģ��	1=�����ģ��
} VPPISCANPARAMETER;

#endif // STRUCT_CC_H
