#include "../FlicsFacts/Model/moviesearchresponse.hpp "
#include "../FlicsFacts/Controller/movieviewmanager.hpp"
#include "../FlicsFacts/Controller/shareresponsesformatter.hpp"
#include "../FlicsFacts/Controller/tmdbresponseparser.hpp"
#include "../FlicsFacts/fam/qqmlobjectlistmodel.hpp"
#include <QTest>
#include <QDebug>
#include <QFile>
#include <QString>
#include <memory>


class TestFlicsFacts : public QObject
{
Q_OBJECT

public:
    TestFlicsFacts(QObject *parent = 0) :    QObject(parent){
    }

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testNoResponse();
};


void TestFlicsFacts::initTestCase()
{
}

void TestFlicsFacts::cleanupTestCase()
{
}


void TestFlicsFacts::testNoResponse()
{
    QQmlObjectListModel<MovieSearchResponse> movieResponses;
    movieResponses.append( new MovieSearchResponse(this));
    ShareResponsesFormatter formatter;
    auto formattedResponses= formatter.formatAsText(movieResponses.constBegin(), movieResponses.constEnd());
    QVERIFY(formattedResponses.size() == 0);
}





QTEST_APPLESS_MAIN(TestFlicsFacts)

#include "testflicsfacts.moc"
