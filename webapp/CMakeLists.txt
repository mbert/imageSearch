include_directories (${IMAGESEARCH_SOURCE_DIR}/backend)

link_directories (${IMAGESEARCH_BINARY_DIR}/backend)

add_executable (imageSearch.wt main.cc ImageSearch.cc Div.cc)

target_link_libraries (imageSearch.wt wavelet Magick++ CxxUtil Backend wt wthttp) 
