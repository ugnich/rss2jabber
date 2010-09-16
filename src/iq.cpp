#include "rss2jabber.h"

bool RSS2Jabber::handleIq(const IQ &iq) {
  if(iq.subtype()==IQ::Get) {
    if(iq.findExtension(ExtVCard)!=0)
      sendVCardReply(iq.to(),iq.from(),iq.id());
  }
  return true;
}

void RSS2Jabber::handleIqID(const IQ &iq,int context) {}

void RSS2Jabber::sendVCardReply(const JID &from,const JID &to,const string &id) {
  IQ *iq=new IQ(IQ::Result,to,id);
  iq->setFrom(from.bare());
//  iq->addExtension(vcard.clone());
  j->send(*iq);
  delete(iq);
}
