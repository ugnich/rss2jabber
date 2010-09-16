#include "rss2jabber.h"

void RSS2Jabber::sendPresence(const JID &from,const JID &to) {
  Presence *p=new Presence(Presence::Available,to,EmptyString,10);
  p->setFrom(from);
  p->addExtension(caps->clone());
  j->send(*p);
  delete(p);
}

void RSS2Jabber::handlePresence(const Presence &presence) {
  if(presence.subtype()==Presence::Probe) {
    if(getRSSID(presence.to().username())>0) sendPresence(presence.to(),presence.from());
    return;
  }

// UPDATE lastonline
//    string query=str(format("INSERT INTO presences(user_id,resource,priority,presence,status_txt) VALUES (%u,'%s',%d,'%s',%s) ON DUPLICATE KEY UPDATE presence='%s',status_txt=%s,priority=%d") % uid % resource % priority % presencestr % status % presencestr % status % priority);
//    mysql_query(&sql,query.c_str());
}
