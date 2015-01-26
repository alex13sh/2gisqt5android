/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Copyright (C) 2014 Keith Gardner <kreios4004@gmail.com>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include <private/qversionnumber_p.h>

class tst_QVersionNumber : public QObject
{
    Q_OBJECT

private:
    void singleInstanceData();
    void comparisonData();

private slots:
    void initTestCase();
    void constructorDefault();
    void constructorVersioned_data();
    void constructorVersioned();
    void constructorExplicit();
    void constructorCopy_data();
    void constructorCopy();
    void compareGreater_data();
    void compareGreater();
    void compareGreaterEqual_data();
    void compareGreaterEqual();
    void compareLess_data();
    void compareLess();
    void compareLessEqual_data();
    void compareLessEqual();
    void compareEqual_data();
    void compareEqual();
    void compareNotEqual_data();
    void compareNotEqual();
    void compare_data();
    void compare();
    void isPrefixOf_data();
    void isPrefixOf();
    void commonPrefix_data();
    void commonPrefix();
    void normalized_data();
    void normalized();
    void isNormalized_data();
    void isNormalized();
    void assignment_data();
    void assignment();
    void fromString_data();
    void fromString();
    void toString_data();
    void toString();
    void isNull_data();
    void isNull();
    void serialize_data();
    void serialize();
    void moveSemantics();
};

void tst_QVersionNumber::singleInstanceData()
{
    QTest::addColumn<QVector<int> >("segments");
    QTest::addColumn<QVersionNumber>("expectedVersion");
    QTest::addColumn<QString>("expectedString");
    QTest::addColumn<QString>("constructionString");
    QTest::addColumn<int>("suffixIndex");
    QTest::addColumn<bool>("isNull");

    //                                        segments                           expectedVersion                                                      expectedString                    constructionString                         suffixIndex   null
    QTest::newRow("null")                  << QVector<int>()                  << QVersionNumber(QVector<int>())                                    << QString()                      << QString()                               << 0          << true;
    QTest::newRow("text")                  << QVector<int>()                  << QVersionNumber(QVector<int>())                                    << QString()                      << QStringLiteral("text")                  << 0          << true;
    QTest::newRow(" text")                 << QVector<int>()                  << QVersionNumber(QVector<int>())                                    << QString()                      << QStringLiteral(" text")                 << 0          << true;
    QTest::newRow("Empty String")          << QVector<int>()                  << QVersionNumber(QVector<int>())                                    << QString()                      << QStringLiteral("Empty String")          << 0          << true;
    QTest::newRow("-1.-2")                 << (QVector<int>())                << QVersionNumber()                                                  << QStringLiteral("")             << QStringLiteral("-1.-2")                 << 0          << true;
    QTest::newRow("1.-2-3")                << (QVector<int>() << 1)           << QVersionNumber(QVector<int>() << 1)                               << QStringLiteral("1")            << QStringLiteral("1.-2-3")                << 1          << false;
    QTest::newRow("1.2-3")                 << (QVector<int>() << 1 << 2)      << QVersionNumber(QVector<int>() << 1 << 2)                          << QStringLiteral("1.2")          << QStringLiteral("1.2-3")                 << 3          << false;
    QTest::newRow("0")                     << (QVector<int>() << 0)           << QVersionNumber(QVector<int>() << 0)                               << QStringLiteral("0")            << QStringLiteral("0")                     << 1          << false;
    QTest::newRow("0.1")                   << (QVector<int>() << 0 << 1)      << QVersionNumber(QVector<int>() << 0 << 1)                          << QStringLiteral("0.1")          << QStringLiteral("0.1")                   << 3          << false;
    QTest::newRow("0.1.2")                 << (QVector<int>() << 0 << 1 << 2) << QVersionNumber(0, 1, 2)                                           << QStringLiteral("0.1.2")        << QStringLiteral("0.1.2")                 << 5          << false;
    QTest::newRow("0.1.2alpha")            << (QVector<int>() << 0 << 1 << 2) << QVersionNumber(0, 1, 2)                                           << QStringLiteral("0.1.2")        << QStringLiteral("0.1.2alpha")            << 5          << false;
    QTest::newRow("0.1.2-alpha")           << (QVector<int>() << 0 << 1 << 2) << QVersionNumber(0, 1, 2)                                           << QStringLiteral("0.1.2")        << QStringLiteral("0.1.2-alpha")           << 5          << false;
    QTest::newRow("0.1.2.alpha")           << (QVector<int>() << 0 << 1 << 2) << QVersionNumber(0, 1, 2)                                           << QStringLiteral("0.1.2")        << QStringLiteral("0.1.2.alpha")           << 5          << false;
    QTest::newRow("0.1.2.3.4.alpha")       << (QVector<int>() << 0 << 1 << 2 << 3 << 4) << QVersionNumber(QVector<int>() << 0 << 1 << 2 << 3 << 4) << QStringLiteral("0.1.2.3.4")    << QStringLiteral("0.1.2.3.4.alpha")       << 9          << false;
    QTest::newRow("0.1.2.3.4 alpha")       << (QVector<int>() << 0 << 1 << 2 << 3 << 4) << QVersionNumber(QVector<int>() << 0 << 1 << 2 << 3 << 4) << QStringLiteral("0.1.2.3.4")    << QStringLiteral("0.1.2.3.4 alpha")       << 9          << false;
    QTest::newRow("0.1.2.3.4 alp ha")      << (QVector<int>() << 0 << 1 << 2 << 3 << 4) << QVersionNumber(QVector<int>() << 0 << 1 << 2 << 3 << 4) << QStringLiteral("0.1.2.3.4")    << QStringLiteral("0.1.2.3.4 alp ha")      << 9          << false;
    QTest::newRow("0.1.2.3.4alp ha")       << (QVector<int>() << 0 << 1 << 2 << 3 << 4) << QVersionNumber(QVector<int>() << 0 << 1 << 2 << 3 << 4) << QStringLiteral("0.1.2.3.4")    << QStringLiteral("0.1.2.3.4alp ha")       << 9          << false;
    QTest::newRow("0.1.2.3.4alpha ")       << (QVector<int>() << 0 << 1 << 2 << 3 << 4) << QVersionNumber(QVector<int>() << 0 << 1 << 2 << 3 << 4) << QStringLiteral("0.1.2.3.4")    << QStringLiteral("0.1.2.3.4alpha ")       << 9          << false;
    QTest::newRow("10.09")                 << (QVector<int>() << 10 << 9)     << QVersionNumber(QVector<int>() << 10 << 9)                         << QStringLiteral("10.9")         << QStringLiteral("10.09")                 << 5          << false;
    QTest::newRow("10.0x")                 << (QVector<int>() << 10 << 0)     << QVersionNumber(QVector<int>() << 10 << 0)                         << QStringLiteral("10.0")         << QStringLiteral("10.0x")                 << 4          << false;
    QTest::newRow("10.0xTest")             << (QVector<int>() << 10 << 0)     << QVersionNumber(QVector<int>() << 10 << 0)                         << QStringLiteral("10.0")         << QStringLiteral("10.0xTest")             << 4          << false;
}

