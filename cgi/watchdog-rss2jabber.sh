#!/bin/sh

while [ 1 ]; do
  cd /var/www/rss2jabber.com/cgi/
  /var/www/rss2jabber.com/cgi/bot-rss2jabber >>/var/www/rss2jabber.com/shell.log 2>&1
  sleep 1
done
