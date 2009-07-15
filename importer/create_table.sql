create table shirts_128_128_40 (
  id int not null primary key,
  file_name varchar(256) unique not null,
  features_y_plus bytea not null,
  features_y_minus bytea not null,
  features_u_plus bytea not null,
  features_u_minus bytea not null,
  features_v_plus bytea not null,
  features_v_minus bytea not null,
  average_y real not null,
  average_u real not null,
  average_v real not null
);