void tst_QVersionNumber::comparisonData()
{
    QTest::addColumn<QVersionNumber>("lhs");
    QTest::addColumn<QVersionNumber>("rhs");
    QTest::addColumn<bool>("equal");
    QTest::addColumn<bool>("notEqual");
    QTest::addColumn<bool>("lessThan");
    QTest::addColumn<bool>("lessThanOrEqual");
    QTest::addColumn<bool>("greaterThan");
    QTest::addColumn<bool>("greaterThanOrEqual");
    QTest::addColumn<int>("compareResult");
    QTest::addColumn<bool>("isPrefix");
    QTest::addColumn<QVersionNumber>("common");

    //                                LHS                          RHS                          ==       !=       <        <=       >        >=       compareResult     isPrefixOf        commonPrefix
    QTest::newRow("null, null")    << QVersionNumber()          << QVersionNumber()          << true  << false << false << true  << false << true  <<  0             << true           << QVersionNumber();
    QTest::newRow("null, 0")       << QVersionNumber()          << QVersionNumber(0)         << false << true  << true  << true  << false << false << -1             << true           << QVersionNumber();
    QTest::newRow("0, null")       << QVersionNumber(0)         << QVersionNumber()          << false << true  << false << false << true  << true  <<  1             << false          << QVersionNumber();
    QTest::newRow("0, 0")          << QVersionNumber(0)         << QVersionNumber(0)         << true  << false << false << true  << false << true  <<  0             << true           << QVersionNumber(0);
    QTest::newRow("1.0, 1.0")      << QVersionNumber(1, 0)      << QVersionNumber(1, 0)      << true  << false << false << true  << false << true  <<  0             << true           << QVersionNumber(1, 0);
    QTest::newRow("1, 1.0")        << QVersionNumber(1)         << QVersionNumber(1, 0)      << false << true  << true  << true  << false << false << -1             << true           << QVersionNumber(1);
    QTest::newRow("1.0, 1")        << QVersionNumber(1, 0)      << QVersionNumber(1)         << false << true  << false << false << true  << true  <<  1             << false          << QVersionNumber(1);
    QTest::newRow("0.1.2, 0.1")    << QVersionNumber(0, 1, 2)   << QVersionNumber(0, 1)      << false << true  << false << false << true  << true  <<  2             << false          << QVersionNumber(0, 1);
    QTest::newRow("0.1, 0.1.2")    << QVersionNumber(0, 1)      << QVersionNumber(0, 1, 2)   << false << true  << true  << true  << false << false << -2             << true           << QVersionNumber(0, 1);
    QTest::newRow("0.1.2, 0.1.2")  << QVersionNumber(0, 1, 2)   << QVersionNumber(0, 1, 2)   << true  << false << false << true  << false << true  <<  0             << true           << QVersionNumber(0, 1, 2);
    QTest::newRow("0.1.2, 1.1.2")  << QVersionNumber(0, 1, 2)   << QVersionNumber(1, 1, 2)   << false << true  << true  << true  << false << false << -1             << false          << QVersionNumber();
    QTest::newRow("1.1.2, 0.1.2")  << QVersionNumber(1, 1, 2)   << QVersionNumber(0, 1, 2)   << false << true  << false << false << true  << true  <<  1             << false          << QVersionNumber();
    QTest::newRow("1, -1")         << QVersionNumber(1)         << QVersionNumber(-1)        << false << true  << false << false << true  << true  <<  2             << false          << QVersionNumber();
    QTest::newRow("-1, 1")         << QVersionNumber(-1)        << QVersionNumber(1)         << false << true  << true  << true  << false << false << -2             << false          << QVersionNumber();
    QTest::newRow("0.1, 0.-1")     << QVersionNumber(0, 1)      << QVersionNumber(0, -1)     << false << true  << false << false << true  << true  <<  2             << false          << QVersionNumber(0);
    QTest::newRow("0.-1, 0.1")     << QVersionNumber(0, -1)     << QVersionNumber(0, 1)      << false << true  << true  << true  << false << false << -2             << false          << QVersionNumber(0);
    QTest::newRow("0.-1, 0")       << QVersionNumber(0, -1)     << QVersionNumber(0)         << false << true  << true  << true  << false << false << -1             << false          << QVersionNumber(0);
    QTest::newRow("0, 0.-1")       << QVersionNumber(0)         << QVersionNumber(0, -1)     << false << true  << false << false << true  << true  <<  1             << true           << QVersionNumber(0);
}

