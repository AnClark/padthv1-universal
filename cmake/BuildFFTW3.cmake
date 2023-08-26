include (ExternalProject)

if (CONFIG_FFTW3_AVX OR CONFIG_FFTW3_AVX2)
    message (WARNING "FFTW3 AVX2 instruction support may not work well on old machines (e.g. Core 2 Series)!")
endif ()

set (FFTW3_PREFIX_PATH ${PROJECT_BINARY_DIR}/fftw3)

# Build FFTW3
ExternalProject_Add (
    dep_fftw3

    URL "http://www.fftw.org/fftw-3.3.10.tar.gz"
    URL_MD5 8ccbf6a5ea78a16dbc3e1306e234cc5c
    UPDATE_DISCONNECTED true  # need this to avoid constant rebuild
    DOWNLOAD_EXTRACT_TIMESTAMP true

    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${FFTW3_PREFIX_PATH} -DENABLE_FLOAT=1 -DBUILD_SHARED_LIBS=0 -DBUILD_TESTS=0 -DENABLE_SSE=1 -DENABLE_SSE2=${CONFIG_FFTW3_SSE2} -DENABLE_AVX=${CONFIG_FFTW3_AVX} -DENABLE_AVX2=${CONFIG_FFTW3_AVX2} -DCMAKE_C_FLAGS=-fPIC

    EXCLUDE_FROM_ALL OFF
)

set (CONFIG_FFTW3 1)

link_directories (${FFTW3_PREFIX_PATH}/lib/)
