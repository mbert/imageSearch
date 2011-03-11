INCLUDE_DIRECTORIES(
	${IMAGESEARCH_SOURCE_DIR}/backend
  )

LINK_DIRECTORIES(
	${IMAGESEARCH_BINARY_DIR}/backend
  )

ADD_EXECUTABLE(serializationBasedImporter
	serializationBasedMain.cc
  )

TARGET_LINK_LIBRARIES(serializationBasedImporter
	Backend
	wavelet
	Magick++
	jpeg
	CxxUtil
	boost_thread
	boost_serialization
	boost_signals
  ) 
