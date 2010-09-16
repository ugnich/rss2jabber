#include <gloox/component.h>
#include <gloox/connectionlistener.h>
#include <gloox/loghandler.h>
#include <gloox/presence.h>
#include <gloox/capabilities.h>
#include <gloox/subscription.h>
#include <gloox/iq.h>
#include <gloox/presencehandler.h>
#include <gloox/subscriptionhandler.h>
#include <gloox/iqhandler.h>
#include <gloox/vcard.h>
#include <gloox/disco.h>

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <boost/format.hpp>

using std::string;
using namespace gloox;
using boost::format;

class RSS2Jabber : public LogHandler,ConnectionListener,PresenceHandler,SubscriptionHandler,IqHandler {
  public:
    RSS2Jabber();
    virtual ~RSS2Jabber();

    void start();

    virtual void handleLog(LogLevel level,LogArea area,const std::string& message);

    virtual void onConnect();
    virtual void onDisconnect(ConnectionError e);
    virtual bool onTLSConnect(const CertInfo &info);

    virtual void handlePresence(const Presence &);
    virtual void handleSubscription(const Subscription &);
    virtual bool handleIq(const IQ &);
    virtual void handleIqID(const IQ &,int context);

    static int getRSSID(const string &);
    static string encloseSQL(const string &);
  private:
    void sendVCardReply(const JID &,const JID &,const string &);
    void sendPresence(const JID &,const JID &);

    static Component *j;
    static MYSQL sql;
    static Capabilities *caps;
};

int main(int /*argc*/,char** /*argv*/);
