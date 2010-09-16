CREATE TABLE `jabber` (
  `rss_id` int(10) unsigned NOT NULL,
  `jid` char(64) NOT NULL,
  `lastonline` datetime NOT NULL,
  `disabled` tinyint(3) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `rss_id` (`rss_id`,`jid`)
);

CREATE TABLE `rss` (
  `rss_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `jid` char(32) NOT NULL,
  `url` char(128) NOT NULL,
  `lastupdate` datetime DEFAULT NULL,
  PRIMARY KEY (`rss_id`),
  UNIQUE KEY `jid` (`jid`) USING HASH
);

INSERT INTO `rss` VALUES (1,'bash.org.ru','http://bash.org.ru/rss/','2010-09-16 07:44:02'),(2,'ibash.org.ru','http://ibash.org.ru/rss.xml','2010-09-03 11:57:19'),(3,'opennet.ru-news-main','http://www.opennet.ru/opennews/opennews_6.rss','2010-09-16 06:31:04'),(4,'opennet.ru-news-mini','http://www.opennet.ru/opennews/opennews_mini.rss','2010-09-16 07:52:51'),(5,'anekdot.ru-daily','http://www.anekdot.ru/rss/export_j.xml','2010-09-15 20:00:00'),(6,'anekdot.ru-top','http://www.anekdot.ru/rss/export_top.xml','2010-09-14 20:00:00');
