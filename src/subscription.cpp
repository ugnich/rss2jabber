#include "rss2jabber.h"

void RSS2Jabber::handleSubscription(const Subscription &subscription) {
  int rssid=getRSSID(subscription.to().username());
  if(rssid==0) return;

  Subscription::S10nType subtype=subscription.subtype();
  if(subtype==Subscription::Subscribe) {
    string query=str(format("INSERT INTO jabber(rss_id,jid,lastonline) VALUES (%u,'%s',NOW())") % rssid % encloseSQL(subscription.from().bare()));
    mysql_query(&sql,query.c_str());
    
    Subscription *s=new Subscription(Subscription::Subscribed,subscription.from().bare());
    s->setFrom(subscription.to().bare());
    j->send(*s);
    delete(s);

    sendPresence(subscription.to(),subscription.from());
    
    s=new Subscription(Subscription::Subscribe,subscription.from().bare());
    s->setFrom(subscription.to().bare());
    j->send(*s);
    delete(s);
  } else if(subtype==Subscription::Unsubscribe || subtype==Subscription::Unsubscribed) {
    string query=str(format("DELETE FROM jabber WHERE rss_id=%u AND jid='%s'") % rssid % encloseSQL(subscription.from().bare()));
    mysql_query(&sql,query.c_str());

    Subscription *s=new Subscription(Subscription::Unsubscribed,subscription.from().bare());
    s->setFrom(subscription.to().bare());
    j->send(*s);
    delete(s);
  }
}
