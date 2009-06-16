INCLUDE_DIRECTORIES(
	${IMAGESEARCH_SOURCE_DIR}/backend
  )

LINK_DIRECTORIES(
	${IMAGESEARCH_BINARY_DIR}/backend
  )

ADD_EXECUTABLE(importer
	main.cc
  )

TARGET_LINK_LIBRARIES(importer
	Backend
	wavelet
	Magick++
	jpeg
	CxxUtil
	pqxx
	boost_thread-mt
  ) 