void tst_QVersionNumber::initTestCase()
{
    qRegisterMetaType<QVector<int> >();
}

void tst_QVersionNumber::constructorDefault()
{
    QVersionNumber version;

    QCOMPARE(version.majorVersion(), 0);
    QCOMPARE(version.minorVersion(), 0);
    QCOMPARE(version.microVersion(), 0);
    QCOMPARE(version.segments(), QVector<int>());
}

void tst_QVersionNumber::constructorVersioned_data()
{
    singleInstanceData();
}

void tst_QVersionNumber::constructorVersioned()
{
    QFETCH(QVector<int>, segments);
    QFETCH(QVersionNumber, expectedVersion);

    QVersionNumber version(segments);
    QCOMPARE(version.majorVersion(), expectedVersion.majorVersion());
    QCOMPARE(version.minorVersion(), expectedVersion.minorVersion());
    QCOMPARE(version.microVersion(), expectedVersion.microVersion());
    QCOMPARE(version.segments(), expectedVersion.segments());
}

void tst_QVersionNumber::constructorExplicit()
{
    QVersionNumber v1(1);
    QVersionNumber v2(QVector<int>() << 1);

    QCOMPARE(v1.segments(), v2.segments());

    QVersionNumber v3(1, 2);
    QVersionNumber v4(QVector<int>() << 1 << 2);

    QCOMPARE(v3.segments(), v4.segments());

    QVersionNumber v5(1, 2, 3);
    QVersionNumber v6(QVector<int>() << 1 << 2 << 3);

    QCOMPARE(v5.segments(), v6.segments());

#ifdef Q_COMPILER_INITIALIZER_LISTS
    QVersionNumber v7(4, 5, 6);
    QVersionNumber v8 = {4, 5, 6};

    QCOMPARE(v7.segments(), v8.segments());
#endif
}

