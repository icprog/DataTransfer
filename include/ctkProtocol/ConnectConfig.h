/**
  * @file   ConnectConfig.h
  * @brief  ������Ϣ����ͨ���������л��뷴���л�����ͷ�ļ�.
  *
  *  ��������ϸ˵����
  * @author WEIKANGC
  * @par   Copyright (c):
  *          WEIKANGC.
  *         All Rights Reserved
  * @date   2016:05:16
  * @note   ������ע�����
  * @version <�汾��>
  */

#ifndef _H_H_CONNECTCONFIG_H_H_
#define _H_H_CONNECTCONFIG_H_H_

#include "macro.h"

#include <string>
using namespace std;

#pragma warning(disable:4251)
/**
  * @class <ClientConfig>
  * @brief  ��Ϣ���߿ͻ���ͨ����������.
  *
  *  �����������ϸ����
  * @note    ������ע������
  * @see     �����ǲο�����
 */

class DLL_EXPORT_CLASS_DECL ClientConfig
{
public:
    ClientConfig();
    ClientConfig(int nType, std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);
    ClientConfig(const ClientConfig& cC);
    ClientConfig& operator=(const ClientConfig& cC);
    ~ClientConfig();
public:
    void SetConnParam(int nType, std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);
public:
    int m_nType;			///< 0 TCP 1 IPC 2 INPROC
    std::string m_sIP;		///< ͨ��IP��ַ
    unsigned long m_nPort;	///< ͨ�Ŷ˿�
    std::string m_sProcess;	///< ���̼�ͨ��
    std::string m_sThread;	///< �̼߳�ͨ��
};


/**
 * @brief: ���л��ͻ������õ��ַ����� ClientConfig -> std::string
 * @param: std::string & sMsg
 * @param: const ClientConfig & m
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeClient2StdString(std::string& sCC, const ClientConfig& cC);


/**
 * @brief: �����л��ͻ������õ�ClientConfig���� std::string -> ClientConfig
 * @param: const std::string & sCC
 * @param: ClientConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeStdString2Client(const std::string& sCC, ClientConfig& cC);


/**
 * @brief: ���л��ͻ���������Ϣ��XML�ļ���
 * @param: std::string & sXml
 * @param: const ClientConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeClient2XmlFile(const std::string& sXmlPath, const ClientConfig& cC);


/**
 * @brief: �����л��ͻ�����Ϣ���������
 * @param: const std::string & sXmlPath
 * @param: ClientConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2Client(const std::string& sXmlPath,  ClientConfig& cC);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


/**
  * @class <ServerConfig>
  * @brief  ��Ϣ���߷����ͨ������������.
  *
  *  �����������ϸ����
  * @note    ������ע������
  * @see     �����ǲο�����
 */
class DLL_EXPORT_CLASS_DECL ServerConfig
{
public:
    ServerConfig();
    ServerConfig(int nType, const std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);
    ServerConfig(const ServerConfig& SC);
    ServerConfig& operator=(const ServerConfig& SC);
    ~ServerConfig();

    void SetConnParam(int nType, const std::string& sIP, unsigned long nPort, const std::string& sProcess, const std::string& sThread);

public:
    int m_nType;			///< 0 TCP 1 IPC 2 INPROC
    std::string m_sIP;		///< ͨ��IP��ַ
    unsigned long m_nPort;	///< ͨ�Ŷ˿�
    std::string m_sProcess;	///< ���̼�ͨ��
    std::string m_sThread;	///< �̼߳�ͨ��
};


/**
 * @brief: ���л���������õ��ַ����� ServerConfig -> std::string
 * @param: std::string & sMsg
 * @param: const ServerConfig & m
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeServer2StdString(std::string& sCC, const ServerConfig& sC);


/**
 * @brief: �����л���������õ�ServerConfig���� std::string -> ServerConfig
 * @param: const std::string & sCC
 * @param: ServerConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeStdString2Server(const std::string& sCC, ServerConfig& sC);


/**
 * @brief: ���л������������Ϣ��XML�ļ���
 * @param: std::string & sXml
 * @param: const ServerConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool SerializeServer2XmlFile(std::string& sXmlPath, const ServerConfig& sC);


/**
 * @brief: �����л��������Ϣ���������
 * @param: const std::string & sXmlPath
 * @param: ServerConfig & cC
 * @return: bool
*/
DLL_EXPORT_DECL bool DeserializeXmlFile2Server(const std::string& sXmlPath,  ServerConfig& sC);

#endif



