CREATE TABLE blog (
	   id SERIAL,
	   title varchar(256) NOT NULL DEFAULT '',
	   content text NOT NULL DEFAULT '',
	   state int NOT NULL DEFAULT 0,
	   author varchar(256) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now(),	
	   PRIMARY KEY (id)	
);
	   
CREATE INDEX blog_index ON blog (state);
CREATE TRIGGER tg_uptime_blog BEFORE UPDATE ON blog FOR EACH ROW EXECUTE PROCEDURE update_time();


CREATE TABLE comment (
	   id SERIAL,
	   type int NOT NULL DEFAULT 0, -- 0 blog
	   state int NOT NULL DEFAULT 0,
	   oid int NOT NULL DEFAULT 0,
	   pid int NOT NULL DEFAULT 0,
	   ip varchar(256) NOT NULL DEFAULT '',
	   addr varchar(256) NOT NULL DEFAULT '',
	   author varchar(256) NOT NULL DEFAULT '',
	   content text NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   uptime timestamp DEFAULT now(),	
	   PRIMARY KEY (id)	
);

CREATE INDEX comment_index ON comment (type, state, cid, pid);
CREATE TRIGGER tg_uptime_comment BEFORE UPDATE ON comment FOR EACH ROW EXECUTE PROCEDURE update_time();
