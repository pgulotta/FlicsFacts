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
    void testFormatRequest();
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

void TestFlicsFacts::testFormatRequest()
{
    std::vector <std::unique_ptr<MovieResponse> > movieResponses;
    MovieResponse movieResponse;
    movieResponse.Title = "My movieResponse.Title";
    movieResponse.Year = "2013";
    movieResponse.Rated = "movieResponse.Rated";
    movieResponse.Released = "March 1, 2013";
    movieResponse.Runtime = "169 minutes";
    movieResponse.Actors = "movieResponse.Actors";
    movieResponse.ImdbRating = "5.9";
    movieResponse.TomatoRating = "4.8";
    movieResponse.Website = "movieResponse.Website";
    movieResponse.Genre = "comedy, family";
    movieResponse.Plot = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    movieResponses.emplace_back( std::make_unique<MovieResponse>(movieResponse) );
    ShareResponsesFormatter formatter;
    auto formattedResponses1= formatter.formatAsText(movieResponses.cbegin(), movieResponses.cend());
    QVERIFY(formattedResponses1.size() >0);
    auto formattedResponses2= formatter.formatAsText(movieResponses.cbegin(), movieResponses.cend());
    QVERIFY(formattedResponses2.size() >0);
    QCOMPARE(formattedResponses1, formattedResponses2);
}



QTEST_APPLESS_MAIN(TestFlicsFacts)

#include "testflicsfacts.moc"
