#include "../FlicsFacts/Model/movieresponse.hpp "
#include "../FlicsFacts/Controller/movieviewmanager.hpp"
#include "../FlicsFacts/Controller/shareresponsesformatter.hpp"
#include "../FlicsFacts/Controller/omdbresponseparser.hpp"
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
    std::vector <std::unique_ptr<MovieResponse> > movieResponses;
    MovieResponse movieResponse;
    movieResponses.emplace_back( std::make_unique<MovieResponse>(movieResponse) );
    ShareResponsesFormatter formatter;
    auto formattedResponses= formatter.formatAsText(movieResponses.cbegin(), movieResponses.cend());
    QVERIFY(formattedResponses.size() == 0);
}





QTEST_APPLESS_MAIN(TestFlicsFacts)

#include "testflicsfacts.moc"
