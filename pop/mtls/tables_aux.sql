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
	   bid int NOT NULL DEFAULT 0,
	   pid int NOT NULL DEFAULT 0,
	   content text NOT NULL DEFAULT '',
	   state int NOT NULL DEFAULT 0,
	   author varchar(256) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   PRIMARY KEY (id)	
);

CREATE INDEX comment_index ON comment (bid, pid, state);
