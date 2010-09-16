#include "rss2jabber.h"

Component *RSS2Jabber::j;
MYSQL RSS2Jabber::sql;
Capabilities *RSS2Jabber::caps;

RSS2Jabber::RSS2Jabber() {}
RSS2Jabber::~RSS2Jabber() {}

void RSS2Jabber::start() {
  mysql_init(&sql);
  my_bool reconnect=1;
  mysql_options(&sql,MYSQL_OPT_RECONNECT,&reconnect);
  mysql_options(&sql,MYSQL_SET_CHARSET_NAME,"utf8");
  if(!mysql_real_connect(&sql,"localhost","rss2jabber","secret","rss2jabber",0,NULL,0)) {
    printf("Can't connect to mysql.\n");
    return;
  }

  j=new Component(XMLNS_COMPONENT_ACCEPT,"127.0.0.1","rss2jabber.com","secret",5347);
  j->registerConnectionListener(this);
  j->logInstance().registerLogHandler(LogLevelWarning,LogAreaAll,this);
  j->registerPresenceHandler(this);
  j->registerSubscriptionHandler(this);
  j->registerIqHandler(this,ExtVCard);

  j->disco()->setIdentity("application","bot","RSS2Jabber");
  j->disco()->setVersion("RSS2Jabber","1.0","Linux");
  j->disco()->removeFeature("jabber:iq:version");
  j->disco()->removeFeature("urn:xmpp:ping");
  
  j->disco()->addFeature("http://jabber.org/protocol/caps");
  j->disco()->addFeature("http://jabber.org/protocol/disco#info");
  j->disco()->addFeature("http://jabber.org/protocol/disco#items");
//  j->disco()->addFeature("vcard-temp");

  caps=new Capabilities(j->disco());
  caps->setNode("http://rss2jabber.com/caps");
  caps->ver();  // TAK NADO!!!

  j->connect();
  delete(caps);
  delete(j);
  
}

void RSS2Jabber::onConnect() {
  printf("CONNECTED\n");
  j->setFrom("rss2jabber@rss2jabber.com/RSS2Jabber");
  j->selectResource("RSS2Jabber");
}
      
void RSS2Jabber::onDisconnect(ConnectionError e) {
  printf("DISCONNECT\n");
}
	
bool RSS2Jabber::onTLSConnect(const CertInfo &info) {
  printf("TLS CONNECT\n");
  return false;
}

void RSS2Jabber::handleLog( LogLevel level, LogArea area, const std::string& message) {
  printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}

string RSS2Jabber::encloseSQL(const string &ostr) {
  string::const_iterator s=ostr.begin(),smax=ostr.end();
  string str;
  str.reserve(smax-s);
  while(s!=smax) {
    switch(*s) { 
      case '\'': str+="\\'"; break;
      case '\"': str+="\\\""; break;
      case '\\': str+="\\\\"; break;
      default: str+=*s;
    }
    s++;
  }
  return str;
}

int RSS2Jabber::getRSSID(const string &jid) {
  string query="SELECT rss_id FROM rss WHERE jid='";
  query+=encloseSQL(jid);
  query+="'";
  MYSQL_RES *res;
  MYSQL_ROW row;
  int rssid=0;
  if(!mysql_query(&sql,query.c_str()) && (res=mysql_use_result(&sql))) {
    if((row=mysql_fetch_row(res))) rssid=atoi(row[0]);
    mysql_free_result(res);
  }
  return rssid;
}

int main(int /*argc*/,char** /*argv*/) {
  printf("START\n");
  fflush(NULL);

  RSS2Jabber *r=new RSS2Jabber();
  r->start();
  delete(r);
  return 0;
}
