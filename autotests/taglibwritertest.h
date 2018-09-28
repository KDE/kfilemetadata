#ifndef TAGLIBWRITERTEST_H
#define TAGLIBWRITERTEST_H

#include <QObject>

class TagLibWriterTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testCommonData();
    void testCommonData_data();
};

#endif // TAGLIBWRITERTEST_H
