/**
  * @file   ConnectConfig.h
  * @brief  �����������߶Ȳ�ֱ����������л��뷴���л�����ͷ�ļ�.
  *
  *  ��������ϸ˵����
  * @author WEIKANGC
  * @par   Copyright (c):
  *          WEIKANGC.
  *         All Rights Reserved
  * @date   2016:05:25
  * @note   ������ע�����
  * @version <�汾��>
  */
#ifndef ParaConfig_h__
#define ParaConfig_h__
#include "macro.h"

#include <string>
#include <vector>
#include <map>
using namespace std;

#pragma warning(disable:4251)

/**
  * @class <CSStation>
  * @brief �״�վ������Ϣ����
  *
  *  �����������ϸ����
  * @note    ������ע������
  * @see     �����ǲο�����
 */
class DLL_EXPORT_CLASS_DECL CSStation
{
public:
    CSStation();
    CSStation(std::string sStationNo, std::string sStationName, const double fLon,
              const double fLat, const double elevation, std::string radarType, const double fRange, std::string sBasePath);
    CSStation(const CSStation& cSS);
    CSStation& operator=(const CSStation& cSS);
    ~CSStation();
public:
    void SetParam(std::string sStationNo, std::string sStationName, const double fLon,
                  const double fLat, const double elevation, std::string radarType, const double fRange, std::string sBasePath);
public:
    std::string m_sStationNo;	///< վ��
    std::string m_sStationName;	///< վ��
    double m_fLon;				///< ����
    double m_fLat;				///< γ��
    double m_elevation;			///< ���θ߶�
    std::string m_sRadarType;	///< �״��ͺ�
    double m_fRange;			///< ̽�ⷶΧ
    std::string m_sBasePath;	///< �����ݴ洢·��
};

/**
 * @brief: ���л�����վ����Ϣ��XmlFile
 * @param: std::string & sXmlPath
 * @param: const CSStation & pC
 * @return: DLL_EXPORT_DECL bool �ɹ����л�����true ���򷵻�false.
*/
DLL_EXPORT_DECL bool SerializeSStation2XmlFile(std::string& sXmlPath, const CSStation& pC);

/**
 * @brief: �����л�����վ����Ϣ
 * @param: const std::string & sXmlPath
 * @param: CSStation & pC
 * @return: DLL_EXPORT_DECL bool �ɹ����л�����true ���򷵻�false.
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2SStation(const std::string& sXmlPath, CSStation& pC);

/**
  * @class <StationConfig>
  * @brief  վ�㼯������.
  *
  *  �����������ϸ����
  * @note    ������ע������
  * @see     �����ǲο�����
 */
class DLL_EXPORT_CLASS_DECL StationConfig
{
public:
    StationConfig();
    StationConfig(const StationConfig& sC);
    StationConfig& operator=(const StationConfig& sC);
    ~StationConfig();
public:
    std::map< std::string, std::vector<CSStation> > m_ArrayRadar;
};

/**
 * @brief: ���л�StationConfigȥXML�ļ�
 * @param: std::string & sXmlPath
 * @param: const StationConfig & pC
 * @return: DLL_EXPORT_DECL bool �ɹ����л�����true ���򷵻�false.
*/
DLL_EXPORT_DECL bool SerializeStationConfig2XmlFile(std::string& sXmlPath, const StationConfig& pC);

/**
 * @brief: �����л�StationConfig
 * @param: const std::string & sXmlPath
 * @param: StationConfig & pC
 * @return: DLL_EXPORT_DECL bool �ɹ������л�����true ���򷵻�false.
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2StationConfig(const std::string& sXmlPath, StationConfig& pC);

/**
  * @class <>
  * @brief  .
  *
  *  �����������ϸ����
  * @note    ������ע������
  * @see     �����ǲο�����
 */
class DLL_EXPORT_CLASS_DECL StationS
{
public:
    StationS();
    StationS(const StationS& sC);
    StationS& operator=(const StationS& sC);
    ~StationS();
public:
    std::vector<CSStation> m_arrayStations;
};

/**
 * @brief: ������������
 * @param: std::string & sXmlPath
 * @param: const StationS & pC
 * @return: DLL_EXPORT_DECL bool: ��������ֵ
*/
DLL_EXPORT_DECL bool SerializeStationS2XmlFile(const std::string& sXmlPath, const StationS& pC);

/**
 * @brief: ������������
 * @param: const std::string & sXmlPath
 * @param: StationS & pC
 * @return: DLL_EXPORT_DECL bool: ��������ֵ
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2StationS(const std::string& sXmlPath, StationS& pC);

/**
  * @class <ParaConfig>
  * @brief  ���л�����ʱ��������߶���ֱ��ʲ���.
  *
  *  �����������ϸ����
  * @note    ������ע������
  * @see     �����ǲο�����
 */
class DLL_EXPORT_CLASS_DECL ParaConfig
{
public:
    ParaConfig();
    ParaConfig(std::string dir_, const unsigned int matchTime_, const unsigned int timeGap_,
               const unsigned int timeDelay_, const double raial_, const double zonal_);
    ParaConfig(const ParaConfig& pC);
    ParaConfig& operator=(const ParaConfig& pC);
    ~ParaConfig();
public:
    void SetParam(std::string dir_, const unsigned int matchTime_,const unsigned int timeGap_,
                  const unsigned int timeDelay_, const double raial_, const double zonal_);
public:
    std::string m_dir;					///�����ļ�·��
    unsigned int m_matchTime;			///ƥ��ʱ��
    unsigned int m_timeGap;				///ʱ����
    unsigned int m_timeDelay;			///ʱ���ӳ�
    double m_raial;						///����
    double m_zonal;     				///γ��
    std::map<unsigned int, bool> m_highlayers; ///�߶Ȳ�
};

/**
 * @brief: ���л��ͻ���������Ϣ��XML�ļ���
 * @param: std::string & sXml
 * @param: const ParaConfig & pC
 * @return: void
*/
DLL_EXPORT_DECL bool SerializePara2XmlFile(std::string& sXmlPath, const ParaConfig& pC);

/**
 * @brief: �����л��ͻ�����Ϣ���������
 * @param: const std::string & sXmlPath
 * @param: const ParaConfig & pC
 * @return: void
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2Para(const std::string& sXmlPath,  ParaConfig& pC);
#endif // ParaConfig_h__
