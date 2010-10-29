CREATE TABLE email (
	   id SERIAL,
	   state int NOT NULL DEFAULT 0, -- 0 fresh 1 sended
	   gotime int NOT NULL DEFAULT 0, -- 0 immediatly 1 minute 2 O clock 3 midnight
	   opts varchar(512) NOT NULL DEFAULT '', -- -c /usr/local/etc/email/liuchunsheng.conf -html -s....
	   subject varchar(256) NOT NULL DEFAULT '',
	   sendto text NOT NULL DEFAULT '',
	   cc text NOT NULL DEFAULT '',
	   bcc text NOT NULL DEFAULT '',
	   content text NOT NULL DEFAULT '',
	   checksum varchar(64) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now(),	
	   PRIMARY KEY (id)
);

CREATE INDEX email_index ON email (state, gotime);
CREATE TRIGGER tg_uptime_email BEFORE UPDATE ON email FOR EACH ROW EXECUTE PROCEDURE update_time();
