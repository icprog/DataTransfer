/**
  * @file   prdcTransferSet.h
  * @brief  ��Ʒ�ַ�����.
  *
  *  ��������ϸ˵����
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:08:01
  * @note   ������ע�����
  * @version <�汾��>
  */

#ifndef prdcTransferSet_H_
#define prdcTransferSet_H_


#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"
#include "fileMsg.h"


/**
  * @class <PrdcTransferSet>
  * @brief  ��Ʒ�ַ��������л�
  *
  *  �����������ϸ����
  * @note	������ע������
 */
class PrdcTransferSet
{
public:
    string	strID;				//�����ʶ��Ҫ��Ψһ��
    bool	bRadarData;
    bool	bRadarPic;
    bool	bMosaicData;
    bool	bMosaicPic;
    bool	bPupData;
    bool	bPupPic;
    bool	bHighData;
    bool	bHighPic;
    vector<string> vecRecvID;	//�û�ID
    bool	bEnable;			//�Ƿ�����

    PrdcTransferSet()
    {
        bRadarData = false;
        bRadarPic = false;
        bMosaicData = false;
        bMosaicPic = false;
        bPupData = false;
        bPupPic = false;
        bHighData = false;
        bHighPic = false;
        bEnable = true;
    }
};

class PrdcTransferSetList
{
public:
    list<PrdcTransferSet> lst;
};

DLL_EXPORT_DECL bool PrdcTransferSetToXml(const string &strPath, const PrdcTransferSetList &info);
DLL_EXPORT_DECL bool XmlToPrdcTransferSet(const string &strPath, PrdcTransferSetList &info);


#endif