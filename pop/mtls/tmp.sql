\c lcs_main

CREATE TABLE appreset (
	   aname varchar(256) NOT NULL DEFAULT '',
	   rlink varchar(256) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   PRIMARY KEY (aname)
);

CREATE FUNCTION merge_appreset(e TEXT, r TEXT) RETURNS VOID AS
$$
BEGIN
    LOOP
        -- first try to update the key
        UPDATE appreset SET rlink = r WHERE aname = e;
        IF found THEN
            RETURN;
        END IF;
        -- not there, so try to insert the key
        -- if someone else inserts the same key concurrently,
        -- we could get a unique-key failure
        BEGIN
            INSERT INTO appreset(aname, rlink) VALUES (e, r);
            RETURN;
        EXCEPTION WHEN unique_violation THEN
            -- do nothing, and loop to try the UPDATE again
        END;
    END LOOP;
END
$$
LANGUAGE plpgsql;

\c lcs_aux
CREATE TABLE improve (
	id SERIAL,
	aid int NOT NULL DEFAULT 0,
	aname varchar(256) NOT NULL DEFAULT '',
	state int NOT NULL DEFAULT 0,
	title varchar(256) NOT NULL DEFAULT '',
	content varchar(1024) NOT NULL DEFAULT '',
	contact varchar(64) NOT NULL DEFAULT '',
	intime timestamp DEFAULT now(),
	PRIMARY KEY (id)
);

CREATE INDEX improve_index ON improve (aid, state);