void tst_QVersionNumber::constructorCopy_data()
{
    singleInstanceData();
}

void tst_QVersionNumber::constructorCopy()
{
    QFETCH(QVector<int>, segments);
    QFETCH(QVersionNumber, expectedVersion);

    QVersionNumber original(segments);
    QVersionNumber version(original);

    QCOMPARE(version.majorVersion(), expectedVersion.majorVersion());
    QCOMPARE(version.minorVersion(), expectedVersion.minorVersion());
    QCOMPARE(version.microVersion(), expectedVersion.microVersion());
    QCOMPARE(version.segments(), expectedVersion.segments());
}

void tst_QVersionNumber::compareGreater_data()
{
    comparisonData();
}

void tst_QVersionNumber::compareGreater()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(bool, greaterThan);

    QCOMPARE(lhs > rhs, greaterThan);
}

void tst_QVersionNumber::compareGreaterEqual_data()
{
    comparisonData();
}

void tst_QVersionNumber::compareGreaterEqual()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(bool, greaterThanOrEqual);

    QCOMPARE(lhs >= rhs, greaterThanOrEqual);
}

void tst_QVersionNumber::compareLess_data()
{
    comparisonData();
}

void tst_QVersionNumber::compareLess()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(bool, lessThan);

    QCOMPARE(lhs < rhs, lessThan);
}

void tst_QVersionNumber::compareLessEqual_data()
{
    comparisonData();
}

void tst_QVersionNumber::compareLessEqual()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(bool, lessThanOrEqual);

    QCOMPARE(lhs <= rhs, lessThanOrEqual);
}

void tst_QVersionNumber::compareEqual_data()
{
    comparisonData();
}

void tst_QVersionNumber::compareEqual()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(bool, equal);

    QCOMPARE(lhs == rhs, equal);
}

void tst_QVersionNumber::compareNotEqual_data()
{
    comparisonData();
}

void tst_QVersionNumber::compareNotEqual()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(bool, notEqual);

    QCOMPARE(lhs != rhs, notEqual);
}

void tst_QVersionNumber::compare_data()
{
    comparisonData();
}

void tst_QVersionNumber::compare()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(int, compareResult);

    QCOMPARE(QVersionNumber::compare(lhs, rhs), compareResult);
}

void tst_QVersionNumber::isPrefixOf_data()
{
    comparisonData();
}

void tst_QVersionNumber::isPrefixOf()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(bool, isPrefix);

    QCOMPARE(lhs.isPrefixOf(rhs), isPrefix);
}

void tst_QVersionNumber::commonPrefix_data()
{
    comparisonData();
}

void tst_QVersionNumber::commonPrefix()
{
    QFETCH(QVersionNumber, lhs);
    QFETCH(QVersionNumber, rhs);
    QFETCH(QVersionNumber, common);

    QVersionNumber calculatedPrefix = QVersionNumber::commonPrefix(lhs, rhs);
    QCOMPARE(calculatedPrefix, common);
    QCOMPARE(calculatedPrefix.segments(), common.segments());
}

void tst_QVersionNumber::normalized_data()
{
    QTest::addColumn<QVersionNumber>("version");
    QTest::addColumn<QVersionNumber>("expected");

    QTest::newRow("0")        << QVersionNumber(0) << QVersionNumber();
    QTest::newRow("1")        << QVersionNumber(1) << QVersionNumber(1);
    QTest::newRow("1.2")      << QVersionNumber(1, 2) << QVersionNumber(1, 2);
    QTest::newRow("1.0")      << QVersionNumber(1, 0) << QVersionNumber(1);
    QTest::newRow("1.0.0")      << QVersionNumber(1, 0, 0) << QVersionNumber(1);
    QTest::newRow("1.0.1")      << QVersionNumber(1, 0, 1) << QVersionNumber(1, 0, 1);
    QTest::newRow("1.0.1.0")      << QVersionNumber(QVector<int>() << 1 << 0 << 1 << 0) << QVersionNumber(1, 0, 1);
    QTest::newRow("0.0.1.0")      << QVersionNumber(QVector<int>() << 0 << 0 << 1 << 0) << QVersionNumber(0, 0, 1);
}

