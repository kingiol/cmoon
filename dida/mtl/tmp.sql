ALTER TABLE email RENAME COLUMN state to statu;

CREATE TABLE inbox (
	id SERIAL,
    mid int NOT NULL DEFAULT 0,
    fresh smallint NOT NULL DEFAULT 1,
    fmid int NOT NULL DEFAULT 0,
    fnick varchar(256) NOT NULL DEFAULT '',
    message varchar(1024) NOT NULL DEFAULT '',
    intime timestamp DEFAULT now()
);

-- 处理用户的“对结果不满意”，
-- 所有这种期盼立即被放到 expect 里，
-- 所有 plan 入库后遍历该表，满足的话插入 meet 表
CREATE TABLE expect (
	id SERIAL,
    mid int NOT NULL DEFAULT 0, --member id
    mname varchar(256) NOT NULL DEFAULT '',
    pid int NOT NULL DEFAULT 0, --plan id
    statu smallint NOT NULL DEFAULT 0,    --0 ok, 1 pause, 2 delete
    addrtype smallint NOT NULL DEFAULT 1, --1 phone, 2 email, 3 all
    --toaddr varchar(128) NOT NULL DEFAULT '',
    gotime smallint NOT NULL DEFAULT 0,  --0 immediatly, 1 day, 2 week

    dad smallint NOT NULL DEFAULT 0,   --0 person, 1 car
    sgeo point NOT NULL DEFAULT '(0,0)',
    rect box NOT NULL DEFAULT '((0,0), (1,1))',
    km numeric NOT NULL DEFAULT 0.1,
    repeat smallint NOT NULL DEFAULT 0, --0 no, 1 day, 2 week
    sdate varchar(16) NOT NULL DEFAULT '2011-11-11', --or 1,3,5
    stime time NOT NULL DEFAULT '08:00:00',
    
    intime timestamp DEFAULT now()
);

CREATE TABLE meet (
    id SERIAL,
    eid int NOT NULL DEFAULT 0, --expect id
    pid int NOT NULL DEFAULT 0, --told plan id
    intime timestamp DEFAULT now()
);

-- OWN Aggregate Functions
CREATE AGGREGATE str_concat(
  basetype    = text,
  sfunc       = textcat,
  stype       = text,
  initcond    = ''
);
