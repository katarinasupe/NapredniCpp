add_executable(hash-seq lookup-seq-main.cpp lookup-seq.h)
add_library(hash_external STATIC lookup-seq-main.cpp lookup-seq.h)
add_executable(hash-par ts-lookup-main.cpp ts-lookup.h)
target_link_libraries(hash-par  Threads::Threads)
