CREATE TABLE lcsjoin (
	   id SERIAL,
	   uid int NOT NULL DEFAULT 0,			   --userid
	   uname varchar(256) NOT NULL DEFAULT '', --username
	   aid int NOT NULL DEFAULT 0,			   --appid
	   aname varchar(256) NOT NULL DEFAULT '', --appname
	   oid int NOT NULL DEFAULT 0,			   --oserid
	   oname varchar(256) NOT NULL DEFAULT '', --appname, which contact with username
	   ip varchar(256) NOT NULL DEFAULT '',
	   refer varchar(256) NOT NULL DEFAULT '',
	   url varchar(256) NOT NULL DEFAULT '',
	   title varchar(256) NOT NULL DEFAULT '',
	   retcode int NOT NULL DEFAULT 0,
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now(),
	   PRIMARY KEY (id)
);

CREATE INDEX app_index ON lcsjoin (aid, uid, retcode);

CREATE TRIGGER tg_uptime_lcsjoin BEFORE UPDATE ON lcsjoin FOR EACH ROW EXECUTE PROCEDURE update_time();



CREATE TABLE visit (
	   id SERIAL,
	   jid int NOT NULL DEFAULT 0,
	   url varchar(256) NOT NULL DEFAULT '',
	   title varchar(256) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now(),
	   PRIMARY KEY (id)
);

CREATE INDEX app_visit ON visit (jid);

CREATE TRIGGER tg_uptime_visit BEFORE UPDATE ON visit FOR EACH ROW EXECUTE PROCEDURE update_time();