void tst_QVersionNumber::normalized()
{
    QFETCH(QVersionNumber, version);
    QFETCH(QVersionNumber, expected);

    QCOMPARE(version.normalized(), expected);
    QCOMPARE(qMove(version).normalized(), expected);
}

void tst_QVersionNumber::isNormalized_data()
{
    QTest::addColumn<QVersionNumber>("version");
    QTest::addColumn<bool>("expected");

    QTest::newRow("null")        << QVersionNumber() << true;
    QTest::newRow("0")        << QVersionNumber(0) << false;
    QTest::newRow("1")        << QVersionNumber(1) << true;
    QTest::newRow("1.2")      << QVersionNumber(1, 2) << true;
    QTest::newRow("1.0")      << QVersionNumber(1, 0) << false;
    QTest::newRow("1.0.0")      << QVersionNumber(1, 0, 0) << false;
    QTest::newRow("1.0.1")      << QVersionNumber(1, 0, 1) << true;
}

void tst_QVersionNumber::isNormalized()
{
    QFETCH(QVersionNumber, version);
    QFETCH(bool, expected);

    QCOMPARE(version.isNormalized(), expected);
}

void tst_QVersionNumber::assignment_data()
{
    singleInstanceData();
}

void tst_QVersionNumber::assignment()
{
    QFETCH(QVector<int>, segments);
    QFETCH(QVersionNumber, expectedVersion);

    QVersionNumber original(segments);
    QVersionNumber version;
    version = original;

    QCOMPARE(version.majorVersion(), expectedVersion.majorVersion());
    QCOMPARE(version.minorVersion(), expectedVersion.minorVersion());
    QCOMPARE(version.microVersion(), expectedVersion.microVersion());
    QCOMPARE(version.segments(), expectedVersion.segments());
}

void tst_QVersionNumber::fromString_data()
{
    singleInstanceData();

    const quint64 largerThanIntCanHold = quint64(std::numeric_limits<int>::max()) + 1;
    const QString largerThanIntCanHoldString0 = QString::number(largerThanIntCanHold) + ".0";
    const QString largerThanIntCanHoldString1 = "0." + QString::number(largerThanIntCanHold);

    QTest::newRow(qPrintable(largerThanIntCanHoldString0))
            << QVector<int>()  << QVersionNumber()  << QString()           << largerThanIntCanHoldString0 << 0 << true;
    QTest::newRow(qPrintable(largerThanIntCanHoldString1))
            << QVector<int>(0) << QVersionNumber(0) << QStringLiteral("0") << largerThanIntCanHoldString1 << 1 << true;

    const QString largerThanULongLongCanHoldString0 = QString::number(std::numeric_limits<qulonglong>::max()) + "0.0";      // 10x ULLONG_MAX
    const QString largerThanULongLongCanHoldString1 = "0." + QString::number(std::numeric_limits<qulonglong>::max()) + '0'; // 10x ULLONG_MAX

    QTest::newRow(qPrintable(largerThanULongLongCanHoldString0))
            << QVector<int>()  << QVersionNumber()  << QString()           << largerThanULongLongCanHoldString0 << 0 << true;
    QTest::newRow(qPrintable(largerThanULongLongCanHoldString1))
            << QVector<int>(0) << QVersionNumber(0) << QStringLiteral("0") << largerThanULongLongCanHoldString1 << 1 << true;
}

void tst_QVersionNumber::fromString()
{
    QFETCH(QString, constructionString);
    QFETCH(QVersionNumber, expectedVersion);
    QFETCH(int, suffixIndex);

    int index;
    QCOMPARE(QVersionNumber::fromString(constructionString), expectedVersion);
    QCOMPARE(QVersionNumber::fromString(constructionString, &index), expectedVersion);
    QCOMPARE(index, suffixIndex);
}

