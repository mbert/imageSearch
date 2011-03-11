INCLUDE_DIRECTORIES(
	${IMAGESEARCH_SOURCE_DIR}/backend
  )

LINK_DIRECTORIES(
	${IMAGESEARCH_BINARY_DIR}/backend
  )

ADD_EXECUTABLE(imageSearch.wt
	main.cc
	ImageSearch.cc
	SearchResult.cc
	Div.cc
  )

TARGET_LINK_LIBRARIES(imageSearch.wt
	Backend
	wavelet
	Magick++
	jpeg
	CxxUtil
	wt
	wthttp
	wtext
	boost_thread
	boost_serialization
	boost_signals
  ) 
