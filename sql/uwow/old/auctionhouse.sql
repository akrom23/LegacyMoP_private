ALTER TABLE `auctionhouse`
CHANGE `buyoutprice` `buyoutprice` BIGINT(40) UNSIGNED DEFAULT '0' NOT NULL,
CHANGE `buyguid` `buyguid` BIGINT(40) UNSIGNED DEFAULT '0' NOT NULL,
CHANGE `lastbid` `lastbid` BIGINT(40) UNSIGNED DEFAULT '0' NOT NULL,
CHANGE `startbid` `startbid` BIGINT(40) UNSIGNED DEFAULT '0' NOT NULL,
CHANGE `deposit` `deposit` BIGINT(40) UNSIGNED DEFAULT '0' NOT NULL;