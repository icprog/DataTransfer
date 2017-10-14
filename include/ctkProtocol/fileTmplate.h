/**
  * @file   fileTmplate.h
  * @brief  �ļ�ģ��.
  *
  *  ��������ϸ˵����
  * @author wujun
  * @par   Copyright (c):
  *          wujun.
  *         All Rights Reserved
  * @date   2016:09:01
  * @note   ������ע�����
  * @version <�汾��>
  */

#ifndef fileTmplate_H_
#define fileTmplate_H_


#include <QString>
#include <QMap>
#include <QVector>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "../macro.h"


class FileTmplate
{
public:
    FileTmplate() {}
    vector<string> vecType;		///< ����
    vector<string> vecReg;		///< ����
    vector<int>	   vecIndex;	///< ����
};

DLL_EXPORT_DECL bool FileTmplateToXml(const string &strPath, const FileTmplate &set);
DLL_EXPORT_DECL bool XmlToFileTmplate(const string &strPath, FileTmplate &set);


#endif
