CREATE TABLE appinfo (
	   aid int PRIMARY KEY,
	   aname varchar(256) NOT NULL DEFAULT '',
	   asn varchar(256) NOT NULL DEFAULT '',
	   masn varchar(256) NOT NULL DEFAULT '',
	   email varchar(256) NOT NULL DEFAULT '',
	   state int NOT NULL DEFAULT 0,
	   pid int NOT NULL DEFAULT 0,	--0: superuser, x: X site's custormer
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now()
);

CREATE INDEX app_index ON appinfo (state);

CREATE TRIGGER tg_uptime_app BEFORE UPDATE ON appinfo FOR EACH ROW EXECUTE PROCEDURE update_time();


CREATE TABLE userinfo (
	   uid int,
	   uname varchar(256) NOT NULL DEFAULT '',
	   aid int,
	   aname varchar(256) NOT NULL DEFAULT '',
	   ip varchar(256) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now(),
	   PRIMARY KEY (uid, aid)
);

CREATE INDEX user_index ON userinfo (aid);

CREATE TRIGGER tg_uptime_user BEFORE UPDATE ON userinfo FOR EACH ROW EXECUTE PROCEDURE update_time();
