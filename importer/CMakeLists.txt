INCLUDE_DIRECTORIES(
	${IMAGESEARCH_SOURCE_DIR}/backend
  )

LINK_DIRECTORIES(
	${IMAGESEARCH_BINARY_DIR}/backend
  )

ADD_EXECUTABLE(dbBasedImporter
	dbBasedMain.cc
  )

TARGET_LINK_LIBRARIES(dbBasedImporter
	Backend
	wavelet
	Magick++
	jpeg
	CxxUtil
	pqxx
	boost_thread-mt
  ) 
