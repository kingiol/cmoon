CREATE TABLE stat (
	   id SERIAL,
	   type int NOT NULL DEFAULT 0,
	   count int NOT NULL DEFAULT 0,
	   intime timestamp DEFAULT now(),
	   PRIMARY KEY (id)
);

CREATE INDEX app_stat ON stat (type, count);

--CREATE TRIGGER tg_uptime_stat BEFORE UPDATE ON stat FOR EACH ROW EXECUTE PROCEDURE update_time();
