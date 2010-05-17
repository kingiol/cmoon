alter table lcsjoin drop unamea;
alter table lcsjoin add oid int not null default 0;
alter table lcsjoin add oname varchar(256) not null default '';
