05-16
lcs_dyn:
alter table lcsjoin drop unamea;
alter table lcsjoin add oid int not null default 0;
alter table lcsjoin add oname varchar(256) not null default '';

lcs_main
alter table appinfo add pid int not null default 0;

CREATE TABLE userinfo (
	   uid int,
	   uname varchar(256) NOT NULL DEFAULT '',
	   aid int,
	   aname varchar(256) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now(),
	   PRIMARY KEY (uid, aid)
);

CREATE INDEX user_index ON userinfo (aid);

CREATE TRIGGER tg_uptime_user BEFORE UPDATE ON userinfo FOR EACH ROW EXECUTE PROCEDURE update_time();

