CREATE TABLE md (
  security_id integer,
  stamp integer,
  last_price float
);
CREATE INDEX idx_stamp ON md (stamp);
