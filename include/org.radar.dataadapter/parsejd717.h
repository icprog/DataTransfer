/**
  * @file   parsejd717.h
  * @brief  ����717��ʽ�״����ݱ�׼��.
  *
  *  ��������ϸ˵����
  * @author nriet
  * @par   Copyright (c):
  *          nriet.
  *         All Rights Reserved
  * @date   2016.5.13
  * @note   ������ע�����
  * @version <�汾��>
  */
#ifndef PARSEJD717_H_
#define PARSEJD717_H_

#include "struct_vtb.h"
#include "struct_717.h"
#include <QVector>

class CParseJD717
{
public:
    CParseJD717(void);
    ~CParseJD717(void);

    // ��������717��ʽ�״��ļ�
    bool parse(const char*fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

    // ��ȡɨ�����
    int getLayNumber() const;

    static bool isJD717File(const char*fileName);

private:
    // ������717��ʽ�ļ�
    bool loadJD717File(const char *fileName);

    // ת����717��ʽ�״�����ͷ��VTB��ʽ
    void convertJD717HeaderToVTB(RadarDataHead &fileHead);
    // ת����717��ʽ�״������嵽VTB��ʽ
    void convertJD717BufferToVTB(RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

private:
    int m_layerNumber;	// ɨ�����
    int m_recordNumber; // ÿ��ļ�¼����
    NewHeader717 m_radarHeader717;
    QVector<QVector<DataRecord717>> m_717DataLst;

};


#endif //PARSEJD717_H_