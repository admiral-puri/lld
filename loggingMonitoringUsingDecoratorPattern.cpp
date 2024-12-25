#include <bits/stdc++.h>
using namespace std;

class RequestHandler
{
public:
    virtual ~RequestHandler() = default;
    virtual void handleRequest() = 0;
};

class CoreRequestHandler : public RequestHandler
{
public:
    void handleRequest()
    {
        cout << "request is being handled" << endl;
    }
};

class Decorator : public RequestHandler
{
    RequestHandler* reqHandler;

public:
    Decorator(RequestHandler* reqPtr): reqHandler(reqPtr){};
    void handleRequest()
    {
        reqHandler->handleRequest();
    }
};

class LoggingDecorator: public Decorator {
    public:
    LoggingDecorator(RequestHandler* reqPtr): Decorator(reqPtr){};
    void handleRequest()
    {
        cout<<"log the request"<<endl;
        Decorator::handleRequest();
    }
};

class MonitoringDecorator: public Decorator {
    public:
    MonitoringDecorator(RequestHandler* reqPtr): Decorator(reqPtr){};
    void handleRequest()
    {
        auto startTime = chrono::high_resolution_clock::now();
        Decorator::handleRequest();
        auto endTime = chrono::high_resolution_clock::now();
        chrono::duration<double,milli> elapsed = endTime - startTime;
        cout<<" request execution time:: "<< elapsed.count()<<" ms\n ";
    }
};



int main()
{
    CoreRequestHandler* reqHandler = new CoreRequestHandler();
    LoggingDecorator* loggingDecorator = new LoggingDecorator(reqHandler);
    MonitoringDecorator* monitoringDecorator = new MonitoringDecorator(loggingDecorator);

    monitoringDecorator->handleRequest();

}
