#ifndef PARSECC_H
#define PARSECC_H

#include "struct_cc.h"
#include "vtbsource.h"

//����CINRAD/CC�״�
class CParseCC
{
public:
    CParseCC();
    ~CParseCC();
    //����CINRAD/CC�״���ں���
    bool parse(const QString &fileName, RadarDataHead &fileHead, LineDataBlock *&dataBuffer);
    bool parse(const char *fileName, RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

private:
    bool isCCFile(const char *filename);
    bool loadVPPIData(const char *filename);
    void convertCCToVTB(RadarDataHead &filehead);
    //void convertCCBufferToVTB(RadarDataHead &filehead, LineDataBlock* &databuffer);
    void convertCCBufferToVTB(RadarDataHead &fileHead, QVector<LineDataBlock> &dataBuffer);

private:
    WEATHERRADAR m_ccFileHead;     //�ļ�ͷ
    short *m_pData;
    VPPISCANPARAMETER m_vppiParam[30];
    int m_layerNumber;                              //��ɨ����
};

#endif // PARSECC_H
