/**
  * @file   transferSet.h
  * @brief  ���ݴ����������л�.
  *
  *  ��������ϸ˵����
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:05:16
  * @note   ������ע�����
  * @version <�汾��>
  */

#ifndef transferSet_H_
#define transferSet_H_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"
#include "fileMsg.h"


// �ַ�����
class SendSet
{
public:
    SendSet()
    {
        bEncrypt=false;
        bCompress=false;
        bKeepDir=true;
        bConSend = false;
        nSendWay = 0;
        nPort = 21;
    }

    string strUsrname;		//�û���ʶ��Ҫ��Ψһ��
    string strPath;			//���Ŀ¼��URL
    string strRltvPath;		//���·��
    vector<string> vecRegs;	//ƥ�����
    bool bEncrypt;			//true��ʾ���ܣ�false��ʾ������
    bool bCompress;			//true��ʾѹ����false��ʾ��ѹ��
    bool bKeepDir;			//true��ʾ����ԭʼĿ¼�ṹ��false��ʾ������
    int nSendWay;			//�ַ���ʽ��0-Ŀ¼��1-FTP��
    bool bConSend;			//true��ʾ�ϵ�����

    string strUsr;			//�û���������Ŀ¼��ʽ����Ҳ��Ҫ��
    string strPwd;			//����
    int	   nPort;			//�˿ں�
    string strIP;			//IP
    //--TCP--��������
};

class SendSetList
{
public:
    list<SendSet> lsts;
};

DLL_EXPORT_C_DECL bool SendSetListToXml(const string &strPath, const SendSetList &info);
DLL_EXPORT_C_DECL bool XmlToSendSetList(const string &strPath, SendSetList &info);

/**
  * @class <TransferSet>
  * @brief  ���ݴ����������л�
  *
  *  �����������ϸ����
  * @note	������ע������
  * @see 	�����ǲο�����
 */
class TransferSet
{
public:
    string strName;				//Ŀ¼��ʶ��Ҫ��Ψһ��
    //�ռ�
    string strPath;				//���Ŀ¼��URL
    int nCollectWay;			//�ռ���ʽ��0-Ŀ¼��1-FTP��
    vector<string> vecRegs;		//ƥ�����
    int nWatchWay;				//Ŀ¼��ط�ʽ��0-��ʱ��1-ʵʱ��
    string strDispatch;			//���ȹ���
    //--FTP--
    string strUsr;				//�û���
    string strPwd;				//����
    int	   nPort;				//�˿ں�
    string strIP;				//IP
    vector<string> vecRecvID;	//�û�ID
    bool	bEnable;			//�Ƿ�����
    //////////////////////////////////////////////////////////////////////////
    // ���²������л�
    vector<SendSet> vecRcvers;	//�ַ��û�
    FileInfoList fileList;		//��Ҫ������ļ��б�
    int  sendFlag;				//�����ʶ
    //----

    TransferSet()
    {
        sendFlag = 0;
        bEnable = true;
    }
};

class TransferSetList
{
public:
    list<TransferSet> lsSets;
};

DLL_EXPORT_DECL bool TransferSetToXml(const string &strPath, const TransferSetList &info);
DLL_EXPORT_DECL bool XmlToTransferSet(const string &strPath, TransferSetList &info);


//////////////////////////////////////////////////////////////////////////
/**
  * @class <TransCollectTime>
  * @brief  ���ݴ����ռ�ʱ��
  *
  *  �����������ϸ����
  * @note	������ע������
  * @see 	�����ǲο�����
 */
class TransCollectTime
{
public:
    std::string strDirID;		//Ŀ¼ID��Ψһ��
    std::map<std::string, uint> mapDirTime;	//·��-����޸�ʱ��
    //std::map<std::string, std::string> mapDirTime;

    //����==����������ֱ��remove
    bool operator==(const TransCollectTime& time)
    {
        if ( this->strDirID == time.strDirID )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class TransCollectTimeList
{
public:
    TransCollectTimeList()
    {
        lsts.clear();
    }
    std::list<TransCollectTime> lsts;
};

DLL_EXPORT_C_DECL bool TransCollectTimeToXml(const std::string &strPath, const TransCollectTimeList &lst);
DLL_EXPORT_C_DECL bool XmlToTransCollectTime(const std::string &strPath, TransCollectTimeList &lst);

#endif