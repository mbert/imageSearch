create table images_256_256 (
  id int not null primary key,
  file_name varchar(256) not null,
  feature_vector_y varchar(8192) not null,
  feature_vector_u varchar(8192) not null,
  feature_vector_v varchar(8192) not null,
  average_y real not null,
  average_u real not null,
  average_v real not null
);
