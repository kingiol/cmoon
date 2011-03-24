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
