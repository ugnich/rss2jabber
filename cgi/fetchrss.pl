#!/usr/bin/perl

use strict;
use DBI;
use XML::RSS::Parser;
use DateTime::Format::Mail;
use DateTime::Format::ISO8601;
use Net::Jabber;

my $dbh=DBI->connect("dbi:mysql:dbname=rss2jabber;host=localhost","rss2jabber","secret",{mysql_enable_utf8=>1});

my $con=new Net::Jabber::Component();
$con->Connect(hostname=>"127.0.0.1",port=>5347,componentname=>"sender.rss2jabber.com");
$con->AuthSend(secret=>"secret");

################################################################################

my $sql="SELECT rss_id,jid,url,DATE_FORMAT(lastupdate,'%Y%m%dT%H%i%S') FROM rss";
my $sth=$dbh->prepare($sql) or die;
$sth->execute or die;
while(my ($rssid,$rssjid,$url,$lastupdate)=$sth->fetchrow) {

  my $mindate;
  if(defined($lastupdate) && $lastupdate ne "") { $mindate=DateTime::Format::ISO8601->parse_datetime($lastupdate); }

  $sql="SELECT jid FROM jabber WHERE rss_id=$rssid";
  my $sth2=$dbh->prepare($sql) or next;
  $sth2->execute or next;
  my @jids;
  while(my ($jid)=$sth2->fetchrow) { push(@jids,$jid); }
  $sth2->finish;

  my $p=new XML::RSS::Parser;
  my $f=$p->parse_uri($url);
  my $maxdate=$mindate;
  foreach my $item ( $f->query('//item') ) { 
    my $idate=DateTime::Format::Mail->parse_datetime($item->query('pubDate')->text_content);
    $idate->set_time_zone('UTC');
    if(!defined($mindate) || DateTime->compare($mindate,$idate)==-1) {
      if(!defined($maxdate) || DateTime->compare($maxdate,$idate)==-1) { $maxdate=$idate; }

      my $txt=$item->query('description')->text_content;
      if($txt ne "") {
        $txt=&decodeHTML($txt);

        my $subj=$item->query('title')->text_content;
	$subj=&decodeHTML($subj);

        my $link=$item->query('link')->text_content;

        my $m=new Net::XMPP::Message();
        $m->SetMessage(from=>$rssjid."\@rss2jabber.com",type=>"headline",subject=>$subj,body=>$txt."\n\n$link");
	for(my $i=0; $i<@jids; $i++) {
	  $m->SetTo($jids[$i]);
	  $con->Send($m);
	}
      }
    }
  }
  
  $sql="UPDATE rss SET lastupdate='".$maxdate->strftime('%Y-%m-%d %H:%M:%S')."' WHERE rss_id=$rssid";
  $dbh->do($sql);
}
$sth->finish;

$con->Disconnect();

################################################################################
sub decodeHTML {
  my $txt=shift;
  $txt=~s/\&quot\;/\"/g;
  $txt=~s/\<br\>/\n/g;
  $txt=~s/\<br\/\>/\n/g;
  $txt=~s/\<br \/\>/\n/g;
  $txt=~s/\&lt\;/\</g;
  $txt=~s/\&gt\;/\>/g;
  return $txt;
}
