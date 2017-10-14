/**
  * @file   dataclearSet.h
  * @brief  ��������������л�.
  *
  *  ��������ϸ˵����
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:05:19
  * @note   ������ע�����
  * @version <�汾��>
  */
#ifndef dataclearSet_H_
#define dataclearSet_H_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class DataClearSet
{
public:
    string  strTask;				///< ��������Ҫ��Ψһ��
    string	strPath;				///< �ļ�·��
    string	strDrule;				///< ���ȹ���
    int	nStyle;						///< ɾ����ʽ��0-ʱ�䣬1-�ռ䣬2-���ߣ�
    vector<string> reges;			///< ƥ�����������ʽ��
    int	nSize;						///< ʣ��ռ䣨��λ��M��
    int	nTime;						///< ����ʱ�䣨��λ��h��

    //����==����������ֱ��remove
    bool operator==(const DataClearSet& e)
    {
        if ( this->strPath == e.strPath )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class DataClearSetList
{
public:
    list<DataClearSet> lst;
};


DLL_EXPORT_DECL bool DataClearSetToXml(const string &strPath, const DataClearSetList &setlst);
DLL_EXPORT_DECL bool XmlToDataClearSet(const string &strPath, DataClearSetList &setlst);

// /**
//   * @class <DataClearSet_N>
//   * @brief  .
//   *
//   *  �����������ϸ����
//   * @note    ������ע������
//   * @see     �����ǲο�����
//  */
// class DataClearSet_N
// {
// public:
//     std::string sClearTask;	///< �������
//     std::string sDir;		///< ���Ŀ¼
//     std::string sFileRule;	///< �ļ�ģ��
//     int		bClearType;		///< �����ʽ	0-��� 1-��ѯ
//     int		bClearscheme;	///< �������  ��0-ʱ�䣬1-�ռ䣬2-���ߣ�
//     int		nSize;			///< ʣ��ռ䣨��λ��M��
//     int		nTime;			///< ����ʱ�䣨��λ��t��
//     string	strDrule;		///< ���ȹ���
// };
//
// /**
//   * @class <DataClearSetList_N>
//   * @brief  .
//   *
//   *  �����������ϸ����
//   * @note    ������ע������
//   * @see     �����ǲο�����
//  */
// class DataClearSetList_N
// {
// public:
//     std::vector<DataClearSet_N> arrayDCS;
// };
//
// /**
//  * @brief: ������������
//  * @param: const string & strPath
//  * @param: const DataClearSetList_N & setlst
//  * @return: DLL_EXPORT_DECL bool: ��������ֵ
// */
// DLL_EXPORT_DECL bool DataClearSetNToXml(const string &strPath, const DataClearSetList_N &setlst);
//
// /**
//  * @brief: ������������
//  * @param: const string & strPath
//  * @param: DataClearSetList_N & setlst
//  * @return: DLL_EXPORT_DECL bool: ��������ֵ
// */
// DLL_EXPORT_DECL bool XmlToDataClearSetN(const string &strPath, DataClearSetList_N &setlst);

#endif