void tst_QVersionNumber::toString_data()
{
    singleInstanceData();

    //                                        segments                      expectedVersion            expectedString      constructionString     suffixIndex   null
    QTest::newRow("-1")                    << (QVector<int>() << -1)       << QVersionNumber(-1)      << QString("-1")    << QString()           << 0          << true;
    QTest::newRow("-1.0")                  << (QVector<int>() << -1 << 0)  << QVersionNumber(-1, 0)   << QString("-1.0")  << QString()           << 0          << true;
    QTest::newRow("1.-2")                  << (QVector<int>() << 1 << -2)  << QVersionNumber(1, -2)   << QString("1.-2")  << QString()           << 0          << true;
}

void tst_QVersionNumber::toString()
{
    QFETCH(QVersionNumber, expectedVersion);
    QFETCH(QString, expectedString);

    QCOMPARE(expectedVersion.toString(), expectedString);
}

void tst_QVersionNumber::isNull_data()
{
    singleInstanceData();
}

void tst_QVersionNumber::isNull()
{
    QFETCH(QVector<int>, segments);
    QFETCH(bool, isNull);

    QVersionNumber version(segments);

    QCOMPARE(version.isNull(), isNull);
}

void tst_QVersionNumber::serialize_data()
{
    singleInstanceData();
}

void tst_QVersionNumber::serialize()
{
    QFETCH(QVector<int>, segments);

    QVersionNumber original(segments);
    QVersionNumber version;

    QByteArray buffer;
    {
        QDataStream ostream(&buffer, QIODevice::WriteOnly);
        ostream << original;
    }
    {
        QDataStream istream(buffer);
        istream >> version;
    }

    QCOMPARE(version.majorVersion(), original.majorVersion());
    QCOMPARE(version.minorVersion(), original.minorVersion());
    QCOMPARE(version.microVersion(), original.microVersion());
    QCOMPARE(version.segments(), original.segments());
}

void tst_QVersionNumber::moveSemantics()
{
#if defined(_MSC_VER) && _MSC_VER == 1600
#  define Q_MSVC_2010
#endif
#if defined(Q_COMPILER_RVALUE_REFS) && !defined(Q_MSVC_2010)
    // QVersionNumber(QVersionNumber &&)
    {
        QVersionNumber v1(1, 2, 3);
        QVersionNumber v2 = qMove(v1);
        QCOMPARE(v2, QVersionNumber(1, 2, 3));
    }
    // QVersionNumber &operator=(QVersionNumber &&)
    {
        QVersionNumber v1(1, 2, 3);
        QVersionNumber v2;
        v2 = qMove(v1);
        QCOMPARE(v2, QVersionNumber(1, 2, 3));
    }
    // QVersionNumber(QVector<int> &&)
    {
        QVector<int> segments = QVector<int>() << 1 << 2 << 3;
        QVersionNumber v1(segments);
        QVersionNumber v2(qMove(segments));
        QVERIFY(!v1.isNull());
        QVERIFY(!v2.isNull());
        QCOMPARE(v1, v2);
    }
#endif
#if defined(Q_COMPILER_REF_QUALIFIERS) && !defined(Q_MSVC_2010)
    // normalized()
    {
        QVersionNumber v(1, 0, 0);
        QVERIFY(!v.isNull());
        QVersionNumber nv;
        nv = v.normalized();
        QVERIFY(!v.isNull());
        QVERIFY(!nv.isNull());
        QVERIFY(nv.isNormalized());
        nv = qMove(v).normalized();
        QVERIFY(!nv.isNull());
        QVERIFY(nv.isNormalized());
    }
    // segments()
    {
        QVersionNumber v(1, 2, 3);
        QVERIFY(!v.isNull());
        QVector<int> segments;
        segments = v.segments();
        QVERIFY(!v.isNull());
        QVERIFY(!segments.empty());
        segments = qMove(v).segments();
        QVERIFY(!segments.empty());
    }
#endif
#if !defined(Q_COMPILER_RVALUE_REFS) && !defined(Q_COMPILER_REF_QUALIFIERS) && !defined(Q_MSVC_2010)
    QSKIP("This test requires C++11 move semantics support in the compiler.");
#elif defined(Q_MSVC_2010)
    QSKIP("This test requires compiler generated move constructors and operators.");
#endif
}

QTEST_APPLESS_MAIN(tst_QVersionNumber)

#include "tst_qversionnumber.moc"