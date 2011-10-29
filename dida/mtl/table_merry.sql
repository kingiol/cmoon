CREATE TABLE city (
    id SERIAL,
    pid int NOT NULL DEFAULT 0,
    s varchar(64) NOT NULL DEFAULT '',
    geopos point NOT NULL DEFAULT '(0,0)',
    PRIMARY KEY (id)
);


CREATE TABLE member (
    mid int PRIMARY KEY,
    mname varchar(256) NOT NULL DEFAULT '',
    male smallint NOT NULL DEFAULT 0,
    verify smallint NOT NULL DEFAULT 0, --0 fresh, 1 id credit, 2 subed, 4 jiazhao
    cityid int NOT NULL DEFAULT 0,
    phone varchar(32) NOT NULL DEFAULT '',
    contact varchar(32) NOT NULL DEFAULT '',
    intime timestamp DEFAULT now()
);
CREATE INDEX member_index ON member (male, verify);


CREATE TABLE car (
    mid int PRIMARY KEY,
    model varchar(64) NOT NULL DEFAULT '',
    mdate date DEFAULT '2011-10-10'
);


CREATE TABLE plan (
    id SERIAL,
    mid int NOT NULL DEFAULT 0,
    dad smallint NOT NULL DEFAULT 0,   --0 person, 1 car
    statu smallint NOT NULL DEFAULT 0, --0 fresh, 1 pause, 2 delete, 3 has part
    repeat smallint NOT NULL DEFAULT 0, --0 no, 1 day, 2 week
    seat smallint NOT NULL DEFAULT 4,
    fee int NOT NULL DEFAULT 0,

    scityid int NOT NULL DEFAULT 0,
    ecityid int NOT NULL DEFAULT 0,
    saddr varchar(256) NOT NULL DEFAULT '',
    eaddr varchar(256) NOT NULL DEFAULT '',
    route path NOT NULL DEFAULT '[(0,0), (1,1)]',
    rect box NOT NULL DEFAULT '((0,0), (1,1))',
    km numeric NOT NULL DEFAULT 0.1,

    sdate varchar(16) NOT NULL DEFAULT '2011-10-10', --or 1,3,5
    edate varchar(16) NOT NULL DEFAULT '2011-10-11',
    stime time NOT NULL DEFAULT '00:00:00',
    etime time NOT NULL DEFAULT '00:00:00',
    estmin int NOT NULL DEFAULT 0,
    
    intime timestamp DEFAULT now(),
    uptime timestamp DEFAULT now(),
    PRIMARY KEY (id)
);
CREATE TRIGGER tg_uptime_plan BEFORE UPDATE ON plan FOR EACH ROW EXECUTE PROCEDURE update_time();
