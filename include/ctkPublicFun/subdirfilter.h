#ifndef subdirfilter_h__
#define subdirfilter_h__

#include <QMap>
#include <QString>

#include "../macro.h"
#include "pathbuilder.h"

typedef QMap<QString, QString> Level2filterRule;

typedef QMap<QString, Level2filterRule> FilterRule;

// �����amc��������Ҫ֧�ֶ���Ŀ¼���а����ļ�������

class DLL_EXPORT_CLASS_DECL CSubDirFilter
{
public:
    CSubDirFilter();

    virtual ~CSubDirFilter();

    /**
     * @brief:������subdirfilter.rule�����ļ��Ķ�ȡ
     * @return:void ��
     */
    void init();

    /**
     * @brief: ��ȡ���й���ID
     * @return:QT_NAMESPACE::QStringList ���й���ID
     */
    QStringList getAllRuleId();

    /**
     * @brief:���ݹ���ID�Լ�Ŀ¼���ơ�Ŀ¼�㼶����ƥ��
     * @param: const QString & ruleId �����ʶID
     * @param: const QString & dirname Ŀ¼����
     * @param: int iLevel Ŀ¼�㼶
     * @return: ����true��������Ŀ¼��������ƥ�䣻false��������Ŀ¼δ��������ƥ��
     */
    bool match(const QString &ruleId, const QString &dirname, int iLevel);

private:
    FilterRule m_oRule;
    CPathBuilder m_oPathBuilder;
};
#endif // subdirfilter_h__
