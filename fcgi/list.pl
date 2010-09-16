#!/usr/bin/perl

use strict;
use DBI;
use CGI::Fast;
use utf8;

my $dbh=DBI->connect("dbi:mysql:dbname=rss2jabber;host=localhost","rss2jabber","secret",{mysql_enable_utf8=>1});
$dbh->{mysql_auto_reconnect}=1;

FCGI:
while(my $cgi=new CGI::Fast) {
  print qq|Content-type: text/html

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<title>RSS2Jabber</title>
</head>

<body>

<h1>RSS2Jabber</h1>

<ol>
|;

  my $sql="SELECT rss.jid,COUNT(jabber.jid) AS cnt FROM rss LEFT JOIN jabber USING(rss_id) GROUP BY rss_id ORDER BY cnt DESC,rss.jid ASC";
  my $sth=$dbh->prepare($sql) or next FCGI;
  $sth->execute or next FCGI;
  while(my ($jid,$cnt)=$sth->fetchrow) {
    print "<li><a href=\"xmpp:$jid\@rss2jabber.com\">$jid</a> <small>($cnt)</small></li>\n";
  }

  print qq|</ol>

</body>
</html>
|;
}
