create table images_256_256 (
  id int not null primary key,
  file_name varchar(256) unique not null,
  feature_vector_y bytea not null,
  feature_vector_u bytea not null,
  feature_vector_v bytea not null,
  average_y real not null,
  average_u real not null,
  average_v real not null
);
