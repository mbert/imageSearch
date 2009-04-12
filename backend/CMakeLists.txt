INCLUDE_DIRECTORIES(
	${IMAGESEARCH_SOURCE_DIR}/backend
  )
ADD_LIBRARY(Backend
	Backend.cc
	postgresql/PostgresQl.cc
  )
